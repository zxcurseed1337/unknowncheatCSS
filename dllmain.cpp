#include "pch.h"

#include "gather_interfaces.h"
#include "interfaces.h"

#include "Hooks/WndProcHook.h"

#include "Hooks/allhooks.h"
#include "State/CheatState.h"

#define MH_CALL(call, msg) { if(call != MH_OK) { std::println(msg); FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(param), EXIT_FAILURE); return FALSE;} }

DWORD WINAPI main_thread(LPVOID param) {
	if (GetConsoleWindow() == NULL) {
		if (AllocConsole() == 0) {
			MessageBox(NULL, L"Failed to alloc console?", L"Critical error.", MB_OK | MB_ICONERROR);
		}
		SetConsoleOutputCP(CP_UTF8);
		(void)freopen("CONOUT$", "w", stdout);
		(void)freopen("CONIN$", "r", stdin);
		SetConsoleTitle(L"cssourcex64 debug");
	}

	

	try {
		gather_interfaces();
	}
	catch(std::exception e) {
		std::println("Exception during initalize: {}", e.what());
		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(param), EXIT_SUCCESS);
		return FALSE;
	}

	CheatState::screensize_y = I::BaseClientDLL->GetScreenHeight();
	CheatState::screensize_x = I::BaseClientDLL->GetScreenWidth();

	MH_CALL(MH_Initialize(), "MH_Initialize failed.");

	Hooks::do_all_hooks();


	MH_CALL(MH_EnableHook(MH_ALL_HOOKS), "Failed to enable hooks!");

	IMaterial* flash = I::MaterialSystem->FindMaterial("effects\\flashbang", "ClientEffect textures");
	IMaterial* flash2 = I::MaterialSystem->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
	IMaterial* smoke = I::MaterialSystem->FindMaterial("particle\\particle_smokegrenade1", NULL);



	if (flash && flash2 && smoke) {
		flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		flash2->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		smoke->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
	}



	if (smoke)
		
	std::println("Successfully initialized.");

	while (!GetAsyncKeyState(VK_F11))
		std::this_thread::sleep_for(500ms);

	Hooks::unhook_wndproc();

	//flash = I::MaterialSystem->FindMaterial("effects\\flashbang", "ClientEffect textures");
	//flash2 = I::MaterialSystem->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
	//smoke = I::MaterialSystem->FindMaterial("particle\\particle_smokegrenade1", NULL);
	//if (flash && flash2 && smoke) {
	//	flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//	flash2->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//	smoke->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//}



	MH_CALL(MH_Uninitialize(), "MH_Uninitialize failed.");
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(param), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_DETACH) {
		std::println("Successfully exited.");
		return FALSE;
	}

	if (reason != DLL_PROCESS_ATTACH) {
		return FALSE;
	}

	const HANDLE thrd{ CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main_thread, inst, 0, 0) };

	if (!thrd) {
		return FALSE;
	}

	CloseHandle(thrd);

	return TRUE;
}