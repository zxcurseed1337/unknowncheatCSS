#include "pch.h"
#include "menu.h"

bool bogus;
void draw_misc() {
    ImGui::Separator();
    ImGui::Checkbox("No Visual Recoil", &Vars::Visual::RemoveVisualRecoil);
    ImGui::Checkbox("Imposter", &bogus);
    ImGui::Separator();
    ImGui::Checkbox("Bunnyhop", &Vars::Misc::Bunnyhop);
    ImGui::SliderInt("Max Perfect Hops", &Vars::Misc::MaxPerfectHops, 0, 100);
    ImGui::SliderFloat("Bhop Reset Time", &Vars::Misc::BhopResetTime, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Checkbox("Autopistol", &bogus);
    ImGui::Checkbox("Banme", &Vars::Misc::BanMe);
    ImGui::Separator();
    ImGui::SliderInt("Fakelag", &Vars::Funny::fakelag, 0, 62);
    ImGui::SliderInt("Speedhack", &Vars::Funny::speedhack_factor, 1, 62);
}