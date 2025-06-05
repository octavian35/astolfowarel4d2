#include "font_awesome_5.h"
#include "CMenu.h"
#include "../Vars.h"

static int tab = 0;

auto s = ImVec2{};
auto p = ImVec2{};
auto MenuSize = ImVec2{ 784, 522 };

#define FixSlider ImGui::PushItemWidth(ImGui::GetCurrentWindow()->Size.x - 8)

ImVec4 to_vec4(float r, float g, float b, float a)
{
    return ImVec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

ImVec4 mColor(Color color) {
    return to_vec4(color.r(), color.g(), color.b(),color.a());
}

Color vColor(ImVec4 color) {
    return {
        (byte)(color.x * 256.0f > 255 ? 255 : color.x * 256.0f),
        (byte)(color.y * 256.0f > 255 ? 255 : color.y * 256.0f),
        (byte)(color.z * 256.0f > 255 ? 255 : color.z * 256.0f),
        (byte)(color.w * 256.0f > 255 ? 255 : color.w * 256.0f)
    };
}

void ColorPicker(const char* label, Color& color, bool alpha = true) {
    ImVec4 FUCKOFF = mColor(color);
    ImGui::PushItemWidth(150);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
    if (alpha) {
        if (ImGui::ColorEdit4(label, &FUCKOFF.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreview)) {
            color = vColor(FUCKOFF);
        }
    }
    else {
        if (ImGui::ColorEdit3(label, &FUCKOFF.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
            color = vColor(FUCKOFF);
        }
    }
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
}

void ColorPicker2(const char* label, Color& color, bool alpha = true) {
    ImVec4 FUCKOFF = mColor(color);
    ImGui::PushItemWidth(150);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
    if (alpha) {
        if (ImGui::ColorEdit4(label, &FUCKOFF.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview)) {
            color = vColor(FUCKOFF);
        }
    }
    else {
        if (ImGui::ColorEdit3(label, &FUCKOFF.x, ImGuiColorEditFlags_NoInputs)) {
            color = vColor(FUCKOFF);
        }
    }
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
}


auto MenuCol = mColor(Vars::Menu::Colors::WidgetActive); // Getting the menu color for "CustomStyle()"
auto ShadowCol = mColor(Vars::Menu::Colors::ShadowColor); // Getting the Shadow color for "CustomStyle()"
//         const ImU32 colorStart = ImGui::GetColorU32(ImVec4(240.0f / 255.0f, 137.0f / 255.0f, 229.0f / 255.0f, 1.0f)); // #f089e5

void CustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Border] = ImVec4(0.24f, 0.24f, 0.24f, 0.33f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.07f, 0.08f, 0.09f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(240.0f / 255.0f, 137.0f / 255.0f, 229.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.07f, 0.08f, 0.09f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05, 0.05, 0.05, 1.00);
    style.Colors[ImGuiCol_ScrollbarGrab] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ScrollbarGrabActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_FrameBgHovered] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_FrameBgActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_CheckMark] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_SliderGrab] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_SliderGrabActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_Header] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_HeaderHovered] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_HeaderActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_SeparatorHovered] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_SeparatorActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ResizeGrip] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ResizeGripHovered] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ResizeGripActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_Tab] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_TabActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_TabUnfocusedActive] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_DragDropTarget] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_NavHighlight] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_WindowShadow] = ImVec4(240.0f / 255.0f, 137.0f / 255.0f, 229.0f / 255.0f, 1.0f);

    style.FrameBorderSize = 1.0f;
    style.WindowShadowSize = Vars::Menu::ShadowSize;
}


bool ClickText(const char* label) {
    const auto id = ImGui::GetID(label);
    ImGui::PushID(label);
    ImGui::Text(label);
    if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0]) {
        return true;
    }
    ImGui::PopID();
    return false;
}

bool InputKeybind(const char* label, CVar<int>& output, bool bAllowNone = true)
{
    bool active = false;

    auto VK2STR = [&](const short key) -> const char* {
        switch (key) {
        case VK_LBUTTON: return "M1";
        case VK_RBUTTON: return "M2";
        case VK_MBUTTON: return "M3";
        case VK_XBUTTON1: return "M4";
        case VK_XBUTTON2: return "M5";
        case VK_SPACE: return "SPC";
        case 0x0: return "None";
        case VK_A: return "A";
        case VK_B: return "B";
        case VK_C: return "C";
        case VK_D: return "D";
        case VK_E: return "E";
        case VK_F: return "F";
        case VK_G: return "G";
        case VK_H: return "H";
        case VK_I: return "J";
        case VK_J: return "I";
        case VK_K: return "K";
        case VK_L: return "L";
        case VK_M: return "M";
        case VK_N: return "N";
        case VK_O: return "O";
        case VK_P: return "P";
        case VK_Q: return "Q";
        case VK_R: return "R";
        case VK_S: return "S";
        case VK_T: return "T";
        case VK_U: return "U";
        case VK_V: return "V";
        case VK_W: return "W";
        case VK_X: return "X";
        case VK_Y: return "Y";
        case VK_Z: return "Z";
        case VK_0: return "0";
        case VK_1: return "1";
        case VK_2: return "2";
        case VK_3: return "3";
        case VK_4: return "4";
        case VK_5: return "5";
        case VK_6: return "6";
        case VK_7: return "7";
        case VK_8: return "8";
        case VK_9: return "9";
        case VK_ESCAPE: return "ESC";
        case VK_SHIFT: return "SHFT";
        case VK_LSHIFT: return "SHFT";
        case VK_RSHIFT: return "SHFT";
        case VK_CONTROL: return "CTRL";
        case VK_MENU: return "ALT";
        default: break;
        }

        WCHAR output[16] = { L"\0" };
        if (const int result = GetKeyNameTextW(MapVirtualKeyW(key, MAPVK_VK_TO_VSC) << 16, output, 16)) {
            char outputt[128];
            sprintf(outputt, "%ws", output);
            return outputt;
        }

        return "VK2STR_FAILED";
    };

    auto labell = VK2STR(output.m_Var);

    const auto id = ImGui::GetID(label);
    ImGui::PushID(label);

    if (ImGui::GetActiveID() == id) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
        ImGui::Button("...", ImVec2(45, 17));
        ImGui::PopStyleColor();

        static float time = I::EngineClient->GetTimescale();
        float elapsed = I::EngineClient->GetTimescale() - time;
        static CVar<int>* curr = nullptr, * prevv = curr;
        if (curr != prevv) {
            time = I::EngineClient->GetTimescale();
            prevv = curr;
        }

        if (curr == nullptr) {
            for (short n = 0; n < 256; n++) {
                if ((n > 0x0 && n < 0x7) || (n > L'A' - 1 && n < L'Z' + 1) || (n > L'0' - 1 && n < L'9' + 1) || n == VK_LSHIFT || n == VK_RSHIFT || n == VK_SHIFT || n == VK_ESCAPE || n == VK_HOME || n == VK_CONTROL || n == VK_MENU) {
                    if ((!ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0])) {
                        ImGui::ClearActiveID();
                        break;
                    }
                    if (GetAsyncKeyState(n) & 0x8000)
                    {
                        if (n == VK_HOME || n == VK_INSERT) {
                            break;
                        }

                        if (n == VK_ESCAPE && bAllowNone) {
                            ImGui::ClearActiveID();
                            output.m_Var = 0x0;
                            break;
                        }

                        output.m_Var = n;
                        ImGui::ClearActiveID();
                        break;
                    }
                } //loop
            }
        }

        if (curr != prevv) {
            time = I::EngineClient->GetTimescale();
            prevv = curr;
        }

        ImGui::GetCurrentContext()->ActiveIdAllowOverlap = true;

        if ((!ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0]))
            ImGui::ClearActiveID();
    }
    else if (ImGui::Button(VK2STR(output.m_Var), ImVec2(45, 17))) {
        ImGui::SetActiveID(id, ImGui::GetCurrentWindow());
    }

    ImGui::PopID();

    return true;
}

void combo(const char* label, int* current_item, const char* const* items, int items_count, int popup_max_height_in_items = -1) {
    ImGui::PushItemWidth(100);
    ImGui::Combo(label, current_item, items, items_count, popup_max_height_in_items);
    ImGui::PopItemWidth();
}

ImVec4 clear_color = ImVec4(0.51f, 0.55f, 0.57f, 1.00f);
static bool one = true;
static bool two = false;
static int sl = 0;

void AlignToRight(int lul) { // My friend told me that you could do it with a macro but fuck it
    ImGui::SameLine(ImGui::GetContentRegionMax().x - lul);
    ImGui::SetNextItemWidth(lul);
}

void AimbotTab() {
    {
        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(7);
        ImGui::MenuChild(_("Aimbot"), ImVec2(250, 190), false, ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::Checkbox(_("Enabled"), &Vars::Hitscan::bEnable);
            ImGui::Checkbox(_("AutoShoot"), &Vars::Hitscan::bAutoShoot);
            AlignToRight(50);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(MenuCol.x / 1.5, MenuCol.y / 1.5, MenuCol.z / 1.5, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(MenuCol.x, MenuCol.y, MenuCol.z, 255));
            InputKeybind(_("Aim key"), Vars::Hitscan::AimKey);
            ImGui::PopStyleColor(3);
            FixSlider;
            ImGui::SliderInt("Aimbot FOV", &Vars::Hitscan::iFov, 0, 180);
            ImGui::Checkbox(_("Silent"), &Vars::Hitscan::bSilentAim);
            ImGui::Checkbox(_("Circle FOV"), &Vars::Hitscan::AimFovCircle);
            AlignToRight(23);
            ColorPicker(_("Circle FOV Color"), Vars::Hitscan::AimFov);
            
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }

    {// last right

        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(517);
        ImGui::MenuChild(_("Ignores"), ImVec2(260, 186), false, ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::Checkbox(_("Ignore Friends"), &Vars::Ignore::IgnoreFriends);
            ImGui::Checkbox(_("Ignore Witch Until Startled"), &Vars::Ignore::IgnoreWitchUntilStartled);
            ImGui::Checkbox(_("Ignore Teammates"), &Vars::Ignore::IgnoreTeammates);
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }

    {//right
        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(262);
        ImGui::MenuChild(_("Compensation"), ImVec2(250, 190), false, ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::Checkbox(_("NoSpread"), &Vars::Removals::NoSpread);
            ImGui::Checkbox(_("NoRecoil"), &Vars::Removals::NoRecoil);
            ImGui::Checkbox(_("No Visual Recoil"), &Vars::Removals::NoVisualRecoil);
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }
}
/* todo make ESP more customizable */

void ESPTab()
{
    {//left upper

        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(7);
        ImGui::MenuChild(_("ESP"), ImVec2(250, 230), false, ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::Checkbox(_("Enabled"), &Vars::ESP::Enabled);
            ImGui::Checkbox(_("Box"), &Vars::ESP::Box);
            ImGui::Checkbox(_("Healthbar"), &Vars::ESP::Healthbar);
            ImGui::Checkbox(_("Name"), &Vars::ESP::Name);
            //ImGui::Checkbox(_("Local ESP"), &Vars::ESP::LocalESP);
            ColorPicker(_("Player Color"), Vars::ESP::PlayerColor);
            ColorPicker(_("Player Infected Color"), Vars::ESP::PlayerInfectedColor);
            ColorPicker(_("Infected Color"), Vars::ESP::InfectedColor);
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }
    { // right
        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(262);
        ImGui::MenuChild(_("Chams"), ImVec2(250, 200), false, ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::Checkbox(_("Players"), &Vars::Chams::Players);
            AlignToRight(23);
            ColorPicker(_("Player Color"), Vars::Chams::PlayerColor);
            ImGui::Text("Player Infected Color");
            AlignToRight(23);
            ColorPicker(_("Player Infected Color"), Vars::Chams::PlayerInfectedColor);
            ImGui::Checkbox(_("Infected"), &Vars::Chams::Infected);
            AlignToRight(23);
            ColorPicker(_("Infected Color"), Vars::Chams::InfectedColor);
            
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }
    {// last right

        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(517);
        ImGui::MenuChild(_("Visual Removals"), ImVec2(250, 200), false, ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::Checkbox("Boomer Spit", &Vars::Removals::BoomerVisual);
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }
}

void MiscTab()
{
    {//left upper
        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(7);
        ImGui::MenuChild(_("General"), ImVec2(300, 190), false, ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::Checkbox(_("BunnyHop"), &Vars::Misc::Bunnyhop);
            ImGui::Checkbox(_("Directional Strafe"), &Vars::Misc::AutoStrafe);
            ImGui::Checkbox(_("AutoShove"), &Vars::Misc::AutoShove);
            ImGui::Checkbox(_("Disable Interpolation"), &Vars::Misc::DisableInterp);
            ImGui::Checkbox(_("ThirdPerson"), &Vars::Misc::ThirdPerson);
            AlignToRight(50);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(MenuCol.x / 1.5, MenuCol.y / 1.5, MenuCol.z / 1.5, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(MenuCol.x, MenuCol.y, MenuCol.z, 255));
            InputKeybind(_("ThirdPerson Key"), Vars::Misc::ThirdPersonKey);
            ImGui::PopStyleColor(3);
            FixSlider;
            ImGui::SliderInt("ThirdPerson Distance Value", &Vars::Misc::ThirdPersonDistance, 0, 180);
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }

    {//right
        ImGui::SetCursorPosY(42);
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(312);
        ImGui::MenuChild(_("Exploits"), ImVec2(300, 200), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::Checkbox(_("SpeedHack"), &Vars::Exploits::Speedhack);
            AlignToRight(50);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(MenuCol.x / 1.5, MenuCol.y / 1.5, MenuCol.z / 1.5, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(MenuCol.x, MenuCol.y, MenuCol.z, 255));
            InputKeybind(_("SpeedHack Key"), Vars::Exploits::Key);
            ImGui::PopStyleColor(3);
            FixSlider;
            ImGui::SliderInt("SpeedHack Value", &Vars::Exploits::SpeedHackValue, 0, 66);
            ImGui::Checkbox(_("Sequence Freezing"), &Vars::Misc::SequenceFreezing);
            AlignToRight(50);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(MenuCol.x / 1.5, MenuCol.y / 1.5, MenuCol.z / 1.5, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(MenuCol.x, MenuCol.y, MenuCol.z, 255));
            InputKeybind(_("Sequence Freezing Key"), Vars::Misc::Key);
            ImGui::PopStyleColor(3);
            FixSlider;
            ImGui::SliderInt("Sequence Freezing Value", &Vars::Misc::SequenceFreezingValue, 0, 180);
            ImGui::Checkbox(_("Teleport"), &Vars::Misc::Teleport);
            AlignToRight(50);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(MenuCol.x / 1.5, MenuCol.y / 1.5, MenuCol.z / 1.5, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(MenuCol.x, MenuCol.y, MenuCol.z, 255));
            InputKeybind(_("Teleport Key"), Vars::Misc::TeleportKey);
            ImGui::PopStyleColor(3);
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }
}
ImU32 LerpColor(ImU32 col1, ImU32 col2, float t) {
    ImVec4 c1 = ImGui::ColorConvertU32ToFloat4(col1);
    ImVec4 c2 = ImGui::ColorConvertU32ToFloat4(col2);
    ImVec4 result = {
        c1.x + (c2.x - c1.x) * t,
        c1.y + (c2.y - c1.y) * t,
        c1.z + (c2.z - c1.z) * t,
        c1.w + (c2.w - c1.w) * t
    };
    return ImGui::ColorConvertFloat4ToU32(result);
}
void Handle()
{
    if (!g_Menu.menuOpen && ImGui::GetStyle().Alpha > 0.f)
    {
        float fc = 255.f / 0.2f * ImGui::GetIO().DeltaTime;
        ImGui::GetStyle().Alpha = std::clamp(ImGui::GetStyle().Alpha - fc / 255.f, 0.f, 0.9f);
    }

    if (g_Menu.menuOpen && ImGui::GetStyle().Alpha < 1.f)
    {
        float fc = 255.f / 0.2f * ImGui::GetIO().DeltaTime;
        ImGui::GetStyle().Alpha = std::clamp(ImGui::GetStyle().Alpha + fc / 255.f, 0.f, 0.9f);
    }
}
void CMenu::Render(IDirect3DDevice9* pDevice) {
    static bool bInitImGui = false;
    static bool bColumnsWidthened = false;
    bool modified_custom_style = false;

    //fix drawing without calling engine functons/cl_showpos
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    //removes the source engine color correction
    pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    static LPDIRECT3DTEXTURE9 lg = nullptr;
    if (!bInitImGui) {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(FindWindowW(_(L"Valve001"), 0));
        ImGui_ImplDX9_Init(pDevice);

        auto& io = ImGui::GetIO();
        io.IniFilename = NULL;
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableKeyboard;
        ImFontConfig font_config;
        font_config.OversampleH = 1;
        font_config.OversampleV = 1;
        font_config.PixelSnapH = 1;
        font_config.FontDataOwnedByAtlas = false;

        static const ImWchar ranges[] =
        {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x0400, 0x044F, // Cyrillic
            0,
        };
        name = io.Fonts->AddFontFromMemoryTTF((void*)MuseoFont, sizeof(MuseoFont), 19.0f, &font_config, ranges);
        font = io.Fonts->AddFontFromMemoryTTF((void*)MuseoFont, sizeof(MuseoFont), 13.0f, &font_config, ranges);

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.FontDataOwnedByAtlas = false;
        io.Fonts->AddFontFromMemoryTTF((void*)fa_solid_900, sizeof(fa_solid_900), 14.0f, &icons_config, icons_ranges);
        bInitImGui = true;
    }

    if (GetAsyncKeyState(VK_INSERT) & 1) { // Can we please fix this in WndProcHook...? :(
        I::VGuiSurface->SetCursorAlwaysVisible(menuOpen = !menuOpen);
        g_Menu.flTimeOnChange = I::EngineClient->GetTimescale();
    }

    g_Menu.m_flFadeElapsed = I::EngineClient->GetTimescale() - g_Menu.flTimeOnChange;

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    ImGui::GetIO().MouseDrawCursor = menuOpen;
    Handle();
    if (menuOpen)
    {
        const ImU32 colorStart = ImGui::GetColorU32(ImVec4(240.0f / 255.0f, 137.0f / 255.0f, 229.0f / 255.0f, 1.0f)); // #f089e5

        //MenuCol = colorStart;
        //ShadowCol = colorStart;
        CustomStyle();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
        ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(MenuSize));

        ImGui::Begin(_("##GUI"), NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
        {
            {
                MenuCol = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];
                ShadowCol = ImGui::GetStyle().Colors[ImGuiCol_WindowShadow];
                s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2);
                p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y); auto draw = ImGui::GetWindowDrawList();
                ImGui::GetBackgroundDrawList()->AddRect({ p.x,p.y }, { p.x + s.x, p.y + s.y }, ImColor(15, 17, 19, 255));
                draw->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y + 40), ImColor(15, 17, 19, 200), 5); // Titlebar
                draw->AddRectFilled(ImVec2(p.x, p.y + 40), ImVec2(p.x + s.x, p.y + s.y), ImColor(18, 20, 21, 200), 5, ImDrawCornerFlags_Bot); // Background

                ImGui::PushFont(name);
                draw->AddText(ImVec2(p.x + 36 / 2, p.y + 29 / 2), ImColor(255, 255, 255, 255), _("Catalyst.club"));
                //const ImU32 colorStart = ImGui::GetColorU32(ImVec4(240.0f / 255.0f, 137.0f / 255.0f, 229.0f / 255.0f, 1.0f)); // #f089e5
                //const ImU32 colorEnd = ImGui::GetColorU32(ImVec4(245.0f / 255.0f, 98.0f / 255.0f, 133.0f / 255.0f, 1.0f));  // #f56285
                draw->AddText(ImVec2(p.x + 36 / 2 + ImGui::CalcTextSize(_("Catalyst")).x, p.y + 29 / 2), colorStart, _(".club"));

                
                ImGui::PopFont();
            }
            {
                ImGui::PushFont(font);
                ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 71 * 3.3); // 6 = how much tabs, .7 = how much tabs + 0.1
                ImGui::BeginGroup();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
                if (ImGui::Tab(_(ICON_FA_CROSSHAIRS " Aimbot"), tab == 0))tab = 0; ImGui::SameLine();
                if (ImGui::Tab(_(ICON_FA_EYE " ESP"), tab == 1))tab = 1; ImGui::SameLine();
                //if (ImGui::Tab(_(ICON_FA_PALETTE " Visuals"), tab == 2))tab = 2; ImGui::SameLine();
                if (ImGui::Tab(_(ICON_FA_COGS " Misc"), tab == 2))tab = 2; ImGui::SameLine();
                //if (ImGui::Tab(_(ICON_FA_FILE " Config"), tab == 4))tab = 4; later.

                ImGui::PopStyleVar();
                ImGui::EndGroup();
                ImGui::PopFont();
            }
            ImGui::PushFont(font);
            {
                switch (tab) {
                case 0: {
                    AimbotTab();
                    break;
                }
                case 1: {
                    ESPTab();
                    break;
                }
                case 2: {
                    MiscTab();
                    break;
                }
                default: {
                    ImGui::SetCursorPosX(7);
                    ImGui::Text(_("How the fuck did you manage to get here?\nInvalid tab value : 5 / %d"), tab);
                }
                }
                ImGui::PopFont();
            }
            ImGui::End();
        }
        ImGui::PopStyleVar();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
}