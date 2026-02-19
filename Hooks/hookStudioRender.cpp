#include "pch.h"
#include "hookStudioRender.h"

void __fastcall hkSetColorModulation(decltype(I::StudioRender) pthis, float* mod);
decltype(hkSetColorModulation)* SetColorModulation;

void __fastcall hkSetColorModulation(decltype(I::StudioRender) pthis, float* mod) {
	static uintptr_t addr1 = Fn::CStaticProp__DrawModel__IStudioRender_SetColorModulation_call_return_adr1_file_offset + enginedll.base;
	static uintptr_t addr2 = Fn::CStaticProp__DrawModel__IStudioRender_SetColorModulation_call_return_adr2_file_offset + enginedll.base;

	uintptr_t ret_addr = (uintptr_t)_ReturnAddress();
	if (ret_addr == addr1 || ret_addr == addr2) {
		mod[0] = Vars::Visual::modluation_r;
		mod[1] = Vars::Visual::modluation_g;
		mod[2] = Vars::Visual::modluation_b;
	}

	SetColorModulation(pthis, mod);
}

void Hooks::hook_StudioRender() {
	uintptr_t original;
	hook(Utility::find_virtual(I::StudioRender, 27u).get(), (uintptr_t)hkSetColorModulation, original);
	SetColorModulation = (decltype(SetColorModulation))(original);
}


