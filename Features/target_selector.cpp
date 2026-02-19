#include "pch.h"
#include "target_selector.h"

typedef void* FileNameHandle_t;
#define	MAX_QPATH		96	
enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

struct worldbrushdata_t;

struct brushdata_t
{
	void* pShared;
	int			firstmodelsurface, nummodelsurfaces;

	unsigned short	renderHandle;
	unsigned short	firstnode;
};

struct spritedata_t
{
	int				numframes;
	int				width;
	int				height;
	CEngineSprite* sprite;
};


struct model_t
{
	FileNameHandle_t	fnHandle;
	char				szName[MAX_QPATH];

	int					nLoadFlags;		// mark loaded/not loaded
	int					nServerCount;	// marked at load

	modtype_t			type;
	int					flags;			// MODELFLAG_???

	// volume occupied by the model graphics	
	Vector				mins, maxs;
	float				radius;

	union
	{
		brushdata_t		brush;
		MDLHandle_t		studio;
		spritedata_t	sprite;
	};
};
#include "autowall.h"

bool get_hitbox_pos(C_BaseEntity* entity, int hitbox, Vector& out) {
	const model_t* model = entity->GetModel();
	if (!model)
		return false;

	const auto hdr = I::ModelInfo->GetStudiomodel(model);
	if (!hdr)
		return false;

	const auto hitbox_set = hdr->pHitboxSet(entity->GetBaseAnimating()->m_nHitboxSet);
	mstudiobbox_t* untransformedBox = hitbox_set->pHitbox(hitbox);

	Vector bbmin = untransformedBox->bbmin;
	Vector bbmax = untransformedBox->bbmax;

	if (!untransformedBox)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!entity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::GlobalVars->curtime))
		return false;

	Vector vMin, vMax, vCenter, sCenter;
	VectorTransform(bbmin, matrix[untransformedBox->bone], vMin);
	VectorTransform(bbmax, matrix[untransformedBox->bone], vMax);

	vCenter = ((vMin + vMax) * 0.5f);

	out = vCenter;

	return true;
}

namespace TargetSelectorState {
	float best = FLT_MAX;;
}

#define M_RADPI		57.295779513082f
QAngle GetAngleToPosition(const Vector vFrom, const Vector vTo)
{
	const Vector vDelta = (vFrom - vTo);
	const float flHyp = ::sqrtf((vDelta.x * vDelta.x) + (vDelta.y * vDelta.y));

	return { (::atanf(vDelta.z / flHyp) * M_RADPI), (::atanf(vDelta.y / vDelta.x) * M_RADPI) + (180.0f * (vDelta.x >= 0.0f)), 0.0f };
}



float GetFovBetween(const QAngle one, const QAngle two, Vector& dir_one, Vector& dir_two)
{
	AngleVectors(one, &dir_one);
	AngleVectors(two, &dir_two);

	dir_one /= dir_one.Length();
	dir_two /= dir_two.Length();


	float result = dir_one.Dot(dir_two);

	if (!isfinite(result) || isinf(result) || isnan(result))
		result = FLT_MAX;

	return result;
}

int one = 1;


#include "SourceDefs/NetVar.h"
inline Vector& get_Origin(C_BasePlayer* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBaseEntity", "m_vecOrigin");

	return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(ent) + offset);
}
inline Vector& get_ViewOffset(C_BaseEntity* ent) {
	static const ptrdiff_t offset = NetVar::Get("CBasePlayer", "m_vecViewOffset");

	return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(ent) + offset);
}

bool CanSeePoint(C_BasePlayer* local_player, C_BasePlayer* target, Vector point) {
	Ray_t ray;
	ray.Init(local_player->EyePosition(), point);

	Filter f;
	f.pSkip = local_player;
	trace_t trace{};
	//I::EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_HITBOX, &f, &trace);
	one = 1;
	I::EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &f, &trace);

	return (!trace.allsolid && !trace.startsolid) && (trace.fraction > 0.97f || (C_BasePlayer*)trace.m_pEnt == target);
}

bool Features::TargetSelector::best_aim_angle(QAngle& out, C_BasePlayer* local_player) {
	QAngle viewangles;
	I::EngineClient->GetViewAngles(viewangles);
	TargetSelectorState::best = FLT_MAX;

	bool ret = false;
	for (int n = 1; n < I::ClientEntityList->GetMaxEntities(); n++)
	{

		IClientEntity* entity = I::ClientEntityList->GetClientEntity(n);
		if (!entity || entity->IsDormant() || entity == local_player)
			continue;

		ClientClass* clientclass = entity->GetClientClass();
		if (!clientclass)
			continue;
		if (clientclass->m_ClassID != 27) // CCSPlayer
			continue;

		C_BasePlayer* player_entity = (C_BasePlayer*)entity;
		if (!player_entity->IsAlive() || player_entity->InLocalTeam())
			continue;

		for (int i = 0; i < 19; i++) {
			if (Vars::Aimbot::hitbox[i]) {
				Vector hitbox;
				bool should = get_hitbox_pos(player_entity, i, hitbox);
				if (!should)
					continue;

				Vector pos = local_player->EyePosition();
				QAngle angle_towards_player = GetAngleToPosition(pos, hitbox);

				Vector angle1, angle2;
				float angle_between = GetFovBetween(viewangles, angle_towards_player, angle1, angle2);
				if (!CanSeePoint(local_player, player_entity, hitbox))
					continue;

				if (abs(angle_between - 1) < abs(TargetSelectorState::best - 1) and angle_between > cosf(DEG2RAD(Vars::Aimbot::max_fov))) {
					TargetSelectorState::best = angle_between;
					out = angle_towards_player;
					ret = true;
				}
			}
		}
	}
	return ret;
}