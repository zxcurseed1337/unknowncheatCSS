#include "pch.h"
#include "EnginePrediction.h"

#include "SourceDefs/NetVar.h"
#include "SourceDefs/checksum_md5.h"

CMoveData m_MoveData = { };
float m_fOldCurrentTime = 0.0f;
float m_fOldFrameTime = 0.0f;
int m_nOldTickCount = 0;

int GetTickbase(C_BasePlayer* pLocal, CUserCmd* cmd)
{
	static int       s_nTick = 0;
	static CUserCmd* s_pLastCmd = nullptr;

	if (cmd)
	{
		if (!s_pLastCmd || s_pLastCmd->hasbeenpredicted)
			s_nTick = get_TickBase(pLocal);
		else
			s_nTick++;

		s_pLastCmd = cmd;
	}

	return s_nTick;
}

CGlobalVarsBase* gpGlobals = I::GlobalVars; // something somewhere got inlined
/*
inline void SetCurrentCommand(CUserCmd* cmd)
	{
		static const int nOffset = (H::NetVar.Get("CBasePlayer", "m_hConstraintEntity") - 0x4);
		*reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(this) + nOffset) = cmd;
	}

	inline bool UsingStandardWeaponsInVehicle()
	{
		return Functions::C_BasePlayer_UsingStandardWeaponsInVehicle(this);
	}

	inline void UpdateButtonState(int nUserCmdButtonMask)
	{
		Functions::C_BasePlayer_UpdateButtonState(this, nUserCmdButtonMask);
	}

	inline int& m_nImpulse()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x10C4);
	}
*/

inline void SetCurrentCommand(C_BasePlayer* player, CUserCmd* cmd)
{
	static const ptrdiff_t offset = NetVar::Get("CBasePlayer", "m_hConstraintEntity") - sizeof(void*);
	*reinterpret_cast<CUserCmd**>(reinterpret_cast<uintptr_t>(player) + offset) = cmd;
}

inline int& m_nImpulse(C_BasePlayer* player)
{
	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(player) + 0x1688);
}


inline CUserCmd*& m_nCurrentCommand(C_BasePlayer* player)
{
	return *reinterpret_cast<CUserCmd**>(reinterpret_cast<uintptr_t>(player) + +0x15B8); 
}


void Features::EnginePrediction::start(CUserCmd* cmd) {
	if (!I::MoveHelper)
		return;

	auto local_player_ = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
	if (!local_player_)
		return;

	auto pLocal = dynamic_cast<C_BasePlayer*>(local_player_->GetBaseEntity());

	if (!pLocal || !pLocal->IsAlive())
		return;
	m_nCurrentCommand(pLocal) = cmd;

#undef max
	cmd->random_seed = (MD5_PseudoRandom(cmd->command_number) & std::numeric_limits<int>::max());

	
	Fn::SetPredictionRandomSeed(cmd);
	*Fn::PredictionPlayer = pLocal;
	//std::println("{:X} | {:X}", Utility::modules["client.dll"].base  + 0x5F0B68, (uintptr_t)Fn::PredictionPlayer);
	m_fOldCurrentTime = I::GlobalVars->curtime;
	m_fOldFrameTime = I::GlobalVars->frametime;
	m_nOldTickCount = I::GlobalVars->tickcount;

	const int nOldFlags = get_Flags(pLocal);
	const int nOldTickBase = get_TickBase(pLocal);
	const bool bOldIsFirstPrediction = I::ClientPrediction->m_bFirstTimePredicted;
	const bool bOldInPrediction = I::ClientPrediction->m_bInPrediction;

	const int nServerTicks = GetTickbase(pLocal, cmd);
	static CGlobalVarsBase* gpGlobals = I::GlobalVars; // something somewhere got inlined
	I::GlobalVars->curtime = TICKS_TO_TIME(nServerTicks);
	I::GlobalVars->frametime = (I::ClientPrediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL);
	I::GlobalVars->tickcount = nServerTicks;

	I::ClientPrediction->m_bFirstTimePredicted = false;
	I::ClientPrediction->m_bInPrediction = true;

	#undef CBasePlayer
	//I::GameMovement->StartTrackPredictionErrors((CBasePlayer*)pLocal);
	//if (cmd->weaponselect != 0)
	//{

	//	auto weap_baseent = I::ClientEntityList->GetClientEntity(cmd->weaponselect);
	//	if (!weap_baseent)
	//		return;
	//	C_BaseCombatWeapon* pWeapon = dynamic_cast<C_BaseCombatWeapon*>(weap_baseent);

	//	if (pWeapon)
	//		pLocal->SelectItem(pWeapon->GetName(), cmd->weaponsubtype);
	//}

	//if (cmd->impulse)
	//{
	//	if (Fn::UsingStandardWeaponsInVehicle(pLocal))
	//		m_nImpulse(pLocal) = cmd->impulse;
	//}

	//Fn::UpdateButtonState(pLocal);

	I::ClientPrediction->SetLocalViewAngles(cmd->viewangles);

	I::MoveHelper->SetHost((CBasePlayer*)pLocal);


	I::ClientPrediction->SetupMove(pLocal, cmd, I::MoveHelper, &m_MoveData);
	I::GameMovement->ProcessMovement((CBasePlayer*)pLocal, &m_MoveData);
	I::ClientPrediction->FinishMove(pLocal, cmd, &m_MoveData);

	get_TickBase(pLocal) = nOldTickBase;
	get_Flags(pLocal) = nOldFlags;

	I::ClientPrediction->m_bInPrediction = bOldInPrediction;
	I::ClientPrediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
}

void Features::EnginePrediction::finish() {
	if (!I::MoveHelper)
		return;

	auto local_player_ = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
	if (!local_player_)
		return;

	auto pLocal = dynamic_cast<C_BasePlayer*>(local_player_->GetBaseEntity());

	if (!pLocal)
		return;

	I::GameMovement->FinishTrackPredictionErrors((CBasePlayer*)pLocal);

	I::GlobalVars->curtime = m_fOldCurrentTime;
	I::GlobalVars->frametime = m_fOldFrameTime;
	I::GlobalVars->tickcount = m_nOldTickCount;

	m_nCurrentCommand(pLocal) = nullptr;
	Fn::SetPredictionRandomSeed(nullptr);
	*Fn::PredictionPlayer = pLocal;
}

