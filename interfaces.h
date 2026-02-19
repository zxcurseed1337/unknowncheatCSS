#pragma once
#include "pch.h"
#include "SourceDefs/CClientState.h"
#include "SourceDefs/CInput.h"
#include "SourceDefs/CPrediction.h"
namespace I {
	inline IBaseClientDLL*	  BaseClientDLL	   = nullptr;
	inline ICvar*			  Cvar			   = nullptr;
	inline IVRenderView*	  RenderView	   = nullptr;
	inline IVModelRender*	  ModelRender      = nullptr;
	inline IVEngineClient*	  EngineClient     = nullptr;
	inline IStudioRender*	  StudioRender     = nullptr;
	inline CPrediction*	      ClientPrediction = nullptr;
	inline IMoveHelper*		  MoveHelper       = nullptr;
	inline IMatSystemSurface* MatSystemSurface = nullptr;
	inline IMaterialSystem*   MaterialSystem   = nullptr;
	inline IGameMovement*	  GameMovement	   = nullptr;
	inline IEngineVGui*		  EngineVGui	   = nullptr;
	inline IEngineTrace*	  EngineTrace	   = nullptr;
	inline IClientEntityList* ClientEntityList = nullptr;
	inline CGlobalVarsBase*	  GlobalVars	   = nullptr;
	inline CClientState*	  ClientState      = nullptr;
	inline IInput*			  Input			   = nullptr;
	inline IVModelInfo*		  ModelInfo        = nullptr;
	inline IPhysicsSurfaceProps* PhysProps     = nullptr;
}

namespace Fn {
	inline __int64(__fastcall* SetPredictionRandomSeed)(CUserCmd*); // 48 85 C9 75 0B C7 05 ? ? ? ? FF FF FF FF C3 8B 41 38 89 05 ? ? ? ? C3
	inline bool(__fastcall* UsingStandardWeaponsInVehicle)(C_BasePlayer*); //  48 89 5C 24 08 57 48 83 EC 20 8B 91 0C 16 00 00 48 8B F9 85 D2 74 6E B8 FF 1F 00 00 83 FA FF 74 03 0F B7 C2 4C 8B 05
	inline void(__fastcall* UpdateButtonState)(C_BasePlayer*);
	inline C_BasePlayer** PredictionPlayer;
	inline void(__fastcall* UTIL_ClipTraceToPlayers)(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);//48 8B C4 48 89 70 10 48 89 78 18 55 41 54 41 55 41 56 41 57 48 8D 68 B8 48 81 EC 20 01 00 00 F3 0F 10 19 4D 8B F1 F3 0F 10 0A 45 8B F8 F3 0F 10 61 04 F3 0F 5C CB F3 0F 10 52 04 4C 8B E2 F3 0F 10 69 08 F3 0F 5C D4 F3 0F 10 42 08
	inline void(__fastcall* GetBulletTypeParameters)(int, float&, float&);
	inline void(__fastcall* GetCsWpnData)(void*, C_BaseCombatWeapon*, char*);

	inline uintptr_t CStaticProp__DrawModel__IStudioRender_SetColorModulation_call_return_adr1_file_offset = 0x2039C8; // CStaticProp::DrawModel string, first virtual call  | IStudioRender idx 27 (216)
	inline uintptr_t CStaticProp__DrawModel__IStudioRender_SetColorModulation_call_return_adr2_file_offset = 0x2035D3; // CStaticProp::DrawModel string, first virtual call  | IStudioRender idx 27
}
class Filter : public CTraceFilter {
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	bool ShouldHitEntity(IHandleEntity* pEntity, int contentsMask) {
		return pEntity != pSkip;
	}
public:
	IHandleEntity* pSkip;
};