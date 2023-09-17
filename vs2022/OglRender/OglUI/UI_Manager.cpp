
#include "MaterialDesign.inl"
#include "IconsMaterialDesignIcons.h"

#include "UI_UIManager.h"

UI::UI_UIManager::UI_UIManager(GLFWwindow* pGlfwWindow, EStyle pStyle, const std::string& pGlslVersion)
{
	ImGui::CreateContext();

	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	EnableDocking(false);

	ApplyStyle(pStyle);

	ImGui_ImplGlfw_InitForOpenGL(pGlfwWindow, true);
	ImGui_ImplOpenGL3_Init(pGlslVersion.c_str());
}

UI::UI_UIManager::~UI_UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::UI_UIManager::ApplyStyle(EStyle pStyle)
{
    ImGuiStyle* style = &ImGui::GetStyle();

    switch (pStyle)
    {
        case EStyle::IM_CLASSIC_STYLE:	ImGui::StyleColorsClassic();	break;
        case EStyle::IM_DARK_STYLE:		ImGui::StyleColorsDark();		break;
        case EStyle::IM_LIGHT_STYLE:	ImGui::StyleColorsLight();		break;
    }

    if (pStyle == EStyle::DUNE_DARK)
    {
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 5.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 4.0f;
        style->ItemSpacing = ImVec2(12, 8);
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;

        style->WindowMenuButtonPosition = ImGuiDir_None;

        style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 0.88f);
        style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.3f, 0.3f, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
        style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

        style->Colors[ImGuiCol_Tab] = style->Colors[ImGuiCol_TabUnfocused];
    }
    else if (pStyle == EStyle::UUNITY_DARK)
    {
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 5.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 4.0f;
        style->ItemSpacing = ImVec2(12, 8);
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;
        style->TabRounding = 5.0f;

        style->WindowMenuButtonPosition = ImGuiDir_None;

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.275f, 0.275f, 0.275f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.000f, 0.434f, 0.878f, 1.000f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.000f, 0.434f, 0.878f, 1.000f);
        colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.000f, 0.439f, 0.878f, 0.824f);
        colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.19f, 0.53f, 0.78f, 0.22f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.47f, 0.94f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.47f, 0.94f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.197f, 0.197f, 0.197f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.197f, 0.197f, 0.197f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.188f, 0.529f, 0.780f, 1.000f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }
    else if (pStyle == EStyle::ALTERNATIVE_DARK)
    {
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 0.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 0.0f;
        style->ItemSpacing = ImVec2(12, 8);
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 0.0f;
        style->TabRounding = 0.0f;
        style->ChildRounding = 0.0f;
        style->PopupRounding = 0.0f;

        style->WindowBorderSize = 1.0f;
        style->FrameBorderSize = 0.0f;
        style->PopupBorderSize = 1.0f;

        style->WindowMenuButtonPosition = ImGuiDir_None;

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.27f, 0.27f, 0.29f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.32f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.42f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.53f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.47f, 0.39f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.44f, 0.47f, 0.59f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.20f, 0.22f, 0.39f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.44f, 0.44f, 0.47f, 0.39f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.91f, 0.62f, 0.00f, 0.78f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.12f, 1.00f, 0.12f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.12f, 1.00f, 0.12f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.91f, 0.62f, 0.00f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
}

bool UI::UI_UIManager::LoadFont(const std::string& pId, const std::string& pPath, float pFontSize)
{
    if (mFonts.find(pId) == mFonts.end())
    {
        auto& io = ImGui::GetIO();

        ImFontConfig iconsConfig1;
        iconsConfig1.MergeMode = false;
        iconsConfig1.PixelSnapH = true;
        iconsConfig1.OversampleH = 1;
        iconsConfig1.OversampleV = 1;
        iconsConfig1.GlyphMinAdvanceX = 4.0f;
        iconsConfig1.SizePixels = 12.0f;

        ImFont* fontInstance = io.Fonts->AddFontFromFileTTF(pPath.c_str(), pFontSize, &iconsConfig1);


        ImFontConfig iconsConfig2;
        iconsConfig2.MergeMode = true;
        iconsConfig2.PixelSnapH = true;
        iconsConfig2.GlyphOffset.y = 1.0f;
        iconsConfig2.OversampleH = 1;
        iconsConfig2.OversampleV = 1;
        iconsConfig2.GlyphMinAdvanceX = 4.0f;
        iconsConfig2.SizePixels = 12.0f;

        static const ImWchar iconsRanges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };

        io.Fonts->AddFontFromMemoryCompressedTTF(MaterialDesign_compressed_data, MaterialDesign_compressed_size, pFontSize, &iconsConfig2, iconsRanges);

        if (fontInstance)
        {
            mFonts[pId] = fontInstance;
            return true;
        }
    }

    return false;
}

bool UI::UI_UIManager::UnloadFont(const std::string& pId)
{
    if (mFonts.find(pId) != mFonts.end())
    {
        mFonts.erase(pId);
        return true;
    }

    return false;
}

bool UI::UI_UIManager::UseFont(const std::string& pId)
{
    auto foundFont = mFonts.find(pId);

    if (foundFont != mFonts.end())
    {
        ImGui::GetIO().FontDefault = foundFont->second;
        return true;
    }

    return false;
}

void UI::UI_UIManager::UseDefaultFont()
{
    ImGui::GetIO().FontDefault = nullptr;
}

void UI::UI_UIManager::EnableEditorLayoutSave(bool pValue)
{
    if (pValue)
    {
        ImGui::GetIO().IniFilename = mLayoutSaveFilename.c_str();
    }
    else
    {
        ImGui::GetIO().IniFilename = nullptr;
    }
}

bool UI::UI_UIManager::IsEditorLayoutSaveEnabled() const
{
    return ImGui::GetIO().IniFilename != nullptr;
}

void UI::UI_UIManager::SetEditorLayoutSaveFilename(const std::string& pFilename)
{
    mLayoutSaveFilename = pFilename;
    if (IsEditorLayoutSaveEnabled())
    {
        ImGui::GetIO().IniFilename = mLayoutSaveFilename.c_str();
    }
}

void UI::UI_UIManager::SetEditorLayoutAutosaveFrequency(float pFrequency)
{
    ImGui::GetIO().IniSavingRate = pFrequency;
}

float UI::UI_UIManager::GetEditorLayoutAutosaveFrequency(float pFrequeny)
{
    return ImGui::GetIO().IniSavingRate;
}

void UI::UI_UIManager::EnableDocking(bool pValue)
{
    mDockingState = pValue;

    if (pValue)
    {
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }
    else
    {
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
    }
}

void UI::UI_UIManager::ResetLayout(const std::string& pConfig) const
{
    ImGui::LoadIniSettingsFromDisk(pConfig.c_str());
}

bool UI::UI_UIManager::IsDockingEnabled() const
{
    return mDockingState;
}

void UI::UI_UIManager::SetCanvas(UI_Canvas& pCanvas)
{
    RemoveCanvas();

    mCurrentCanvas = &pCanvas;
}

void UI::UI_UIManager::RemoveCanvas()
{
    mCurrentCanvas = nullptr;
}

void UI::UI_UIManager::Render()
{
    if (mCurrentCanvas)
    {
        mCurrentCanvas->Draw();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void UI::UI_UIManager::PushCurrentFont()
{
}

void UI::UI_UIManager::PopCurrentFont()
{
}
