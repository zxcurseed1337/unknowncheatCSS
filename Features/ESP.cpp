#include "pch.h"
#include "ESP.h"

#include "State/CheatState.h"

#include "SourceDefs/NetVar.h"

bool GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h);
inline void MakeSafeName(const char* oldName, char* newName, int newNameBufSize);
inline const char* SafeName(const char* oldName);

bool get_hitbox_pos2(C_BaseEntity* entity, int hitbox, Vector& out1, Vector& out2) {
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

	out1 = vMin;
	out2 = vMax;

	return true;
}
bool WorldPosToScreenPos(const Vector vWorld, Vector2D& vScreen);
void PointsFromBox(const Vector mins, const Vector maxs, Vector* points);
void Features::ESP::draw() {
	if (!Vars::ESP::Enabled)
		return;

	
	

	auto pDrawList = ImGui::GetWindowDrawList();

	float radius = (CheatState::screensize_x / 2)  * tan(DEG2RAD(Vars::Aimbot::max_fov)) / (tan(DEG2RAD(CheatState::current_fov / 2)) + 0.0001);
	pDrawList->AddCircle(ImVec2(CheatState::screensize_x / 2, CheatState::screensize_y / 2), radius, ImColor(255, 0, 0));
	//int teamnum = pLocal->GetTeamNumber();

	int local = I::EngineClient->GetLocalPlayer();

	auto local_player_ = I::ClientEntityList->GetClientEntity(local);
	if (!local_player_)
		return;

	auto local_player = local_player_->GetBaseEntity();
	int teamnum = local_player->GetTeamNumber();
	int x, y, w, h;
	for (int n = 1; n < (I::ClientEntityList->GetMaxEntities() + 1); n++)
	{
		if (n == local)
			continue;

		IClientEntity* pEntity = I::ClientEntityList->GetClientEntity(n);

		if (!pEntity || pEntity->IsDormant())
			continue;

		ClientClass* pCC = pEntity->GetClientClass();

		if (!pCC)
			continue;

		switch (pCC->m_ClassID)
		{
		case ECSClientClass::CCSPlayer:
		{
			if (!Vars::ESP::Enabled)
				break;


			C_BaseEntity* pPlayer = pEntity->GetBaseEntity();
			bool notdead = get_deadflag(pPlayer);
			if (notdead || pPlayer->GetTeamNumber() == teamnum)
				break;

			if (!GetDynamicBounds(pPlayer, x, y, w, h))
				break;

			const int nHealth = pPlayer->GetHealth();
			const int nMaxHealth = pPlayer->GetMaxHealth();

			if (Vars::ESP::Players::Box)
			{
				pDrawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImColor(255, 0, 0), 0, 240, 1.f);
			}




			player_info_t pi;
			I::EngineClient->GetPlayerInfo(n, &pi);

			//for (int i = 0; i < 19; i++) {
			//	Vector pos, pos2;



			//	
			//	get_hitbox_pos2(pPlayer, i, pos, pos2);

			//	Vector v[8];
			//	PointsFromBox(pos, pos2, v);
			//	Vector2D previous;
			//	for (auto& p : v) {
			//		Vector2D pos1;
			//		WorldPosToScreenPos(p, pos1);

			//		pDrawList->AddRectFilled(ImVec2(pos1.x - 1, pos1.y - 1), ImVec2(pos1.x + 1, pos1.y + 1), ImColor(255, 0, 0));
			//		pDrawList->AddLine(ImVec2(previous.x, previous.y), ImVec2(pos1.x, pos1.y), ImColor(255, 0, 0));
			//		previous = pos1;

			//	}
			//}

			//C_BaseCombatWeapon* weapon = pPlayer->GetActiveWeapon();
			//if (!weapon)
			//	continue;

			//auto wpn_class = weapon->GetClientClass();
			//if (!wpn_class)
			//	continue;

			std::string buffer;
			std::format_to
			(
				std::back_inserter(buffer), // < OutputIt
				"{}/{}\n{}\n",          // < fmt
				nHealth, nMaxHealth, pi.name
			);
			ImVec2 box_size = ImGui::CalcTextSize(buffer.c_str(), buffer.c_str() + buffer.size());

			pDrawList->AddRectFilled(ImVec2(x + w + 1, y), ImVec2(x + w + 1 + box_size.x, y + box_size.y), ImColor(0, 0, 0, 200));
			pDrawList->AddText(ImVec2(x + w + 2, y), ImColor(255, 255, 255), buffer.c_str(), buffer.c_str() + buffer.size());
			//pDrawList->AddText(ImVec2(x+w+2, y + 11), ImColor(255, 255, 255), pi.name, pi.name + strlen(pi.name));
			//pDrawList->AddText(ImVec2(x + w + 2, y + 23), ImColor(255, 255, 255), weapon_class_name);
			break;
		}
		default: break;
		}
	}
}



inline const char* SafeName(const char* oldName)
{
	static char safeName[MAX_PLAYER_NAME_LENGTH * 2 + 1];
	MakeSafeName(oldName, safeName, sizeof(safeName));

	return safeName;
}



bool WorldPosToScreenPos(const Vector vWorld, Vector2D& vScreen) {
	const matrix3x4_t& w2s = CheatState::m_WorldToProjection.As3x4();

	const float w = (w2s[3][0] * vWorld[0] + w2s[3][1] * vWorld[1] + w2s[3][2] * vWorld[2] + w2s[3][3]);

	if (w > 0.001f)
	{
		const float flsw = static_cast<float>(CheatState::screensize_x);
		const float flsh = static_cast<float>(CheatState::screensize_y);
		const float fl1dbw = 1.0f / w;

		vScreen.x = (flsw / 2.0f) + (0.5f * ((w2s[0][0] * vWorld[0] + w2s[0][1] * vWorld[1] + w2s[0][2] * vWorld[2] + w2s[0][3]) * fl1dbw) * flsw + 0.5f);
		vScreen.y = (flsh / 2.0f) - (0.5f * ((w2s[1][0] * vWorld[0] + w2s[1][1] * vWorld[1] + w2s[1][2] * vWorld[2] + w2s[1][3]) * fl1dbw) * flsh + 0.5f);

		return true;
	}

	return false;
}
#include "SourceDefs/NetVar.h"


void PointsFromBox(const Vector mins, const Vector maxs, Vector* points)
{
	points[0][0] = mins[0];
	points[0][1] = mins[1];
	points[0][2] = mins[2];

	points[1][0] = mins[0];
	points[1][1] = mins[1];
	points[1][2] = maxs[2];

	points[2][0] = mins[0];
	points[2][1] = maxs[1];
	points[2][2] = mins[2];

	points[3][0] = mins[0];
	points[3][1] = maxs[1];
	points[3][2] = maxs[2];

	points[4][0] = maxs[0];
	points[4][1] = mins[1];
	points[4][2] = mins[2];

	points[5][0] = maxs[0];
	points[5][1] = mins[1];
	points[5][2] = maxs[2];

	points[6][0] = maxs[0];
	points[6][1] = maxs[1];
	points[6][2] = mins[2];

	points[7][0] = maxs[0];
	points[7][1] = maxs[1];
	points[7][2] = maxs[2];
}

void BuildTransformedBox(Vector* v2, const Vector bbmin, const Vector bbmax, const matrix3x4_t& m)
{
	Vector v[8];
	PointsFromBox(bbmin, bbmax, v);

	VectorTransform(v[0], m, v2[0]);
	VectorTransform(v[1], m, v2[1]);
	VectorTransform(v[2], m, v2[2]);
	VectorTransform(v[3], m, v2[3]);
	VectorTransform(v[4], m, v2[4]);
	VectorTransform(v[5], m, v2[5]);
	VectorTransform(v[6], m, v2[6]);
	VectorTransform(v[7], m, v2[7]);
}

bool GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	Vector vPoints[8];
	BuildTransformedBox(vPoints, get_vec_mins(pEntity), get_vec_maxs(pEntity), pEntity->RenderableToWorldTransform());

	Vector2D flb, brt, blb, frt, frb, brb, blt, flt;

	if (WorldPosToScreenPos(vPoints[3], flb) && WorldPosToScreenPos(vPoints[5], brt)
		&& WorldPosToScreenPos(vPoints[0], blb) && WorldPosToScreenPos(vPoints[4], frt)
		&& WorldPosToScreenPos(vPoints[2], frb) && WorldPosToScreenPos(vPoints[1], brb)
		&& WorldPosToScreenPos(vPoints[6], blt) && WorldPosToScreenPos(vPoints[7], flt)
		&& WorldPosToScreenPos(vPoints[6], blt) && WorldPosToScreenPos(vPoints[7], flt))
	{
		const Vector2D arr[8] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float righ = flb.x;
		float bottom = flb.y;

		for (int n = 1; n < 8; n++)
		{
			if (left > arr[n].x)
				left = arr[n].x;

			if (top < arr[n].y)
				top = arr[n].y;

			if (righ < arr[n].x)
				righ = arr[n].x;

			if (bottom > arr[n].y)
				bottom = arr[n].y;
		}

		x = static_cast<int>(left);
		y = static_cast<int>(bottom);
		w = static_cast<int>((righ - left));
		h = static_cast<int>((top - bottom));

		return !(x > CheatState::screensize_x || (x + w) < 0 || y > CheatState::screensize_y || (y + h) < 0);
	}

	return false;
}

inline void MakeSafeName(const char* oldName, char* newName, int newNameBufSize)
{
	assert(newNameBufSize >= sizeof(newName[0]));

	int newpos = 0;

	for (const char* p = oldName; *p != 0 && newpos < newNameBufSize - 1; p++)
	{
		//check for a '#' char at the beginning
		if (p == oldName && *p == '#')
		{
			newName[newpos] = '*';
			newpos++;
		}
		else if (*p == '%')
		{
			// remove % chars
			newName[newpos] = '*';
			newpos++;
		}
		else if (*p == '&')
		{
			//insert another & after this one
			if (newpos + 2 < newNameBufSize)
			{
				newName[newpos] = '&';
				newName[newpos + 1] = '&';
				newpos += 2;
			}
		}
		else
		{
			newName[newpos] = *p;
			newpos++;
		}
	}
	newName[newpos] = 0;
}