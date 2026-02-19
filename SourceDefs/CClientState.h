#pragma once
#include "pch.h"

struct CClockDriftMgr
{
	float m_ClockOffsets[16]{};
	int m_iCurClockOffset{};
	int m_nServerTick{};
	int m_nClientTick{};
};

enum SignonState_t {
SIGNONSTATE_NONE,
SIGNONSTATE_CHALLENGE,
SIGNONSTATE_CONNECTED,
SIGNONSTATE_NEW,
SIGNONSTATE_PRESPAWN,
SIGNONSTATE_SPAWN,
SIGNONSTATE_FULL,
SIGNONSTATE_CHANGELEVEL,

};

class CClientState
{
private:
	byte gap0[24]{};
public:
	int m_Socket;
	INetChannel* m_NetChannel{};
	unsigned int m_nChallengeNr{};
	double m_flConnectTime{};
	int m_nRetryNumber{};
	char m_szRetryAddress[260]{};
	CUtlString m_sRetrySourceTag{};
	int m_retryChallenge{};
	int m_nSignonState{};
	double m_flNextCmdTime{};
	int m_nServerCount{};
	unsigned __int64 m_ulGameServerSteamID{};
	int m_nCurrentSequence{};
	CClockDriftMgr m_ClockDriftMgr{};
	int m_nDeltaTick{};
	bool m_bPaused{};
	float m_flPausedExpireTime{};
	int m_nViewEntity{};
	int m_nPlayerSlot{};
	char m_szLevelFileName[128]{};
private:
	byte gap24C[132]{};
public:
	char m_szLevelBaseName[128]{};
private:
	byte gap350[116]{};
public:
	int m_nMaxClients{};
private:
	byte gap3C8[34856]{};
public:
	void* m_StringTableContainer{};
	bool m_bRestrictServerCommands{};
	bool m_bRestrictClientCommands{};
private:
	byte gap8BFC[106]{};
public:
	bool insimulation{};
	int oldtickcount{};
	float m_tickRemainder{};
	float m_frameTime{};
	int lastoutgoingcommand{};
	int chokedcommands{};
	int last_command_ack{};
	int command_ack{};
	int m_nSoundSequence{};
	bool ishltv{};
	bool isreplay{};
private:
	byte gap8C8A[278]{};
public:
	int demonum{};
	CUtlString demos[32]{};
private:
	byte gap8EE8[344184]{};
public:
	bool m_bMarkedCRCsUnverified{};

public:
	inline bool IsConnected() const
	{
		return m_nSignonState >= SIGNONSTATE_CONNECTED;
	}

	inline bool IsActive() const
	{
		return m_nSignonState == SIGNONSTATE_FULL;
	}

	inline bool IsPaused()
	{
		//return s::CClientState_IsPaused.call<bool>(this);
	}

	inline void ForceFullUpdate()
	{
		//s::CClientState_ForceFullUpdate.call<void>(this);
	}

public:
	float GetClientInterpAmount()
	{
		//return s::CClientState_GetClientInterpAmount.call<float>(this);
	}

	void SendStringCmd(const char* command)
	{
		//s::CBaseClientState_SendStringCmd.call<void>(this, command);
	}
};