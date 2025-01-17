#include "../Backend/Backend.h"

Backend::presentVariable originalPresent;
Backend::presentVariable hookedPresent;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool init = false;

Backend RunBackend;

bool Backend::DirectXPresentHook()
{
	ZeroMemory(&m_gSwapChainDescription, sizeof(m_gSwapChainDescription));

	m_gSwapChainDescription.BufferCount = 2;
	m_gSwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_gSwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_gSwapChainDescription.OutputWindow = GetForegroundWindow();
	m_gSwapChainDescription.SampleDesc.Count = 1;
	m_gSwapChainDescription.Windowed = TRUE;
	m_gSwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT createDevice = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, m_gFeatureLevels, 2, D3D11_SDK_VERSION, &m_gSwapChainDescription, &m_gSwapChain, &m_gDevice, nullptr, nullptr);
		
	if (FAILED(createDevice)) 
		return false; // Don't return false make an endless cycle (only if u wanna go cpu boom) 

	void** DX11Vtable = *reinterpret_cast<void***>(m_gSwapChain);

	UnloadDevices(false); // Don't need to reset mainrendertargetview
	hookedPresent = (Backend::presentVariable)DX11Vtable[8]; // 8. Virtual table is present

	return true;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ImGuiIO& io = ImGui::GetIO();

	RunBackend.CursorFix(RunBackend.m_bOpenMenu);
	io.MouseDrawCursor = RunBackend.m_bOpenMenu;

	if (RunBackend.m_bOpenMenu) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(RunBackend.m_goriginalWndProc, hWnd, uMsg, wParam, lParam);
}

void Backend::LoadImGui(HWND window, ID3D11Device* device, ID3D11DeviceContext* context)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);
} // Loading the imgui

void Backend::DrawImGui(ID3D11DeviceContext* context, ID3D11RenderTargetView* targetview)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	if (m_bOpenMenu)
	{
		ImGui::Begin("Pixel Gun Currency Adder", &m_bOpenMenu);
		{
			ImGui::Text("Wow! Such cool!");
			ImGui::Separator();
			ImGui::Combo("Currency Type", &CurrencyType, CurrencyList, IM_ARRAYSIZE(CurrencyList));
			ImGui::SliderInt("Currency Amount", &CurrencyAmount, 0, 99999, "%d", 0);
			if (ImGui::Button("Add Currency")) { RunBackend.AddCurrency = true; }
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();

	context->OMSetRenderTargets(1, &targetview, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // Drawing the imgui menu
}

void Backend::UnloadImGui()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Backend::UnloadDevices(bool renderTargetViewReset)
{
	if(renderTargetViewReset) if (m_gMainRenderTargetView) { m_gMainRenderTargetView->Release(); m_gMainRenderTargetView = nullptr; }
	if (m_gPointerContext) { m_gPointerContext->Release(); m_gPointerContext = nullptr; }
	if (m_gDevice) { m_gDevice->Release(); m_gDevice = nullptr; }
	SetWindowLongPtr(m_gWindow, GWLP_WNDPROC, (LONG_PTR)(m_goriginalWndProc));
}

static long __stdcall PresentHook(IDXGISwapChain* pointerSwapChain, UINT sync, UINT flags)
{
	if (!init) {
		if (SUCCEEDED(pointerSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&RunBackend.m_gDevice)))
		{
			RunBackend.m_gDevice->GetImmediateContext(&RunBackend.m_gPointerContext);
			pointerSwapChain->GetDesc(&RunBackend.m_gPresentHookSwapChain);
			RunBackend.m_gWindow = RunBackend.m_gPresentHookSwapChain.OutputWindow;

			pointerSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RunBackend.m_gPointerBackBuffer);
			if (RunBackend.m_gPointerBackBuffer != nullptr) RunBackend.m_gDevice->CreateRenderTargetView(RunBackend.m_gPointerBackBuffer, NULL, &RunBackend.m_gMainRenderTargetView); // from backbuffer to our monitor
			RunBackend.m_gPointerBackBuffer->Release();

			RunBackend.LoadImGui(RunBackend.m_gWindow, RunBackend.m_gDevice, RunBackend.m_gPointerContext); // Load imgui!!!
			RunBackend.m_goriginalWndProc = (WNDPROC)SetWindowLongPtr(RunBackend.m_gWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

			init = true;
		}
		else
			return originalPresent(pointerSwapChain, sync, flags); // Returning original
	}

	if (Utils::keyPressed(VK_RCONTROL) || Utils::keyPressed(VK_INSERT) || Utils::keyPressed(VK_F1))
		RunBackend.m_bOpenMenu = !RunBackend.m_bOpenMenu;

	RunBackend.DrawImGui(RunBackend.m_gPointerContext, RunBackend.m_gMainRenderTargetView); // Draw imgui every time
	return originalPresent(pointerSwapChain, sync, flags); // Return the original so no stack corruption
}

bool Backend::Load()
{
	RunBackend.DirectXPresentHook(); // This always okay if game directx11
	MH_Initialize(); // Aint no error checking cuz if minhook bad then its your problem 

	MH_CreateHook(reinterpret_cast<void**>(hookedPresent), &PresentHook, reinterpret_cast<void**>(&originalPresent)); 
	MH_EnableHook(hookedPresent);
	return true;
}

void Backend::Unload()
{
	UnloadImGui();
	UnloadDevices(true);
}