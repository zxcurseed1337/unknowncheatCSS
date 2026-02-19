#include "pch.h"
#include "menu.h"

void draw_aimbot(){
    ImGui::RadioButton("None",    &Vars::Aimbot::mode, 0); ImGui::SameLine();
    ImGui::RadioButton("Str8",    &Vars::Aimbot::mode, 1); ImGui::SameLine();
    ImGui::RadioButton("Silent",  &Vars::Aimbot::mode, 2); ImGui::SameLine();
    ImGui::RadioButton("PSilent", &Vars::Aimbot::mode, 3); ImGui::SameLine();
    ImGui::RadioButton("LILAc",   &Vars::Aimbot::mode, 4);

    ImGui::SliderFloat("FOV", &Vars::Aimbot::max_fov, 0.0f, 180.0f);
    ImGui::SliderFloat("Awall Min Dmg", &Vars::Aimbot::awall_min_dmg, 0.1f, 180.0f);

    ImGui::Checkbox("Autoshoot", &Vars::Aimbot::autoshoot);
    ImGui::Checkbox("Perfect Accuracy", &Vars::Aimbot::perfect_accuracy);
    ImGui::Separator();
    ImGui::Text("Hitboxes");

    ImGui::Checkbox("Head",        &Vars::Aimbot::hitbox[0]);
    ImGui::Checkbox("Neck",        &Vars::Aimbot::hitbox[1]);
    ImGui::Checkbox("Upper Chest", &Vars::Aimbot::hitbox[2]);
    ImGui::Checkbox("Chest",       &Vars::Aimbot::hitbox[3]);
    ImGui::Checkbox("Stomach",     &Vars::Aimbot::hitbox[4]);
    ImGui::Checkbox("Pelvis",      &Vars::Aimbot::hitbox[5]);
}