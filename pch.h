// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

// Добавьте сюда заголовочные файлы для предварительной компиляции
#include "framework.h"

#include <print>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <chrono>
#include <thread>
#include <functional>
#include <fstream>
#include <thread>
#include <mutex>
#include <cassert>
#include <format>
#include <filesystem>
#include <unordered_set>
#include <cstdint>

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <Psapi.h>

#include <numeric>

#include "Utility/Modules.h"

#pragma warning( push )
#pragma warning( disable : 26495 )
#pragma warning( disable : 28251 )
#pragma warning( disable : 28182 )

#pragma warning( disable : 6386 )
#pragma warning( disable : 6201 )
#pragma warning( disable : 4005 )
#pragma warning( disable : 6385 )
#include <intrin.h>
#include <basehandle.h>

#include <bitbuf.h>
#include <usercmd.h>
#include <cdll_int.h>
#include <icliententitylist.h>
#include <ivrenderview.h>
#include <istudiorender.h>
#include <iprediction.h>
#include <imaterialsystem.h>
#include <igamemovement.h>
#include <ienginevgui.h>
#include <ienginetrace.h>
#include <icvar.h>
#include <ivmodelrender.h>
#include <IMatSystemSurface.h>
#include <inetchannel.h>


#define DECLARE_PREDICTABLE_IMPL( MAYBE_OVERRIDE )						\
	public:																\
		static typedescription_t m_PredDesc[];							\
		static datamap_t m_PredMap;										\
		virtual datamap_t *GetPredDescMap( void ) MAYBE_OVERRIDE;		\
		template <typename T> friend datamap_t *PredMapInit(T *)
#define DECLARE_PREDICTABLE()	DECLARE_PREDICTABLE_IMPL( /* override */; )
// With warnings set to inconsistent-override, marking this properly as override would create warnings in all old files.
// Instead, as files are converted to use override, just upgrade to this
#define DECLARE_PREDICTABLE_OVERRIDE()	DECLARE_PREDICTABLE_IMPL( OVERRIDE )

#include <c_baseentity.h>
#include <c_baseplayer.h>
#include <c_basecombatweapon.h>
#pragma warning( pop )
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGUI/imgui.h"
#include "ImGUI/backends/imgui_impl_dx9.h"
#include "ImGUI/backends/imgui_impl_win32.h"
#include "Libraries/MinHook/include/MinHook.h"
#pragma comment(lib, "Libraries/MinHook/lib/libMinHook-x64-v141-mdd.lib")
#include <d3d9.h>
//#include "Libraries/imgui/imgui.hpp"
//#include "Libraries/imgui/imgui_impl_dx9.hpp"
//#include "Libraries/imgui/imgui_impl_win32.hpp"
//#include "Libraries/imgui/imgui_freetype.hpp"
#include "interfaces.h"

#include "State/Settings.h"
#include "Utility/memory.h"

#include "State/CheatState.h"


using namespace std::chrono_literals;
using namespace std::string_literals;

void hook(uintptr_t what, uintptr_t with, uintptr_t& original);
unsigned constexpr const_hash(char const* input) {
    return *input ?
        static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
        5381;
}
#define FastRSqrt( x ) ( 1.0f / ::sqrtf( x ) )

#define FastCos ::cosf
#define FastSqrt ::sqrtf
#define FastSinCos ::SinCos
#define FastRSqrtFast FastRSqrt

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2
#define	IN_ATTACK3		(1 << 25)

enum CSWeaponID
{
	WEAPON_NONE = 0,
	WEAPON_P228,
	WEAPON_GLOCK,
	WEAPON_SCOUT,
	WEAPON_HEGRENADE,
	WEAPON_XM1014,
	WEAPON_C4,
	WEAPON_MAC10,
	WEAPON_AUG,
	WEAPON_SMOKEGRENADE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_UMP45,
	WEAPON_SG550,
	WEAPON_GALIL,
	WEAPON_FAMAS,
	WEAPON_USP,
	WEAPON_AWP,
	WEAPON_MP5NAVY,
	WEAPON_M249,
	WEAPON_M3,
	WEAPON_M4A1,
	WEAPON_TMP,
	WEAPON_G3SG1,
	WEAPON_FLASHBANG,
	WEAPON_DEAGLE,
	WEAPON_SG552,
	WEAPON_AK47,
	WEAPON_KNIFE,
	WEAPON_P90,
	WEAPON_SHIELDGUN,
	WEAPON_KEVLAR,
	WEAPON_ASSAULTSUIT,
	WEAPON_NVG,
	WEAPON_MAX,
};


class C_WeaponCSBase : public C_BaseCombatWeapon
{
public:
	virtual				~C_WeaponCSBase() = 0;

	virtual bool		IsFullAuto() const = 0;
	virtual float		GetMaxSpeed() const = 0;
	virtual CSWeaponID	GetWeaponID(void) const = 0;
	virtual bool		IsSilenced(void) const = 0;
	virtual void		SetWeaponModelIndex(const char* pName) = 0;
	virtual void		OnJump(float fImpulse) = 0;
	virtual void		OnLand(float fVelocity) = 0;
	virtual int			GetMuzzleAttachment(void) = 0;
	virtual bool		HideViewModelWhenZoomed(void) = 0;
	virtual void		UpdateShieldState(void) = 0;
	virtual void*	GetDeployActivity(void) = 0;
	
	virtual float		GetInaccuracy() const = 0;
		virtual bool		DefaultPistolReload() = 0;
	virtual float		GetSpread() const = 0;
	virtual void		UpdateAccuracyPenalty() = 0;

	//NETVAR(m_iPrimaryReserveAmmoCount, int, "DT_WeaponCSBase", "m_iPrimaryReserveAmmoCount");

};

#endif //PCH_H
