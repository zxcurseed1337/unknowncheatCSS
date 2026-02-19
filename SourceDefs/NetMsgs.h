#pragma once
class INetMessage
{
public:
	virtual ~INetMessage() {};
	virtual void SetNetChannel(INetChannelInfo* netchan) = 0;
	virtual void SetReliable(bool state) = 0;
	virtual bool Process(void) = 0;
	virtual bool ReadFromBuffer(bf_read& buffer) = 0;
	virtual bool WriteToBuffer(bf_write& buffer) = 0;
	virtual bool IsReliable(void) const = 0;
	virtual int GetType(void) const = 0;
	virtual int GetGroup(void) const = 0;
	virtual const char* GetName(void) const = 0;
	virtual INetChannelInfo* GetNetChannel(void) const = 0;
	virtual const char* ToString(void) const = 0;
};

class CNetMessage : public INetMessage
{
public:
	CNetMessage()
	{
		m_bReliable = true;
		m_NetChannel = nullptr;
	}

public:
	virtual ~CNetMessage() {};
	virtual int GetGroup() const { return INetChannelInfo::GENERIC; }
	virtual void SetReliable(bool state) { m_bReliable = state; };
	virtual bool IsReliable() const { return m_bReliable; };
	virtual void SetNetChannel(INetChannelInfo* netchan) { m_NetChannel = netchan; }
	virtual bool Process() { return false; }; // no handler set

public:
	INetChannelInfo* GetNetChannel() const { return m_NetChannel; }

protected:
	bool m_bReliable{};
	INetChannelInfo* m_NetChannel{};
	byte pad0[8]{};
};

#define net_NOP 0
#define net_Disconnect 1
#define net_File 2
#define net_Tick 3
#define net_StringCmd 4
#define net_SetConVar 5
#define net_SignonState 6
#define clc_ClientInfo 8       
#define clc_Move 9             
#define clc_VoiceData 10       
#define clc_BaselineAck 11     
#define clc_ListenEvents 12    
#define clc_RespondCvarValue 13
#define clc_FileCRCCheck 14    
#define clc_CmdKeyValues 16    

#define DELTA_OFFSET_BITS 5
#define DELTA_OFFSET_MAX ( ( 1 << DELTA_OFFSET_BITS ) - 1 )
#define DELTASIZE_BITS 20	
#define NUM_NEW_COMMAND_BITS 4
#define MAX_NEW_COMMANDS ((1 << NUM_NEW_COMMAND_BITS)-1)
#define NUM_BACKUP_COMMAND_BITS 3
#define MAX_BACKUP_COMMANDS ((1 << NUM_BACKUP_COMMAND_BITS)-1)
#define MAX_PLAYER_NAME_LENGTH 32
#define MAX_CUSTOM_FILES 4
#define NETMSG_TYPE_BITS 6

class CLC_Move : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const { return "hi"; }
	int GetType() const { return clc_Move; }
	const char* GetName() const { return "clc_Move"; }
	void* m_pMessageHandler{ nullptr };
	int GetGroup() const { return INetChannelInfo::MOVE; }
	CLC_Move() { m_bReliable = false; }

public:
	int m_nBackupCommands{};
	int m_nNewCommands{};
	int m_nLength{};
	bf_read m_DataIn{};
	bf_write m_DataOut{};
};

#define NET_TICK_SCALEUP 100000.0f

class NET_Tick : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return net_Tick; }
	const char* GetName() const { return "net_Tick"; }
	void* m_pMessageHandler{ nullptr };

public:
	NET_Tick()
	{
		m_bReliable = false;
		m_flHostFrameTime = 0;
		m_flHostFrameTimeStdDeviation = 0;
	};

	NET_Tick(int tick, float hostFrametime, float hostFrametime_stddeviation)
	{
		m_bReliable = false;
		m_nTick = tick;
		m_flHostFrameTime = hostFrametime;
		m_flHostFrameTimeStdDeviation = hostFrametime_stddeviation;
	};

public:
	int m_nTick{};
	float m_flHostFrameTime{};
	float m_flHostFrameTimeStdDeviation{};
};
