#include "pch.h"
#include "menu.h"

int tab = 0;


void draw_menu() {
    ImGui::SetNextWindowSize(ImVec2(800, 600));
    ImGui::Begin("LMAOBOX 2 2 2", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::RadioButton("Aimbot",      &tab, 0); ImGui::SameLine();
    ImGui::RadioButton("Visual",      &tab, 1); ImGui::SameLine();
    ImGui::RadioButton("Misc",        &tab, 2); ImGui::SameLine();
    ImGui::RadioButton("Player List", &tab, 3); ImGui::SameLine();
    ImGui::RadioButton("Trivia",      &tab, 4); 
    ImGui::Separator();

    switch (tab) {
    case 0:
        draw_aimbot();
        break;
    case 1:
        draw_visual();
        break;
    case 2:
        draw_misc();
        break;
    case 3:
        draw_plist();
        break;
    case 4:
        draw_trivia();
        break;
    }
    ImGui::End();
}
