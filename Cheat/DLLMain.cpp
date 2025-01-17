#include "Utils/Utils.h"
#include "Backend/Backend.h"
#include "Game/Hooks.h"

#define QWORD int64_t

// Run it in Release | x64, already set up everything

QWORD WINAPI MainThread(LPVOID param)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);;
	SetConsoleTitleA("PG3D-Currency-Adder");
	const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	IL2CPP::INIT();
	RunBackend.Load(); // Load everything

	Hooks();
	Logger::Log("Menu binds: Right Control, F1, Insert\n", Logger::LogLevel::INFO);

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(420));
		if (Utils::keyPressed(UNINJECT_KEY)) break; // Uninject
	}
	
	RunBackend.Unload();
	return 0;
}

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case 1:
		HANDLE hMainThread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, nullptr);
		if (hMainThread)
			CloseHandle(hMainThread);
		break;
	}

	return TRUE;
}