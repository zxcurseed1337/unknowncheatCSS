#include "pch.h"
#include "bhop.h"

#include "SourceDefs/NetVar.h"

int perfect_hops = 0;
float bhop_reset_time = 0.f;


void Features::Bhop::perform(C_BasePlayer* local_player, CUserCmd* cmd) {
	if (cmd->buttons & IN_JUMP)
	{
		int flags = get_Flags(local_player);
		if (!(flags & 1))
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}
}
