#pragma once
#include "pch.h"

#include "Utility/memory.h"
#include "State/Settings.h"
class CVerifiedUserCmd
{
public:
	CUserCmd m_cmd;
	uint64_t m_crc;
};


class IInput
{
public:
	char pad_0000[0x108];

public:
	CUserCmd* m_pCommands{};
	CVerifiedUserCmd* m_pVerifiedCommands{};

public:
	void CAM_ToThirdPerson()
	{
		//*(bool*)(this + 0xA1) = true;
		*(Vector*)(this + 0xA2) = Vector(Vars::Visual::view_x_offset, Vars::Visual::view_y_offset, Vars::Visual::view_z_offset);
		Utility::callVirtual<32, void>(this);
	}

	void CAM_ToFirstPerson()
	{
		Utility::callVirtual<33, void>(this);
	}
};