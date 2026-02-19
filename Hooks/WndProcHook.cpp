#include "pch.h"
#include "WndProcHook.h"

#include "State/CheatState.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
decltype(WndProc)* original;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_KEYDOWN && wParam == VK_INSERT) {
		CheatState::menu_open = !CheatState::menu_open;
	}

	if (CheatState::menu_open)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		return WM_CREATE;
	}

	return CallWindowProcW((WNDPROC)original, hWnd, msg, wParam, lParam);
}


inline void(__fastcall* FX_FireBullets)(
	int player_idx,
	__int64 Origin,
	__int64 vAngles,
	int a4,
	int a5,
	int iSeed,
	int a7,
	int a8);

void __fastcall _FX_FireBullets(int player_idx,
	__int64 Origin,
	__int64 vAngles,
	int a4,
	int a5,
	int iSeed,
	int a7,
	int a8) {
	std::println("Seed {}", iSeed);
	FX_FireBullets(player_idx, Origin, vAngles, a4, a5, iSeed, a7, a8);
}

void Hooks::hook_wndproc() {
	original = (decltype(original))SetWindowLongPtr(FindWindow(L"Valve001", NULL), GWLP_WNDPROC, (LONG_PTR)WndProc);

	uintptr_t original;
	hook(Utility::modules["server.dll"].base + 0x3252E0, (uintptr_t)_FX_FireBullets, original); //
	FX_FireBullets = (decltype(FX_FireBullets))original;
}

void Hooks::unhook_wndproc() {
	SetWindowLongPtr(FindWindow(L"Valve001", NULL), GWLP_WNDPROC, (LONG_PTR)original);
	
}

