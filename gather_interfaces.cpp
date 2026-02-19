#include "pch.h"
#include "gather_interfaces.h"

#include "interfaces.h"

#define clientdll Utility::modules["client.dll"]

void gather_interfaces() {
	I::BaseClientDLL =    clientdll.get_interface<IBaseClientDLL*>(CLIENT_DLL_INTERFACE_VERSION);
	I::ClientEntityList = clientdll.get_interface<IClientEntityList*>(VCLIENTENTITYLIST_INTERFACE_VERSION);
	I::GameMovement =     clientdll.get_interface<IGameMovement*>(INTERFACENAME_GAMEMOVEMENT);
	I::ClientPrediction = clientdll.get_interface<CPrediction*>(VCLIENT_PREDICTION_INTERFACE_VERSION);

	I::EngineClient = enginedll.get_interface<IVEngineClient*>(VENGINE_CLIENT_INTERFACE_VERSION);
	I::EngineVGui =   enginedll.get_interface<IEngineVGui*>(VENGINE_VGUI_VERSION);
	I::EngineTrace =  enginedll.get_interface<IEngineTrace*>(INTERFACEVERSION_ENGINETRACE_CLIENT);
	I::RenderView =   enginedll.get_interface<IVRenderView*>(VENGINE_RENDERVIEW_INTERFACE_VERSION);
	I::ModelRender =  enginedll.get_interface<IVModelRender*>(VENGINE_HUDMODEL_INTERFACE_VERSION);
	I::ModelInfo =    enginedll.get_interface<IVModelInfo*>(VMODELINFO_CLIENT_INTERFACE_VERSION);



	I::PhysProps = Utility::modules["vphysics.dll"].get_interface<decltype(I::PhysProps)>(VPHYSICS_SURFACEPROPS_INTERFACE_VERSION);
	//I::VModelInfo = enginedll.get_interface<IVModelInfo*>("VModelInfoClient006");
	//I::GameEventManager = enginedll.get_interface<IGameEventManager2*>("GAMEEVENTSMANAGER002");

	I::StudioRender =     Utility::modules["studiorender.dll"].get_interface<IStudioRender*>(STUDIO_RENDER_INTERFACE_VERSION);
	//I::MatSystemSurface = Utility::modules["vguimatsurface.dll"].get_interface<IMatSystemSurface*>(MAT_SYSTEM_SURFACE_INTERFACE_VERSION);
	I::MaterialSystem =   Utility::modules["materialsystem.dll"].get_interface<IMaterialSystem*>(MATERIAL_SYSTEM_INTERFACE_VERSION);
	I::Cvar =             Utility::modules["vstdlib.dll"].get_interface<ICvar*>(CVAR_INTERFACE_VERSION);

	I::ClientState = (CClientState*)Utility::find_bytes(enginedll, "48 8D 0D ? ? ? ? 48 8B 5C 24 ? 48 83 C4 ? 5F E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC 48 89 6C 24").fixRip(3).get();
	I::GlobalVars = (decltype(I::GlobalVars))Utility::find_bytes(enginedll, "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B CA").fixRip(3).get();
	I::Input = (decltype(I::Input))Utility::find_bytes(clientdll, "48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 85 C0 0F 84 ? ? ? ? F3 0F 10 05").fixRip(3).deref().get();



	Fn::SetPredictionRandomSeed = (decltype(Fn::SetPredictionRandomSeed))Utility::find_bytes(clientdll, "48 85 C9 75 0B C7 05 ? ? ? ? FF FF FF FF C3 8B 41 38 89 05 ? ? ? ? C3").get();
	Fn::UsingStandardWeaponsInVehicle = (decltype(Fn::UsingStandardWeaponsInVehicle))Utility::find_bytes(clientdll, "48 89 5C 24 08 57 48 83 EC 20 8B 91 0C 16 00 00 48 8B F9 85 D2 74 6E B8 FF 1F 00 00 83 FA FF 74 03 0F B7 C2 4C 8B 05").get();
	Fn::UpdateButtonState = (decltype(Fn::UpdateButtonState))Utility::find_bytes(clientdll, "44 8B 81 B4 15 00 00 89 91 B4 15 00 00 44 89 81 A8 15 00 00 44 33 C2 41 8B C0 23 C2 F7 D2 41 23 D0 89 81 AC 15 00 00 89 91 B0 15 00 00 C3").get();
	Fn::PredictionPlayer = (decltype(Fn::PredictionPlayer))Utility::find_bytes(clientdll, "0F 57 F6 48 89 3D ? ? ? ? 66 0F 6E 87 B0 16 00 00 0F 5B C0 F3 0F 59 40 1C F3 0F 11 40 0C 41 80 7E 0F 00").fixRip(6).deref().get();
	
	Fn::UTIL_ClipTraceToPlayers = (decltype(Fn::UTIL_ClipTraceToPlayers))Utility::find_bytes(clientdll, 
		"48 8B C4 48 89 70 10 48 89 78 18 55 41 54 41 55 41 56 41 57 48 8D 68 B8 48 81 EC 20 01 00 00 F3 0F 10 19 4D 8B F1 F3 0F 10 0A 45 8B F8 F3 0F 10 61 04 F3 0F 5C CB F3 0F 10 52 04 4C 8B E2 F3 0F 10 69 08 F3 0F 5C D4 F3 0F 10 42 08").get();
	

	auto clientlist = Utility::find_bytes(clientdll, "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B D8 48 85 C0 74 29").fixRip(3).deref().get();
	std::println("VCLIENTENTITYLIST_INTERFACE_VERSION: {:X}, Sigscan: {:X}", (uintptr_t)I::ClientEntityList, clientlist);

	//Fn::GetBulletTypeParameters = (decltype(Fn::GetBulletTypeParameters))Utility::find_bytes(clientdll, 
	//	"48 8B C4 48 89 50 10 48 89 48 08 55 53 56 41 54 41 56 48 8D A8 38 FD FF FF 48 81 EC A0 03 00 00 0F 29 70 B8").get();

	//Fn::GetCsWpnData = (decltype(Fn::GetCsWpnData))Utility::find_bytes(clientdll,
		//"48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 30 0F 29 74 24 20 49 8B D8 48 8B F2").get();
	//
	// 
	// iinput 48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 85 C0 0F 84 ? ? ? ? F3 0F 10 05
	//I::PhysProps = Utility::modules["vphysics.dll"].get_interface<IPhysicsSurfaceProps*>("VPhysicsSurfaceProps001");
}
