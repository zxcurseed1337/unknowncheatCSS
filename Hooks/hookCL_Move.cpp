#include "pch.h"
#include "hookCL_Move.h"

#include "SourceDefs/NetMsgs.h"

#include "Events/Events.h"

void __fastcall  hkCL_Move(float sample_time, bool active);
decltype(hkCL_Move)* CL_Move;
int64_t(__fastcall* GetCheckSum)(CUserCmd* usercmd);
bool(__stdcall* Host_ShouldRun)(); //48 83 EC ? 48 8B 05 ? ? ? ? 83 78 ? ? 74 ? 48 8B 05  (first call in CL_Move)
bool(__stdcall* cl_IsPaused)(CClientState*);
static double* net_time;
static float* interval_per_tick;
static float* host_frametime_stddeviation;
static float* host_frametime_unbounded;
static int* some_weird_value;
static void** net_tick_vtable;
static void** clc_move_vtable;

void FixMovement(CUserCmd* cmd) {

    QAngle v;
    I::EngineClient->GetViewAngles(v);
    float clientSideYaw = v[1];

    float fm = cmd->forwardmove;
    float sm = cmd->sidemove;

    if (fm != 0 || sm != 0) {

        // Get movement speed for normalization and de-normalization
        float moveSpeed = min(450.f, sqrt(fm * fm + sm * sm));

        // Get delta from clientside yaw to actual cmd yaw
        float radDelta = DEG2RAD(cmd->viewangles[1] - clientSideYaw);

        // Normalize
        fm /= moveSpeed;
        sm /= moveSpeed;

        // https://www.codegrepper.com/code-examples/whatever/rotate+2d+vector+by+angle
        cmd->forwardmove = (fm * cos(radDelta) - sm * sin(radDelta)) * moveSpeed;
        cmd->sidemove = (fm * sin(radDelta) + sm * cos(radDelta)) * moveSpeed;
    }
}

bool ManipCommand(int nextcommandnr) {
    CUserCmd* cmd = &I::Input->m_pCommands[nextcommandnr % 90];
    CVerifiedUserCmd* vcmd = &I::Input->m_pVerifiedCommands[nextcommandnr % 90];

    bool ret = true;
    if (cmd && cmd->command_number > 0) {
        ret = CheatEvents::ModifyCmd(cmd);

    }
    auto chkcsum = GetCheckSum(cmd);
    FixMovement(cmd);
    vcmd->m_cmd = *cmd;
    vcmd->m_crc = chkcsum;
    return ret;
}

#define MAX_CMD_BUFFER 4000
void CL_SendMove2() {
    byte data[MAX_CMD_BUFFER];

    int nextcommandnr = I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands + 1;

    // send the client update packet

    CLC_Move moveMsg;

    moveMsg.m_DataOut.StartWriting(data, sizeof(data));
    // Determine number of backup commands to send along
    int cl_cmdbackup = 2;
    moveMsg.m_nBackupCommands = min(max(cl_cmdbackup, 0), MAX_BACKUP_COMMANDS);

    moveMsg.m_nNewCommands = 1 + I::ClientState->chokedcommands;
    moveMsg.m_nNewCommands = min(max(moveMsg.m_nNewCommands, 0), MAX_NEW_COMMANDS);
    // How many real new commands have queued up
   
    

    int numcmds = moveMsg.m_nNewCommands + moveMsg.m_nBackupCommands;

    int from = -1;    // first command is deltaed against zeros

    bool bOK = true;

    for (int to = nextcommandnr - numcmds + 1; to <= nextcommandnr; to++)
    {
        bool isnewcmd = to >= (nextcommandnr - moveMsg.m_nNewCommands + 1);

        // first valid command number is 1
        bOK = bOK && I::BaseClientDLL->WriteUsercmdDeltaToBuffer(&moveMsg.m_DataOut, from, to, isnewcmd);
        from = to;
    }

    if (bOK)
    {
        // only write message if all usercmds were written correctly, otherwise parsing would fail
        I::ClientState->m_NetChannel->SendNetMsg(moveMsg);
    }
}

void __fastcall MyCL_Move(float sample_time, bool active) {
    if (!(I::ClientState->m_nSignonState >= SIGNONSTATE_CONNECTED))
        return;
    if (!Host_ShouldRun())
        return;

    bool bSendPacket = true;
    INetChannel* net_channel = I::ClientState->m_NetChannel;
    static auto host_limitlocal = I::Cvar->FindVar("host_limitlocal");
    static auto cl_cmdrate = I::Cvar->FindVar("cl_cmdrate");
    bSendPacket = true;

    if (!net_channel->IsLoopback() || host_limitlocal->GetInt())
        if (I::ClientState->m_flNextCmdTime > *net_time || !net_channel->CanPacket() || !active)
            bSendPacket = false;


    if (I::ClientState->m_nSignonState == SIGNONSTATE_FULL)
    {
        int nextcommandnr = I::ClientState->lastoutgoingcommand + 1 + I::ClientState->chokedcommands;
        I::BaseClientDLL->CreateMove(nextcommandnr, I::GlobalVars->interval_per_tick - sample_time, !cl_IsPaused(I::ClientState)); //cl_IsPaused(I::IClientState)
        bSendPacket = ManipCommand(nextcommandnr);

        if (!bSendPacket)
        {
            net_channel->SetChoked();
            I::ClientState->chokedcommands += 1;
        }
        else {
            CL_SendMove2();
        }
    }

    if (I::ClientState->m_nSignonState == SIGNONSTATE_FULL)
    {
        NET_Tick mymsg(I::ClientState->m_nDeltaTick, *host_frametime_unbounded, *host_frametime_stddeviation);
        net_channel->SendNetMsg(mymsg);
    }

    if (!bSendPacket)
        return;


    I::ClientState->lastoutgoingcommand = net_channel->SendDatagram(NULL);
    I::ClientState->chokedcommands = 0;

    if (I::ClientState->m_nSignonState == SIGNONSTATE_FULL)
    {
        // use full update rate when active
        float commandInterval = 1.0f / cl_cmdrate->GetFloat();
        float maxDelta = min(I::GlobalVars->interval_per_tick, commandInterval);
        float delta = min(max(*net_time - I::ClientState->m_flNextCmdTime, 0.0), (double)maxDelta);
        I::ClientState->m_flNextCmdTime = *net_time + commandInterval - delta;
    }
    else
    {
        // during signon process send only 5 packets/second
        I::ClientState->m_flNextCmdTime = *net_time + 0.2f;
    }
}

void __fastcall hkCL_Move(float sample_time, bool active) {
    MyCL_Move(sample_time, active);
}

void Hooks::hook_CL_Move() {
//40 55 53 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 83 3D
	uintptr_t original;

    Host_ShouldRun  = reinterpret_cast<decltype(Host_ShouldRun)>(Utility::find_bytes(enginedll, "48 83 EC ? 48 8B 05 ? ? ? ? 83 78 ? ? 74 ? 48 8B 05").get());

    net_time = reinterpret_cast<double*>(Utility::find_bytes(enginedll, "F2 0F 10 0D ? ? ? ? F2 0F 5C 0D").fixRip(4).get());//; (in CL_Move)
    host_frametime_unbounded = reinterpret_cast<float*>(Utility::find_bytes(enginedll, "F3 0F 10 05 ? ? ? ? F3 0F 11 45 ? F3 0F 11 4D ? 89 45").fixRip(4).get());//; (in CL_Move)
    host_frametime_stddeviation = reinterpret_cast<float*>(Utility::find_bytes(enginedll, "F3 0F 10 0D ? ? ? ? 48 8D 54 24 ? 0F 57 C0 48 89 44 24 ? 8B 05").fixRip(4).get());
    cl_IsPaused = reinterpret_cast<decltype(cl_IsPaused)>(Utility::find_bytes(enginedll, "48 83 EC ? 80 B9 ? ? ? ? ? 75").get());

    GetCheckSum = reinterpret_cast<decltype(GetCheckSum)>(Utility::find_bytes(Utility::modules["client.dll"], "40 53 48 83 EC 20 48 8B D9 48 8D 4C 24 30 E8 ? ? ? ? 48 8D 53 08 41 B8 04 00 00 00 48 8D 4C 24 30 E8 ? ? ? ? 48 8D 53 0C 41 B8 04 00 00 00").get());

    hook(Utility::find_bytes(enginedll, "40 55 53 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 83 3D").get(), reinterpret_cast<uintptr_t>(hkCL_Move), original);
    CL_Move = reinterpret_cast<decltype(CL_Move)>(original);
}


