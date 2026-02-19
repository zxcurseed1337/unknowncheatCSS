#pragma once

namespace Vars
{
	namespace Funny {
		inline int fakelag = 0;
		inline int speedhack_factor = 1.f;
	}

	namespace Aimbot {
		inline bool  smac_rage = false;
		inline float max_fov = 0.f;
		inline int mode = 0;
		inline bool hitbox[19] = { false, false, false, false, false, false, false, false, false, false, false, false,  true, false, false, false, false, false, false };
		inline bool autoshoot = false; // need vischeck for that
		inline bool perfect_accuracy = true;
		inline float smooth = 5.f;
		inline float awall_min_dmg = 100.f;
	}
	namespace ESP
	{
		inline bool Enabled = true;

		namespace Players
		{
			inline bool Enabled = true;
			inline bool IgnoreTeam = true;
			inline bool Box = true;
			inline bool HealthBar = true;
			inline bool Name = true;
		}
	}

	namespace Misc
	{
		inline bool Bunnyhop = true;
		inline int  MaxPerfectHops = 0;
		inline float BhopResetTime = 0.f;

		inline bool BanMe = true;
		inline bool NoRecoil = true;
		inline bool Imposter = false;
	}

	namespace Visual
	{
		inline bool RemoveVisualRecoil = false;
		inline float BonusFOV = 30.f;
		inline float vm_x_offset = 0.f, vm_y_offset = 0.f, vm_z_offset = 0.f;
		inline float vm_pitch_offset = 0.f, vm_yaw_offset = 0.f, vm_roll_offset = 0.f;
		inline float view_x_offset = 0.f, view_y_offset = 0.f, view_z_offset = 0.f;
		inline float view_pitch_offset = 0.f, view_yaw_offset = 0.f, view_roll_offset = 0.f;

		inline float aspect_ratio = 16.f / 9.f;

		inline float additional_viewmodel_fov = 0.f;

		inline float modluation_r = 1.f, modluation_g = 1.f, modluation_b = 1.f;
	}
}