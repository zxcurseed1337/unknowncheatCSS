#include "pch.h"
#include "HookRunCommand.h"

void __fastcall hkRunCommand(void* ecx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper);
decltype(hkRunCommand)* RunCommand;

void __fastcall hkRunCommand(void* ecx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper) {

	RunCommand(ecx, player, ucmd, moveHelper);

	if (!I::MoveHelper)
		I::MoveHelper = moveHelper;
}

void Hooks::hook_RunCommand() {
	uintptr_t original;

	hook(Utility::find_virtual(I::ClientPrediction, 17u).get(), reinterpret_cast<uintptr_t>(hkRunCommand), original);
	RunCommand = reinterpret_cast<decltype(RunCommand)>(original);
}

