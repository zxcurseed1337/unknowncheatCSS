#include "pch.h"
#include "menu.h"

#include "SourceDefs/NetVar.h"

void draw_visual() {
    ImGui::Checkbox("ESP",            &Vars::ESP::Enabled);
    ImGui::Checkbox("Draw Box",       &Vars::ESP::Players::Box);
    ImGui::Checkbox("Draw Healthbar", &Vars::ESP::Players::HealthBar);
    ImGui::Checkbox("Ignore Team",    &Vars::ESP::Players::IgnoreTeam);
    ImGui::Checkbox("Draw Name",      &Vars::ESP::Players::Name);

    ImGui::Separator();
    ImGui::SliderFloat("Bonus FOV", &Vars::Visual::BonusFOV, -60.0f, 80.0f);
    ImGui::SliderFloat("More Viewmodel FOV", &Vars::Visual::additional_viewmodel_fov, -90.0f, 90.0f);
    ImGui::Separator();

    ImGui::SliderFloat("Viewmodel X",        &Vars::Visual::vm_x_offset,              -90.0f, 90.0f);
    ImGui::SliderFloat("Viewmodel Y",        &Vars::Visual::vm_y_offset,              -90.0f, 90.0f);
    ImGui::SliderFloat("Viewmodel Z",        &Vars::Visual::vm_z_offset,              -90.0f, 90.0f);
    ImGui::Separator();
    ImGui::SliderFloat("Viewmodel Pitch",    &Vars::Visual::vm_pitch_offset,         -180.0f, 180.0f);
    ImGui::SliderFloat("Viewmodel Yaw",      &Vars::Visual::vm_yaw_offset,           -180.0f, 180.0f);
    ImGui::SliderFloat("Viewmodel Roll",     &Vars::Visual::vm_roll_offset,          -180.0f, 180.0f);
    ImGui::Separator();

    ImGui::SliderFloat("View X",  &Vars::Visual::view_x_offset, -90.0f, 90.0f);
    ImGui::SliderFloat("View Y",  &Vars::Visual::view_y_offset, -90.0f, 90.0f);
    ImGui::SliderFloat("Viewl Z", &Vars::Visual::view_z_offset, -90.0f, 90.0f);
    ImGui::Separator();
    ImGui::SliderFloat("View Pitch", &Vars::Visual::view_pitch_offset, -180.0f, 180.0f);
    ImGui::SliderFloat("View Yaw",   &Vars::Visual::view_yaw_offset, -180.0f, 180.0f);
    ImGui::SliderFloat("View Roll",  &Vars::Visual::view_roll_offset, -180.0f, 180.0f);
    ImGui::Separator();
    ImGui::SliderFloat("Aspect Ratio", &Vars::Visual::aspect_ratio, 0.f, 30.f);
    ImGui::Separator();
    if (ImGui::Button("Thirdperson"))
        I::Input->CAM_ToThirdPerson();

    if (ImGui::Button("Firstperson"))
        I::Input->CAM_ToFirstPerson();

    ImGui::Separator();
    ImGui::Text("Color modulation");
    if (ImGui::SliderFloat("R", &Vars::Visual::modluation_r, 0.f, 1.f) ||
        ImGui::SliderFloat("G", &Vars::Visual::modluation_g, 0.f, 1.f) ||
        ImGui::SliderFloat("B", &Vars::Visual::modluation_b, 0.f, 1.f)
        ) 
    {
        for (MaterialHandle_t i = I::MaterialSystem->FirstMaterial(); i != I::MaterialSystem->InvalidMaterial(); i = I::MaterialSystem->NextMaterial(i))
        {
            IMaterial* pMaterial = I::MaterialSystem->GetMaterial(i);

            if (!pMaterial || pMaterial->IsErrorMaterial())
                continue;

            auto name = std::string_view(pMaterial->GetName());
            auto group = std::string_view(pMaterial->GetTextureGroupName());

            if (!group.starts_with("World") && !group.starts_with("Model") && !group.starts_with("SkyBox") && !group.starts_with("ClientEffect") && !group.starts_with("Decal"))
                continue;

  

            I::Cvar->FindVar("r_colorstaticprops")->SetValue(1);
                //pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, false); // walls were translucent for me for some odd reason, probably p100 codenz :/
            pMaterial->ColorModulate(Vars::Visual::modluation_r, Vars::Visual::modluation_g, Vars::Visual::modluation_b);
        }
    }
}