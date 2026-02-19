#pragma once
namespace Features::Aimbot {
	bool perform(C_BasePlayer* player, CUserCmd* cmd);
	enum LAC_Stage {
		NONE,
		PRE_FIRE,
		FIRE,
		POST_FIRE
	};
	inline LAC_Stage next_LAC_stage = LAC_Stage::NONE;
	void AimStep(CUserCmd* cmd);
	inline QAngle target;
}

