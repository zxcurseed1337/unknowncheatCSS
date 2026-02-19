#include "pch.h"
#include "aimbot.h"
#include "target_selector.h"

#include "SourceDefs/NetVar.h"



float NormalizeAngle(const float ang)
{
	if (!::isfinite(ang))
		return 0.0f;

	return ::remainderf(ang, 360.0f);
}


void ClampAngles(QAngle& v)
{
	v.x = Max(-89.0f, Min(89.0f, NormalizeAngle(v.x)));
	v.y = NormalizeAngle(v.y);
	v.z = 0.0f;
}

float Normalize(QAngle& q)
{
	float fl_lenght = q.Length();
	float fl_lenght_normal = 1.f / ((1.19209290E-07F) + fl_lenght); //FLT_EPSILON

	q.x = q.x * fl_lenght_normal;
	q.y = q.y * fl_lenght_normal;
	q.z = q.z * fl_lenght_normal;

	return fl_lenght;
}
void Features::Aimbot::AimStep(CUserCmd* cmd) {
	static auto Last_Angles{ cmd->viewangles };


	constexpr auto MAX_ANGLE_DELTA = 4.f;

	auto delta{ cmd->viewangles - Last_Angles };

	//ClampAngles(delta);

	if (Normalize(delta) >= MAX_ANGLE_DELTA)
	{
		cmd->viewangles = Last_Angles + delta * MAX_ANGLE_DELTA;

		//ClampAngles(cmd->viewangles);

		if (Features::Aimbot::next_LAC_stage != Features::Aimbot::LAC_Stage::NONE) {

			Features::Aimbot::next_LAC_stage = (Features::Aimbot::LAC_Stage::NONE);

			cmd->buttons &= ~(IN_ATTACK);
		}
	}

	Last_Angles = cmd->viewangles;
}




bool ShouldRun(C_BasePlayer* local_player, C_WeaponCSBase* weapon, CUserCmd* cmd) {
	bool ret = true;
	if(float nextattack = get_NextPrimaryAttack(weapon)) {
		ret =  nextattack < I::GlobalVars->curtime;
	}

	//if (!weapon->HasAnyAmmo() && weapon->Clip1() <= 0) {
	//	cmd->buttons |= IN_RELOAD;
	//	return false;
	//}

	switch (weapon->GetWeaponID())
	{
	case WEAPON_P228:
	case WEAPON_GLOCK:
	case WEAPON_SCOUT:
	case WEAPON_MAC10:
	case WEAPON_AUG:
	case WEAPON_FIVESEVEN:
	case WEAPON_UMP45:
	case WEAPON_SG550:
	case WEAPON_GALIL:
	case WEAPON_FAMAS:
	case WEAPON_USP:
	case WEAPON_AWP:
	case WEAPON_MP5NAVY:
	case WEAPON_M249:
	case WEAPON_M3:
	case WEAPON_M4A1:
	case WEAPON_TMP:
	case WEAPON_G3SG1:
	case WEAPON_DEAGLE:
	case WEAPON_SG552:
	case WEAPON_AK47:
	case WEAPON_P90:
		if (Vars::Aimbot::autoshoot)
			return  ret;
		return ret && (cmd->buttons & IN_ATTACK);
	default:
		return false;
		break;
	}
}


Vector wanted_angle = Vector(0, 0, 0);
QAngle Nospread(C_BasePlayer* local_player, C_WeaponCSBase* weapon, CUserCmd* cmd) {
	static const auto s_pfRandomSeed = reinterpret_cast<void(__fastcall*)(unsigned)>(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomSeed"));
	static const auto s_pfRandomFloat = reinterpret_cast<float(__fastcall*)(float, float)>(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomFloat"));



	if (Vars::Aimbot::perfect_accuracy)
	{
		Vector vForward, vRight, vUp; 
		QAngle vFinal;

		s_pfRandomSeed((cmd->random_seed & 255) + 1);

		Utility::callVirtual<382, void>(weapon);

		float spread = Utility::callVirtual<382, float>(weapon);
		float inaccuracy = Utility::callVirtual<383, float>(weapon);

		//std::println("curtime = {:07};seed = {:03} ; spread = {} ; inaccuracy = {} ; {} ; {}", I::GlobalVars->curtime, cmd->random_seed & 255, spread, inaccuracy, weapon->GetSpread(), weapon->GetInaccuracy());

		float fTheta0 = s_pfRandomFloat(0.0f, 2.0f * M_PI);
		float fRadius0 = s_pfRandomFloat(0.0f,spread);

		float fTheta1 = s_pfRandomFloat(0.0f, 2.0f * M_PI);
		float fRadius1 = s_pfRandomFloat(0.0f, inaccuracy);

		float x0 = fRadius0 * cosf(fTheta0);
		float y0 = fRadius0 * sinf(fTheta0);

		float x1 = fRadius1 * cosf(fTheta1);
		float y1 = fRadius1 * sinf(fTheta1);

		float xSpread = x0 + x1;
		float ySpread = y0 + y1;


		AngleVectors(cmd->viewangles, &vForward, &vRight, &vUp);

		Vector vSpreadAngles = vForward + (vRight * xSpread) + (vUp * ySpread);
		vSpreadAngles /= vSpreadAngles.Length();

		VectorAngles(vSpreadAngles, vFinal);
		ClampAngles(vFinal);
		QAngle vNewAngles = (cmd->viewangles + (cmd->viewangles - vFinal));
		auto offset = vNewAngles - cmd->viewangles;

		ClampAngles(vNewAngles);


		return vNewAngles;

	} 

	return QAngle(0,0,0);
}


enum AimbotMode {
	None, Straight, Silent, Psilent, LILAc
};

bool Features::Aimbot::perform(C_BasePlayer* local_player, CUserCmd* cmd) {
	if (Vars::Aimbot::mode == AimbotMode::None)
		return true;
	QAngle out;

	C_WeaponCSBase* wep = (C_WeaponCSBase*)local_player->GetActiveWeapon();
	if (!wep)
		return true;

	//bool skipaim = false;
	if (Vars::Aimbot::mode == LILAc)
		if (next_LAC_stage != LAC_Stage::NONE)
			return true;


	if (!ShouldRun(local_player, wep, cmd) || I::ClientState->chokedcommands == 15) {
		return true;
	}


	if (Features::TargetSelector::best_aim_angle(out, local_player) ) {


		cmd->viewangles = out;
		target = Nospread(local_player, wep, cmd) - get_PunchAngle(local_player) * 2.f;;
		cmd->viewangles = target;
		ClampAngles(cmd->viewangles);
		if (Vars::Aimbot::autoshoot) {
			cmd->buttons = cmd->buttons | IN_ATTACK;
		}
		if (Vars::Aimbot::mode == Psilent)
			return false;


	}
	if (Vars::Aimbot::mode == LILAc)
		if (cmd->buttons & IN_ATTACK) {
			next_LAC_stage = LAC_Stage::PRE_FIRE;
		}

	if (Vars::Aimbot::mode == Straight) {
		I::EngineClient->SetViewAngles(cmd->viewangles);
	}

	return true;
}

/*


	}*/