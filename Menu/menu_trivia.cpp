#include "pch.h"
#include "menu.h"
//#include "lua.h"

void draw_trivia() {
    static char buf[256]{};
    ImGui::InputText("Lua script path", buf, 255);
    buf[255] = '\0';

    std::u8string str((char8_t*)buf);
    if (ImGui::Button("Load Script")) {
        //Lua::load_file(buf);
    }

    ImGui::SameLine();
    if (ImGui::Button("Save Config(as a script)")) {

        //Lua::save_config(buf);
    }
    if (ImGui::Button("Print Buf")) {
        //std::println(str);
        //auto str = (const char*)u8"υσι υσι υσι";
        std::println("{}", (const char*)str.c_str());
    }
}