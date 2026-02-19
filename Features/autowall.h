#pragma once
namespace Features::Autowall {
	struct FireBulletData
	{
		Vector src;
		trace_t enter_trace;
		Vector direction;
		void* filter[2];
		float trace_length;
		float trace_length_remaining;
		float current_damage;
		int penetrate_count;
	};

	void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, C_BasePlayer* ignore, trace_t* ptr);
	bool CanPenetratePoint(C_BasePlayer* pLocal, C_BasePlayer* target_player, QAngle shootAngles, float& damage);
}

