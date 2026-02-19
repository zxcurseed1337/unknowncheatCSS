#include "pch.h"
#include "HookCalcViewModel.h"

void __fastcall hkCalcPlayerViewmodelView(C_BasePlayer* pThis, Vector& origin, Vector& angles);
decltype(hkCalcPlayerViewmodelView)* CalcPlayerViewmodelView;

void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

void Hooks::hook_CalcViewModel() {
	//48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 49 8B F0 48 8B EA 48 8B F9 33 DB 90
	uintptr_t original;
	hook(Utility::find_bytes(Utility::modules["client.dll"], "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 49 8B F0 48 8B EA 48 8B F9 33 DB 90").get(), (uintptr_t)hkCalcPlayerViewmodelView, original); //CViewRender::RenderView string
	CalcPlayerViewmodelView = (decltype(CalcPlayerViewmodelView))original;
}
void __fastcall hkCalcPlayerViewmodelView(C_BasePlayer* pThis, Vector& origin, Vector& angles) {
	angles += Vector(Vars::Visual::vm_pitch_offset, Vars::Visual::vm_yaw_offset, Vars::Visual::vm_roll_offset);
	Vector forward, right, up;
	AngleVectors(angles, &forward, &right, &up);
	Vector offset = forward * Vars::Visual::vm_x_offset + right * Vars::Visual::vm_y_offset + up * Vars::Visual::vm_z_offset;
	origin += offset;

	CalcPlayerViewmodelView(pThis, origin, angles);
}