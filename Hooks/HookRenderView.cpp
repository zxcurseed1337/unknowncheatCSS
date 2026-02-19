#include "pch.h"
#include "HookRenderView.h"
#include "State/CheatState.h"
void __fastcall hkRenderView(void* that, CViewSetup& view, int nClearFlags, int whatToDraw);
decltype(hkRenderView)* RenderView;
enum StereoEye_t
{
	STEREO_EYE_MONO = 0,
	STEREO_EYE_LEFT = 1,
	STEREO_EYE_RIGHT = 2,
	STEREO_EYE_MAX = 3,
};
class CViewSetup
{
public:


	// shared by 2D & 3D views

		// left side of view window
	int			x;
	int			m_nUnscaledX;
	// top side of view window
	int			y;
	int			m_nUnscaledY;
	// width of view window
	int			width;
	int			m_nUnscaledWidth;
	// height of view window
	int			height;
	// which eye are we rendering?
	StereoEye_t m_eStereoEye;
	int			m_nUnscaledHeight;

	// the rest are only used by 3D views

		// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vector		origin;

	// heading of camera (pitch, yaw, roll)
	Vector		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState;

	// If using VR, the headset calibration will feed you a projection matrix per-eye.
	// This does NOT override the Z range - that will be set up as normal (i.e. the values in this matrix will be ignored).
	bool        m_bViewToProjectionOverride;
	VMatrix     m_ViewToProjection;
};
#include "SourceDefs/NetVar.h"
float o_not_two = 10.f;
void __fastcall hkRenderView(void* that, CViewSetup& view, int nClearFlags, int whatToDraw) {
	view.m_flAspectRatio = Vars::Visual::aspect_ratio;
	view.fov += Vars::Visual::BonusFOV;
	view.origin += Vector(Vars::Visual::view_x_offset, Vars::Visual::view_y_offset, Vars::Visual::view_z_offset);
	view.angles += Vector(Vars::Visual::view_pitch_offset, Vars::Visual::view_yaw_offset, Vars::Visual::view_roll_offset);
	view.fovViewmodel += Vars::Visual::additional_viewmodel_fov;

	int local = I::EngineClient->GetLocalPlayer();

	auto local_player_ = I::ClientEntityList->GetClientEntity(local);
	if (!local_player_)
		return RenderView(that, view, nClearFlags, whatToDraw);

	CheatState::znear = view.zNear;
	CheatState::current_fov = view.fov;


	auto local_player = dynamic_cast<C_BasePlayer*>(local_player_->GetBaseEntity());
	


	RenderView(that, view, nClearFlags, whatToDraw);
	 
	I::RenderView->GetMatricesForView(view, &CheatState::WorldToView, &CheatState::ViewToProjection, &CheatState::m_WorldToProjection, &CheatState::WorldToPixels);
}

void Hooks::hook_RenderView() {
	uintptr_t original;

	/*
	.text:00000001801D6940 48 8B C4                                                        mov     rax, rsp
	.text:00000001801D6943 44 89 48 20                                                     mov     [rax+20h], r9d
	.text:00000001801D6947 44 89 40 18                                                     mov     [rax+18h], r8d
	.text:00000001801D694B 48 89 50 10                                                     mov     [rax+10h], rdx
	.text:00000001801D694F 48 89 48 08                                                     mov     [rax+8], rcx
	.text:00000001801D6953 55                                                              push    rbp
	.text:00000001801D6954 53                                                              push    rbx
	.text:00000001801D6955 41 57                                                           push    r15
	.text:00000001801D6957 48 8D A8 E8 FC FF FF                                            lea     rbp, [rax-318h]
	.text:00000001801D695E 48 81 EC 00 04 00 00   
	*/
	hook(Utility::find_bytes(Utility::modules["client.dll"], "48 8B C4 44 89 48 20 44 89 40 18 48 89 50 10 48 89 48 08 55 53 41 57 48 8D A8 E8 FC FF FF 48 81 EC 00 04 00 00").get(), (uintptr_t)hkRenderView, original); //CViewRender::RenderView string

	RenderView = (decltype(RenderView))original;
}
