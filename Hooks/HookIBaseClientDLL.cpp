#include "pch.h"
#include "HookIBaseClientDLL.h"

#include "Events/Events.h"

void __fastcall hkFrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage);
decltype(hkFrameStageNotify)* FrameStageNotify;

void __fastcall hkLevelShutdown(void* ecx, void* edx);
decltype(hkLevelShutdown)* LevelShutdown;

void __fastcall hkLevelInitPostEntity(void* ecx, void* edx);
decltype(hkLevelInitPostEntity)* LevelInitPostEntity;

void __fastcall hkFrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage) {
	FrameStageNotify(ecx, edx, curStage);
	CheatEvents::FrameStageNotify(curStage);
}
void __fastcall hkLevelShutdown(void* ecx, void* edx) {
	LevelShutdown(ecx, edx);
	CheatEvents::LevelShutdown();
}
void __fastcall hkLevelInitPostEntity(void* ecx, void* edx) {
	LevelInitPostEntity(ecx, edx);
	CheatEvents::LevelInitPostEntity();
}

void Hooks::hook_IBaseClientDLL() {
	uintptr_t original;

	hook(Utility::find_virtual(I::BaseClientDLL, 35u).get(), reinterpret_cast<uintptr_t>(hkFrameStageNotify), original);
	FrameStageNotify = reinterpret_cast<decltype(FrameStageNotify)>(original);

	hook(Utility::find_virtual(I::BaseClientDLL, 7u).get(), reinterpret_cast<uintptr_t>(hkLevelShutdown), original);
	LevelShutdown = reinterpret_cast<decltype(LevelShutdown)>(original);

	hook(Utility::find_virtual(I::BaseClientDLL, 6u).get(), reinterpret_cast<uintptr_t>(hkLevelInitPostEntity), original);
	LevelInitPostEntity = reinterpret_cast<decltype(LevelInitPostEntity)>(original);
}
