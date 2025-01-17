#pragma once

#define WIN32_LEAN_AND_MEAN
#define UNINJECT_KEY VK_DELETE
#define OPEN_MENU_KEY VK_INSERT
#include <windows.h>
#include <thread>
#include <iostream>
#include <intrin.h>
#include <cstdint>
#include <string>
#include <MinHook.h> // IF ERROR: Install vcpkg then run this: "vcpkg install minhook:x64-windows-static"

#include "./ImGui/imgui.h"
#include "./ImGui/imgui_impl_win32.h"
#include "./ImGui/imgui_impl_dx11.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace Utils {
	inline bool keyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 1) != 0;
	}
}