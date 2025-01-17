#pragma once
#include "../Utils/Utils.h"

class Backend
{
public:
	bool Load();
	void Unload();
public:
	bool DirectXPresentHook();
	void UnloadDevices(bool renderTargetViewReset);
public:
	void LoadImGui(HWND window, ID3D11Device* device, ID3D11DeviceContext* context);
	void DrawImGui(ID3D11DeviceContext* context, ID3D11RenderTargetView* targetview);
	void UnloadImGui();
public:
	typedef long(__stdcall* presentVariable)(IDXGISwapChain*, UINT, UINT);
public:
	DXGI_SWAP_CHAIN_DESC m_gSwapChainDescription{};
	IDXGISwapChain* m_gSwapChain = nullptr;
	ID3D11Device* m_gDevice = nullptr;
	const D3D_FEATURE_LEVEL m_gFeatureLevels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	HWND m_gWindow = NULL;
	ID3D11DeviceContext* m_gPointerContext = nullptr;
	ID3D11RenderTargetView* m_gMainRenderTargetView = nullptr;
	ID3D11Texture2D* m_gPointerBackBuffer = nullptr;
public:
	WNDPROC m_goriginalWndProc;
	DXGI_SWAP_CHAIN_DESC m_gPresentHookSwapChain;
public:
	bool m_bOpenMenu, AddCurrency = false; 
	int CurrencyType, CurrencyAmount = 0;
	const char* CurrencyList[12] = {"GemsCurrency", "Coins", "ClanSilver", "ClanLootBoxPoints", "Coupons", "PixelPassCurrency", "RouletteAdsCurrency", "RouletteAdsSpin", "PixelBucks", "BattlePassCurrency", "CurrencyCompetitionTier1", "CurrencyCompetitionTier2"};
	uint64_t baseAddr = (uint64_t)GetModuleHandleA("GameAssembly.dll");

	// Cursor fix
	void CursorFix(bool clip) {
		HWND hwnd = FindWindowA(NULL, "Pixel Gun 3D");
		if (!hwnd) return;

		HWND activeWindow = GetForegroundWindow();

		if (activeWindow != hwnd || IsIconic(hwnd)) return;

		if (!clip) {
			RECT windowRect;
			GetWindowRect(hwnd, &windowRect);

			if (windowRect.left >= 0 && windowRect.top >= 0) {
				POINT center;
				center.x = (windowRect.left + windowRect.right) / 2;
				center.y = (windowRect.top + windowRect.bottom) / 2;

				SetCursorPos(center.x, center.y);
			}
		}
		else {
			RECT screenRect = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
			ClipCursor(&screenRect);
		}
	}
};

extern Backend RunBackend;