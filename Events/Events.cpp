#include "pch.h"
#include "Events.h"

#include "Features/bhop.h"
#include "Features/EnginePrediction.h"
#include "Features/Aimbot.h"

#include "SourceDefs/NetVar.h"

bool CheatEvents::ModifyCmd(CUserCmd* cmd) {
	int local = I::EngineClient->GetLocalPlayer();

	auto local_player_ = I::ClientEntityList->GetClientEntity(local);
	if (!local_player_)
		return true;



	auto local_player = dynamic_cast<C_BasePlayer*>(local_player_->GetBaseEntity());
	C_WeaponCSBase* wep = (C_WeaponCSBase*)local_player->GetActiveWeapon();
	if (!wep)
		return true;

	bool ret = true;
	Features::Bhop::perform(local_player, cmd);
	Features::EnginePrediction::start(cmd);
	{
		ret = Features::Aimbot::perform(local_player, cmd);
		if (Vars::Aimbot::mode == 4) {
			if (Features::Aimbot::next_LAC_stage != Features::Aimbot::LAC_Stage::NONE)
			{
				switch (Features::Aimbot::next_LAC_stage)
				{
				case Features::Aimbot::LAC_Stage::PRE_FIRE:

					Features::Aimbot::next_LAC_stage = Features::Aimbot::LAC_Stage::FIRE;
					cmd->buttons &= ~IN_ATTACK;
					break;

				case Features::Aimbot::LAC_Stage::FIRE:

					cmd->buttons |= IN_ATTACK;

					Features::Aimbot::next_LAC_stage = Features::Aimbot::LAC_Stage::POST_FIRE;

					cmd->viewangles = Features::Aimbot::target;
					break;

				default:

					Features::Aimbot::next_LAC_stage = Features::Aimbot::LAC_Stage::NONE;

					cmd->buttons |= IN_ATTACK;

					cmd->viewangles.y = Features::Aimbot::target.y + 4.f;
					break;
				}
			}

			Features::Aimbot::AimStep(cmd);
		}

	}
	Features::EnginePrediction::finish();
	

    

	if (Vars::Funny::fakelag > 0)
		return ret && (cmd->command_number % (Vars::Funny::fakelag + 1) == 0);

    return ret;
}

void CheatEvents::FrameStageNotify(ClientFrameStage_t stage) {
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_END)
	{
		if (I::EngineClient->IsInGame())
		{
			CheatState::max_clients = I::EngineClient->GetMaxClients();
			CheatState::max_entities = I::ClientEntityList->GetMaxEntities();
		}
	}
}

void CheatEvents::LevelInitPostEntity() {
	CheatState::local_player_idx = I::EngineClient->GetLocalPlayer();
}

void CheatEvents::LevelShutdown() {
	CheatState::local_player_idx = CheatState::max_clients = CheatState::max_entities = -1;
}
