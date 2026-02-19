#include "pch.h"
#include "NetMsgs.h"

bool CLC_Move::WriteToBuffer(bf_write& buffer)
{
	buffer.WriteUBitLong(GetType(), NETMSG_TYPE_BITS);
	m_nLength = m_DataOut.GetNumBitsWritten();

	buffer.WriteUBitLong(m_nNewCommands, NUM_NEW_COMMAND_BITS);
	buffer.WriteUBitLong(m_nBackupCommands, NUM_BACKUP_COMMAND_BITS);

	buffer.WriteWord(m_nLength);

	return buffer.WriteBits(m_DataOut.GetData(), m_nLength);
}

bool CLC_Move::ReadFromBuffer(bf_read& buffer)
{
	m_nNewCommands = buffer.ReadUBitLong(NUM_NEW_COMMAND_BITS);
	m_nBackupCommands = buffer.ReadUBitLong(NUM_BACKUP_COMMAND_BITS);
	m_nLength = buffer.ReadWord();
	m_DataIn = buffer;
	return buffer.SeekRelative(m_nLength);
}

bool NET_Tick::WriteToBuffer(bf_write& buffer)
{
	buffer.WriteUBitLong(GetType(), NETMSG_TYPE_BITS);
	buffer.WriteLong(m_nTick);
	buffer.WriteUBitLong(std::clamp((int)(NET_TICK_SCALEUP * m_flHostFrameTime), 0, 65535), 16);
	buffer.WriteUBitLong(std::clamp((int)(NET_TICK_SCALEUP * m_flHostFrameTimeStdDeviation), 0, 65535), 16);
	return !buffer.IsOverflowed();
}

bool NET_Tick::ReadFromBuffer(bf_read& buffer)
{
	m_nTick = buffer.ReadLong();
	m_flHostFrameTime = (float)buffer.ReadUBitLong(16) / NET_TICK_SCALEUP;
	m_flHostFrameTimeStdDeviation = (float)buffer.ReadUBitLong(16) / NET_TICK_SCALEUP;
	return !buffer.IsOverflowed();
}

const char* NET_Tick::ToString(void) const
{
	const std::string result = std::format("{}: tick {}", GetName(), m_nTick);

#pragma warning (push)
#pragma warning (disable : 26816)
	return result.c_str();
#pragma warning (pop)
}