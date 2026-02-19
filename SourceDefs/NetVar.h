#pragma once
namespace NetVar {
	int Get(const char* const szClass, const char* const szNetVar);
	int GetOffset(RecvTable* pTable, const char* const szNetVar);
};
inline Vector get_vec_mins(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBaseEntity", "m_vecMins");
	return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(ent) + offset);
}

inline Vector get_vec_maxs(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBaseEntity", "m_vecMaxs");
	return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(ent) + offset);
}

inline QAngle& get_PunchAngle(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBasePlayer", "m_vecPunchAngle");

	return *reinterpret_cast<QAngle*>(reinterpret_cast<uintptr_t>(ent) + offset);
}

inline int& get_Flags(C_BasePlayer* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBasePlayer", "m_fFlags");

	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ent) + offset);
}
inline int& get_TickBase(C_BasePlayer* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBasePlayer", "m_nTickBase");

	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ent) + offset);
}
inline int& get_MoveType(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBaseEntity", "m_MoveType");

	return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ent) + offset);
}

inline float& get_NextPrimaryAttack(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBaseCombatWeapon", "m_flNextPrimaryAttack");

	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(ent) + offset);
}

inline bool& get_deadflag(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CCSPlayer", "deadflag");

	return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(ent) + offset);
}

inline Vector& m_clrRender(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBaseEntity", "m_clrRender");

	return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(ent) + offset);
}


enum ECSClientClass
{
	CTestTraceline = 156,
	CTEWorldDecal = 157,
	CTESpriteSpray = 154,
	CTESprite = 153,
	CTESparks = 152,
	CTESmoke = 151,
	CTEShowLine = 149,
	CTEProjectedDecal = 146,
	CTEPlayerDecal = 145,
	CTEPhysicsProp = 142,
	CTEParticleSystem = 141,
	CTEMuzzleFlash = 140,
	CTELargeFunnel = 138,
	CTEKillPlayerAttachments = 137,
	CTEImpact = 136,
	CTEGlowSprite = 135,
	CTEShatterSurface = 148,
	CTEFootprintDecal = 133,
	CTEFizz = 132,
	CTEExplosion = 130,
	CTEEnergySplash = 129,
	CTEEffectDispatch = 128,
	CTEDynamicLight = 127,
	CTEDecal = 125,
	CTEClientProjectile = 124,
	CTEBubbleTrail = 123,
	CTEBubbles = 122,
	CTEBSPDecal = 121,
	CTEBreakModel = 120,
	CTEBloodStream = 119,
	CTEBloodSprite = 118,
	CTEBeamSpline = 117,
	CTEBeamRingPoint = 116,
	CTEBeamRing = 115,
	CTEBeamPoints = 114,
	CTEBeamLaser = 113,
	CTEBeamFollow = 112,
	CTEBeamEnts = 111,
	CTEBeamEntPoint = 110,
	CTEBaseBeam = 109,
	CTEArmorRicochet = 108,
	CTEMetalSparks = 139,
	CSteamJet = 104,
	CSmokeStack = 99,
	DustTrail = 188,
	CFireTrail = 47,
	SporeTrail = 195,
	SporeExplosion = 194,
	RocketTrail = 192,
	SmokeTrail = 193,
	CPropVehicleDriveable = 90,
	ParticleSmokeGrenade = 191,
	CParticleFire = 73,
	MovieExplosion = 189,
	CTEGaussExplosion = 134,
	CEnvQuadraticBeam = 41,
	CEmbers = 34,
	CEnvWind = 45,
	CPrecipitation = 87,
	CBaseTempEntity = 17,
	CWeaponXM1014 = 186,
	CWeaponUSP = 185,
	CWeaponUMP45 = 184,
	CWeaponTMP = 183,
	CSmokeGrenade = 98,
	CWeaponSG552 = 182,
	CWeaponSG550 = 181,
	CWeaponScout = 180,
	CWeaponP90 = 179,
	CWeaponP228 = 178,
	CWeaponMP5Navy = 177,
	CWeaponMAC10 = 176,
	CWeaponM4A1 = 175,
	CWeaponM3 = 174,
	CWeaponM249 = 173,
	CKnife = 70,
	CHEGrenade = 65,
	CWeaponGlock = 172,
	CWeaponGalil = 171,
	CWeaponG3SG1 = 170,
	CFlashbang = 49,
	CWeaponFiveSeven = 169,
	CWeaponFamas = 168,
	CWeaponElite = 167,
	CDEagle = 31,
	CWeaponCSBaseGun = 165,
	CWeaponCSBase = 164,
	CC4 = 23,
	CBaseCSGrenade = 6,
	CWeaponAWP = 163,
	CWeaponAug = 162,
	CAK47 = 1,
	NextBotCombatCharacter = 190,
	CFootstepControl = 51,
	CCSGameRulesProxy = 26,
	CWeaponCubemap = 0,
	CWeaponCycler = 166,
	CTEPlantBomb = 143,
	CTEFireBullets = 131,
	CTERadioIcon = 147,
	CPlantedC4 = 81,
	CCSTeam = 30,
	CCSPlayerResource = 28,
	CCSPlayer = 27,
	CCSRagdoll = 29,
	CTEPlayerAnimEvent = 144,
	CHostage = 66,
	CBaseCSGrenadeProjectile = 7,
	CHandleTest = 64,
	CTeamplayRoundBasedRulesProxy = 107,
	CSpriteTrail = 103,
	CSpriteOriented = 102,
	CSprite = 101,
	CRagdollPropAttached = 93,
	CRagdollProp = 92,
	CPredictedViewModel = 88,
	CPoseController = 86,
	CGameRulesProxy = 63,
	CInfoLadderDismount = 67,
	CFuncLadder = 56,
	CEnvDetailController = 38,
	CWorld = 187,
	CWaterLODControl = 161,
	CWaterBullet = 160,
	CVoteController = 159,
	CVGuiScreen = 158,
	CPropJeep = 89,
	CPropVehicleChoreoGeneric = 0,
	CTest_ProxyToggle_Networkable = 155,
	CTesla = 150,
	CTeamTrainWatcher = 0,
	CBaseTeamObjectiveResource = 16,
	CTeam = 106,
	CSun = 105,
	CParticlePerformanceMonitor = 74,
	CSpotlightEnd = 100,
	CSlideshowDisplay = 97,
	CShadowControl = 96,
	//CSceneEntity = 95,
	CRopeKeyframe = 94,
	CRagdollManager = 91,
	CPhysicsPropMultiplayer = 79,
	CPhysBoxMultiplayer = 77,
	CBasePropDoor = 15,
	CDynamicProp = 33,
	CPointCommentaryNode = 85,
	CPointCamera = 84,
	CPlayerResource = 83,
	CPlasma = 82,
	CPhysMagnet = 80,
	CPhysicsProp = 78,
	CPhysBox = 76,
	CParticleSystem = 75,
	CMaterialModifyControl = 72,
	CLightGlow = 71,
	CInfoOverlayAccessor = 69,
	CFuncTrackTrain = 62,
	CFuncSmokeVolume = 61,
	CFuncRotating = 60,
	CFuncReflectiveGlass = 59,
	CFuncOccluder = 58,
	CFuncMonitor = 57,
	CFunc_LOD = 53,
	CTEDust = 126,
	CFunc_Dust = 52,
	CFuncConveyor = 55,
	CBreakableSurface = 22,
	CFuncAreaPortalWindow = 54,
	CFish = 48,
	CEntityFlame = 36,
	CFireSmoke = 46,
	CEnvTonemapController = 44,
	CEnvScreenEffect = 42,
	CEnvScreenOverlay = 43,
	CEnvProjectedTexture = 40,
	CEnvParticleScript = 39,
	//CFogController = 50,
	CEntityParticleTrail = 37,
	CEntityDissolve = 35,
	CDynamicLight = 32,
	CColorCorrectionVolume = 25,
	CColorCorrection = 24,
	CBreakableProp = 21,
	//CBasePlayer = 13,
	//CBaseFlex = 10,
	//CBaseEntity = 9,
	CBaseDoor = 8,
	//CBaseCombatCharacter = 4,
	//CBaseAnimatingOverlay = 3,
	CBoneFollower = 20,
	//CBaseAnimating = 2,
	CInfoLightingRelative = 68,
	CAI_BaseNPC = 0,
	CBeam = 19,
	//CBaseViewModel = 18,
	CBaseProjectile = 14,
	CBaseParticleEntity = 12,
	CBaseGrenade = 11,
	//CBaseCombatWeapon = 5
};
