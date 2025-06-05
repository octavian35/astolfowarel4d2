#include "WndProc.h"
#include "../../Features/NewMenu/CMenu.h"
#include "../../Features/NewMenu/ImGui/imgui.h"
#include "../../Features/NewMenu/ImGui/imgui_impl_win32.h"
#include "../../Features/NewMenu/ImGui/imgui_impl_dx9.h"
#include "../../Features/NewMenu/ImGui/imgui_internal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LONG WINAPI WndProc::Detour(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_Menu.menuOpen) {
		//g_Interfaces.InputSystem->ResetInputState();
		//g_Interfaces.InputSystem->EnableInput(false);
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return 1;
	}
	return CallWindowProcW(WndProc, hWnd, uMsg, wParam, lParam);
}

void WndProc::Initialize()
{
	while (!hwWindow)
	{
		hwWindow = FindWindowW(_(L"Valve001"), 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	WndProc = reinterpret_cast<WNDPROC>(SetWindowLongW(hwWindow, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(Detour)));
}

void WndProc::UnInitialize()
{
	SetWindowLongW(hwWindow, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
}