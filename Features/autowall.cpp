#include "pch.h"
//#include "autowall.h"
//
//#define	HITGROUP_GENERIC	0
//#define	HITGROUP_HEAD		1
//#define	HITGROUP_CHEST		2
//#define	HITGROUP_STOMACH	3
//#define HITGROUP_LEFTARM	4	
//#define HITGROUP_RIGHTARM	5
//#define HITGROUP_LEFTLEG	6
//#define HITGROUP_RIGHTLEG	7
//#define HITGROUP_GEAR		10	
//
////Yes i love copy pasting from ida (55 8B EC 8B 45 08 83 C0 BD)
//void GetMaterialParameters(int iMaterial, float& flPenetrationModifier, float& flDamageModifier)
//{
//	switch (iMaterial)
//	{
//	case 67:
//		flPenetrationModifier = 0.40000001;
//		flDamageModifier = 0.25;
//		break;
//	case 68:
//	case 77:
//		flPenetrationModifier = 0.5;
//		flDamageModifier = 0.30000001;
//		break;
//	case 71:
//		flPenetrationModifier = 1.0;
//		flDamageModifier = 0.99000001;
//		break;
//	case 80:
//		flPenetrationModifier = 0.40000001;
//		flDamageModifier = 0.44999999;
//		break;
//	case 84:
//		flPenetrationModifier = 0.64999998;
//		flDamageModifier = 0.30000001;
//		break;
//	case 86:
//		flPenetrationModifier = 0.5;
//		flDamageModifier = 0.44999999;
//		break;
//	case 87:
//		flPenetrationModifier = 1.0;
//		flDamageModifier = 0.60000002;
//		break;
//	default:
//		flPenetrationModifier = 1.0;
//		flDamageModifier = 0.5;
//		break;
//	}
//}
//
//bool TraceToExit(Vector& start, Vector& dir, Vector& end, float flStepSize, float flMaxDistance)
//{
//	float flDistance = 0;
//	Vector last = start;
//
//	while (flDistance <= flMaxDistance)
//	{
//		flDistance += flStepSize;
//
//		end = start + (dir * flDistance);
//
//		if ((I::EngineTrace->GetPointContents(end) & MASK_SOLID) == 0)
//		{
//			// found first free point
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool IsArmored(int hitgroup, int armor_value, bool has_helmet)
//{
//	bool bApplyArmor = false;
//
//	if (armor_value > 0)
//	{
//		switch (hitgroup)
//		{
//		case HITGROUP_GENERIC:
//		case HITGROUP_CHEST:
//		case HITGROUP_STOMACH:
//		case HITGROUP_LEFTARM:
//		case HITGROUP_RIGHTARM:
//			bApplyArmor = true;
//			break;
//		case HITGROUP_HEAD:
//			if (has_helmet)
//			{
//				bApplyArmor = true;
//			}
//			break;
//		default:
//			break;
//		}
//	}
//
//	return bApplyArmor;
//}
//
//float ScaleDamage(float damage, int hitgroup, int armor_value, bool has_helmet, float weapon_armor_ratio)
//{
//	auto new_dmg = damage;
//	switch (hitgroup)
//	{
//	case HITGROUP_HEAD:
//		new_dmg *= 4.f;
//		break;
//	case HITGROUP_STOMACH:
//		new_dmg *= 1.25f;
//		break;
//	case HITGROUP_LEFTLEG:
//	case HITGROUP_RIGHTLEG:
//		new_dmg *= 0.75f;
//		break;
//	default:
//		break;
//	}
//
//	auto armor_bonus = 0.5f;
//	auto armor_ratio = weapon_armor_ratio * 0.5f;
//	if (armor_value && IsArmored(hitgroup, armor_value, has_helmet))
//	{
//		auto flNew = new_dmg * armor_ratio;
//		auto flArmor = (new_dmg - flNew) * armor_bonus;
//
//		if (flArmor > armor_value)
//		{
//			flArmor = armor_value;
//			flArmor *= (1 / armor_bonus);
//			flNew = new_dmg - flArmor;
//		}
//
//		new_dmg = flNew;
//	}
//
//	return new_dmg;
//}
//
//#define MAX_WEAPON_STRING	80
//#define MAX_WEAPON_PREFIX	16
//#define MAX_WEAPON_AMMO_NAME		32
//
//#define _BYTE char
//#define _DWORD uint32_t
//class CCSWeaponInfo
//{
//public:
//	void* vtbl;
//	bool bParsedScript; //0x0004
//	bool bLoadedHudElements; //0x0005
//	char szClassName[MAX_WEAPON_STRING]; //0x0006
//	char szPrintName[MAX_WEAPON_STRING]; //0x0056
//	char szViewModel[MAX_WEAPON_STRING]; //0x00A6
//	char szWorldModel[MAX_WEAPON_STRING]; //0x00F6
//	char szAnimationPrefix[MAX_WEAPON_PREFIX]; //0x0146
//	char pad_0156[2]; //0x0156
//	int32_t iSlot; //0x0158
//	int32_t iPosition; //0x015C
//	int32_t iMaxClip1; //0x0160
//	int32_t iMaxClip2; //0x0164
//	int32_t iDefaultClip1; //0x0168
//	int32_t iDefaultClip2; //0x016C
//	int32_t iWeight; //0x0170
//	int32_t iRumbleEffect; //0x0174
//	bool bAutoSwitchTo; //0x0178
//	bool bAutoSwitchFrom; //0x0179
//	char pad_017A[2]; //0x017A
//	int32_t iFlags; //0x017C
//	char szAmmo1[MAX_WEAPON_AMMO_NAME]; //0x0180
//	char szAmmo2[MAX_WEAPON_AMMO_NAME]; //0x01A0
//	char aShootSounds[16][MAX_WEAPON_STRING]; //0x01C0
//	int32_t iAmmoType; //0x06C0
//	int32_t iAmmo2Type; //0x06C4
//	bool m_bMeleeWeapon; //0x06C8
//	bool m_bBuiltRightHanded; //0x06C9
//	bool m_bAllowFlipping; //0x06CA
//	char pad_06CB[1]; //0x06CB
//	int32_t iSpriteCount; //0x06CC
//	class CHudTexture* iconActive; //0x06D0
//	class CHudTexture* iconInactive; //0x06D4
//	class CHudTexture* iconAmmo; //0x06D8
//	class CHudTexture* iconAmmo2; //0x06DC
//	class CHudTexture* iconCrosshair; //0x06E0
//	class CHudTexture* iconAutoaim; //0x06E4
//	class CHudTexture* iconZoomedCrosshair; //0x06E8
//	class CHudTexture* iconZoomedAutoaim; //0x06EC
//	class CHudTexture* iconSmall; //0x06F0
//	bool bShowUsageHint; //0x06F4
//	char pad_06F5[3]; //0x06F5
//	float m_flMaxSpeed; //0x06F8
//	int32_t m_WeaponType; //0x06FC
//	bool bFullAuto; //0x0700
//	char pad_0701[3]; //0x0701
//	int32_t m_iTeam; //0x0704
//	float m_flBotAudibleRange; //0x0708
//	float m_flArmorRatio; //0x070C
//	int32_t m_iCrosshairMinDistance; //0x0710
//	int32_t m_iCrosshairDeltaDistance; //0x0714
//	bool m_bCanUseWithShield; //0x0718
//	char m_WrongTeamMsg[32]; //0x0719
//	char m_szAnimExtension[16]; //0x0739
//	char m_szShieldViewModel[64]; //0x0749
//	char m_szAddonModel[MAX_WEAPON_STRING]; //0x0789
//	char m_szDroppedModel[MAX_WEAPON_STRING]; //0x07D9
//	char m_szSilencerModel[MAX_WEAPON_STRING]; //0x0829
//	char pad_0879[3]; //0x0879
//	int32_t m_iMuzzleFlashStyle; //0x087C
//	float m_flMuzzleScale; //0x0880
//	int32_t m_iPenetration; //0x0884
//	int32_t m_iDamage; //0x0888
//	float m_flRange; //0x088C
//	float m_flRangeModifier; //0x0890
//	int32_t m_iBullets; //0x0894
//	float m_flCycleTime; //0x0898
//	bool m_bAccuracyQuadratic; //0x089C
//	char pad_089D[3]; //0x089D
//	float m_flAccuracyDivisor; //0x08A0
//	float m_flAccuracyOffset; //0x08A4
//	float m_flMaxInaccuracy; //0x08A8
//	float m_flSpread; //0x08AC
//	float m_flSpreadAlt; //0x08B0
//	float m_flInaccuracyCrouch; //0x08B4
//	float m_flInaccuracyCrouchAlt; //0x08B8
//	float m_flInaccuracyStand; //0x08BC
//	float m_flInaccuracyStandAlt; //0x08C0
//	float m_flInaccuracyJump; //0x08C4
//	float m_flInaccuracyJumpAlt; //0x08C8
//	float m_flInaccuracyLand; //0x08CC
//	float m_flInaccuracyLandAlt; //0x08D0
//	float m_flInaccuracyLadder; //0x08D4
//	float m_flInaccuracyLadderAlt; //0x08D8
//	float m_flInaccuracyFire; //0x08DC
//	float m_flInaccuracyFireAlt; //0x08E0
//	float m_flInaccuracyMove; //0x08E4
//	float m_flInaccuracyMoveAlt; //0x08E8
//	float m_flRecoveryTimeStand; //0x08EC
//	float m_flRecoveryTimeCrouch; //0x08F0
//	float m_flInaccuracyReload; //0x08F4
//	float m_flInaccuracyAltSwitch; //0x08F8
//	float m_flTimeToIdleAfterFire; //0x08FC
//	float m_flIdleInterval; //0x0900
//	int32_t m_iWeaponPrice; //0x0904
//	int32_t m_iDefaultPrice; //0x0908
//	int32_t m_iPreviousPrice; //0x090C
//}; //S
//
//typedef enum
//{
//	Primary_Mode = 0,
//	Secondary_Mode,
//} SDKWeaponMode;
//
//
//
//void Features::Autowall::TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, C_BasePlayer* ignore, trace_t* ptr)
//{
//	Ray_t ray;
//	ray.Init(vecAbsStart, vecAbsEnd);
//
//	Filter f;
//	f.pSkip = ignore;
//	trace_t trace{};
//	//I::EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_HITBOX, &f, &trace);
//	I::EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &f, &trace);
//}
//
//
//using namespace Features::Autowall;
//bool Features::Autowall::CanPenetratePoint(C_BasePlayer* pLocal, C_BasePlayer* target_player, QAngle shootAngles, float& damage)
//{
//	auto weapon = (C_WeaponCSBase*)pLocal->GetActiveWeapon();
//	if (!weapon)
//		return false;
//
//	CCSWeaponInfo weapon_data;
//	Fn::GetCsWpnData(&weapon_data, weapon, (char*)"");
//
//	auto iWeaponID = weapon->GetWeaponID();
//	auto iDamage = weapon_data.m_iDamage;
//	auto iBulletType = weapon_data.iAmmoType;
//	auto flDistance = weapon_data.m_flRange;
//	auto flRangeModifier = weapon_data.m_flRangeModifier;
//	auto iPenetration = weapon_data.m_iPenetration;
//
//	float fCurrentDamage = iDamage;   // damage of the bullet at it's current trajectory
//	float flCurrentDistance = 0.0;  //distance that the bullet has traveled so far
//
//	Vector vecDir, vecRight, vecUp;
//	AngleVectors(shootAngles, &vecDir, &vecRight, &vecUp);
//
//	float flPenetrationPower = 0;		// thickness of a wall that this bullet can penetrate
//	float flPenetrationDistance = 0;	// distance at which the bullet is capable of penetrating a wall
//	float flDamageModifier = 0.5;		// default modification of bullets power after they go through a wall.
//	float flPenetrationModifier = 1.f;
//
//	Fn::GetBulletTypeParameters(iBulletType, flPenetrationPower, flPenetrationDistance);
//
//	auto vecSrc = pLocal->EyePosition();
//
//	int Loop = 0;
//	while (Loop++ < 15 && fCurrentDamage > 0)
//	{
//		Vector vecEnd = vecSrc + vecDir * flDistance;
//
//		trace_t tr;
//
//		TraceLine(vecSrc, vecEnd, MASK_SHOT | CONTENTS_HITBOX, pLocal, &tr);
//		{
//			Filter f;
//			f.pSkip = pLocal;
//			Fn::UTIL_ClipTraceToPlayers(vecSrc, vecEnd + vecDir * 40.0f, MASK_SHOT | CONTENTS_HITBOX, &f, &tr);
//		}
//
//		if (tr.fraction == 1.0f)
//			break;
//
//		/************* MATERIAL DETECTION ***********/
//		surfacedata_t* pSurfaceData = I::PhysProps->GetSurfaceData(tr.surface.surfaceProps);
//		int iEnterMaterial = pSurfaceData->game.material;
//
//		GetMaterialParameters(iEnterMaterial, flPenetrationModifier, flDamageModifier);
//
//		bool hitGrate = tr.contents & CONTENTS_GRATE;
//
//		// since some railings in de_inferno are CONTENTS_GRATE but CHAR_TEX_CONCRETE, we'll trust the
//		// CONTENTS_GRATE and use a high damage modifier.
//		if (hitGrate)
//		{
//			// If we're a concrete grate (TOOLS/TOOLSINVISIBLE texture) allow more penetrating power.
//			flPenetrationModifier = 1.0f;
//			flDamageModifier = 0.99f;
//		}
//
//		//calculate the damage based on the distance the bullet travelled.
//		flCurrentDistance += tr.fraction * flDistance;
//		fCurrentDamage *= pow(flRangeModifier, (flCurrentDistance / 500));
//
//		auto player = reinterpret_cast<C_BasePlayer*>(tr.m_pEnt);
//		if (player == target_player && tr.hitgroup != HITGROUP_GENERIC && tr.hitgroup != HITGROUP_GEAR)
//		{
//			float dmg = ScaleDamage(fCurrentDamage, tr.hitgroup, 100/*player->m_ArmorValue() */ , true/*player->m_bHasHelmet()*/, weapon_data.m_flArmorRatio);
//			if (dmg > Vars::Aimbot::awall_min_dmg) {
//				damage = dmg;
//				return true;
//			}
//			return false;
//		}
//
//		// check if we reach penetration distance, no more penetrations after that
//		if (flCurrentDistance > flPenetrationDistance && iPenetration > 0)
//			iPenetration = 0;
//
//		// check if bullet can penetarte another entity
//		if (iPenetration == 0 && !hitGrate)
//			break; // no, stop
//
//		// If we hit a grate with iPenetration == 0, stop on the next thing we hit
//		if (iPenetration < 0)
//			break;
//
//		Vector penetrationEnd;
//
//		// try to penetrate object, maximum penetration is 128 inch
//		if (!TraceToExit(tr.endpos, vecDir, penetrationEnd, 24, 128))
//			break;
//
//		// find exact penetration exit
//		trace_t exitTr;
//		TraceLine(penetrationEnd, tr.endpos, MASK_SHOT | CONTENTS_HITBOX, NULL, &exitTr);
//
//		if (exitTr.m_pEnt != tr.m_pEnt && exitTr.m_pEnt != NULL)
//		{
//			// something was blocking, trace again
//			TraceLine(penetrationEnd, tr.endpos, MASK_SHOT | CONTENTS_HITBOX, (C_BasePlayer*)exitTr.m_pEnt, &exitTr);
//		}
//
//		// get material at exit point
//		pSurfaceData = I::PhysProps->GetSurfaceData(exitTr.surface.surfaceProps);
//		int iExitMaterial = pSurfaceData->game.material;
//
//		hitGrate = hitGrate && (exitTr.contents & CONTENTS_GRATE);
//#define CHAR_TEX_VENT			'V'
//#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_METAL			'M'
//		// if enter & exit point is wood or metal we assume this is 
//		// a hollow crate or barrel and give a penetration bonus
//		if (iEnterMaterial == iExitMaterial)
//		{
//			if (iExitMaterial == CHAR_TEX_WOOD ||
//				iExitMaterial == CHAR_TEX_METAL)
//			{
//				flPenetrationModifier *= 2;
//			}
//		}
//
//		float flTraceDistance = (exitTr.endpos - tr.endpos).Length();
//
//		// check if bullet has enough power to penetrate this distance for this material
//		if (flTraceDistance > (flPenetrationPower * flPenetrationModifier))
//			break; // bullet hasn't enough power to penetrate this distance
//
//		// penetration was successful
//
//		//setup new start end parameters for successive trace
//
//		flPenetrationPower -= flTraceDistance / flPenetrationModifier;
//		flCurrentDistance += flTraceDistance;
//
//		vecSrc = exitTr.endpos;
//		flDistance = (flDistance - flCurrentDistance) * 0.5;
//
//		// reduce damage power each time we hit something other than a grate
//		fCurrentDamage *= flDamageModifier;
//
//		// reduce penetration counter
//		iPenetration--;
//	}
//
//	return false;
//}