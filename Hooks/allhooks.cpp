#include "pch.h"
#include "allhooks.h"

#include "HookCalcViewModel.h"
#include "hookCL_Move.h"
#include "hookd3d.h"
#include "HookIBaseClientDLL.h"
#include "HookRenderView.h"
#include "HookRunCommand.h"
#include "surfacehook.h"
#include "WndProcHook.h"
#include "hookStudioRender.h"

void Hooks::do_all_hooks() {
	Hooks::hook_wndproc();
	Hooks::hook_d3d();
	Hooks::hook_RenderView();
	Hooks::hook_CalcViewModel();

	Hooks::hook_RunCommand();
	Hooks::hook_CL_Move();
	Hooks::hook_StudioRender();
}
