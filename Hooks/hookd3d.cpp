#include "pch.h"
#include "hookd3d.h"
#include "State/CheatState.h"
#include "Features/ESP.h"
#include "Menu/menu.h"
//MAKE_IFACE(IDirect3DDevice9, dxdev, mem::findBytes("shaderapidx9.dll", "48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 8B 01 FF 50 ? 48 C7 05").fixRip(3).deref());

HRESULT __stdcall hkD3D_EndScene(IDirect3DDevice9* that);
decltype(hkD3D_EndScene)* EndScene;

HRESULT __stdcall hkD3D_Reset(IDirect3DDevice9* that, D3DPRESENT_PARAMETERS* Params);
decltype(hkD3D_Reset)* Reset;

void NewStyle();
void Initialize_GUI(IDirect3DDevice9* vDevice);

ImFont* font;


bool done = false;
HRESULT __stdcall hkD3D_EndScene(IDirect3DDevice9* that) {
	if (done == false) {
		Initialize_GUI(that);
		done = true;
		return EndScene(that);
	}
	static auto wanted_ret_address = _ReturnAddress();
	if (_ReturnAddress() == wanted_ret_address)
	{
		//backup render states
		DWORD colorwrite, srgbwrite;
		that->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		that->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		//fix drawing without calling engine functons/cl_showpos
		that->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//removes the source engine color correction
		that->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(font);

		if (CheatState::menu_open) {


			draw_menu();



			// Rendering

		}
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(CheatState::screensize_x, CheatState::screensize_y));
		ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

		Features::ESP::draw();
		

		ImGui::End();
		ImGui::PopFont();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		that->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		that->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	}
	return EndScene(that);
}

HRESULT __stdcall hkD3D_Reset(IDirect3DDevice9* that, D3DPRESENT_PARAMETERS* Params) {
	if (done == false)
		return Reset(that, Params);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT bReturn = Reset(that, Params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return bReturn;
}

void Hooks::hook_d3d() {
	IDirect3DDevice9* dev = (IDirect3DDevice9 * )Utility::find_bytes(Utility::modules["shaderapidx9.dll"], "48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 8B 01 FF 50 ? 48 C7 05").fixRip(3).deref().get();

	uintptr_t original;
	hook(Utility::find_virtual(dev, 42u).get(), reinterpret_cast<uintptr_t>(hkD3D_EndScene), original);
	EndScene = reinterpret_cast<decltype(EndScene)>(original);

	hook(Utility::find_virtual(dev, 16u).get(), reinterpret_cast<uintptr_t>(hkD3D_Reset), original);
	Reset = reinterpret_cast<decltype(Reset)>(original);
	//hook(Utility::GetVtableMethod<uintptr_t>(d3ddevice, 16u), reinterpret_cast<uintptr_t>(hkD3D_Reset), original);
}

void Initialize_GUI(IDirect3DDevice9* vDevice) {
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(FindWindowA("Valve001", NULL));
	ImGui_ImplDX9_Init(vDevice);


	NewStyle();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14, NULL, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->Build();
}

void NewStyle() {
	auto* style = &ImGui::GetStyle();
	style->FrameRounding = 2.0f;
	style->WindowPadding = ImVec2(4.0f, 3.0f);
	style->FramePadding = ImVec2(4.0f, 4.0f);
	style->ItemSpacing = ImVec2(4.0f, 3.0f);
	style->IndentSpacing = 12;
	style->ScrollbarSize = 12;
	style->GrabMinSize = 9;

	// Sizes
	style->WindowBorderSize = 0.0f;
	style->ChildBorderSize = 0.0f;
	style->PopupBorderSize = 0.0f;
	style->FrameBorderSize = 0.0f;
	style->TabBorderSize = 0.0f;


	style->WindowRounding = 0.0f;
	style->ChildRounding = 0.0f;
	style->FrameRounding = 0.0f;
	style->PopupRounding = 0.0f;
	style->GrabRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->TabRounding = 0.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.17f, 0.18f, 0.20f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.24f, 0.25f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.16f, 0.17f, 0.18f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.16f, 0.17f, 0.18f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.15f, 0.16f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.19f, 0.20f, 0.22f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.22f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.17f, 0.18f, 0.20f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.84f, 0.34f, 0.17f, 0.14f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.28f, 0.14f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.17f, 0.18f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}
