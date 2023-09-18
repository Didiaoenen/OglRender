
#include <filesystem>

#include <OglTools/Tools_PathParser.h>
#include <OglTools/Tools_SizeConverter.h>

#include <OglCore/Core_GUIDrawer.h>
#include <OglCore/Core_ModelManager.h>
#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_TextureManager.h>

#include <OglUI/UI_Group.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_NewLine.h>
#include <OglUI/UI_ComboBox.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_GroupCollapsable.h>

#include "Editor_AssetView.h"
#include "Editor_EditorActions.h"
#include "Editor_AssetProperties.h"

Editor::Editor_AssetProperties::Editor_AssetProperties(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	UI::UI_PanelWindow(pTitle, pOpened, pWindowSettings)
{
    m_targetChanged += [this]() { SetTarget(m_assetSelector->mContent); };

    CreateHeaderButtons();

    m_headerSeparator = &CreateWidget<UI::UI_Separator>();
    m_headerSeparator->mEnabled = false;

    CreateAssetSelector();

    m_settings = &CreateWidget<UI::UI_GroupCollapsable>("Settings");
    m_settingsColumns = &m_settings->CreateWidget<UI::UI_Columns<2>>();
    m_settingsColumns->mWidths[0] = 150;

    m_info = &CreateWidget<UI::UI_GroupCollapsable>("Info");
    m_infoColumns = &m_info->CreateWidget<UI::UI_Columns<2>>();
    m_infoColumns->mWidths[0] = 150;

    m_settings->mEnabled = m_info->mEnabled = false;
}

void Editor::Editor_AssetProperties::SetTarget(const std::string& pPath)
{
    m_resource = pPath == "" ? pPath : EDITOR_EXEC(GetResourcePath(pPath));

    if (m_assetSelector)
    {
        m_assetSelector->mContent = m_resource;
    }

    Refresh();
}

void Editor::Editor_AssetProperties::Refresh()
{
    m_metadata.reset(new Tools::Tools_IniFile(EDITOR_EXEC(GetRealPath(m_resource)) + ".meta"));

    CreateSettings();
    CreateInfo();

    m_applyButton->mEnabled = m_settings->mEnabled;
    m_resetButton->mEnabled = m_settings->mEnabled;
    m_revertButton->mEnabled = m_settings->mEnabled;

    switch (Tools::Tools_PathParser::GetFileType(m_resource))
    {
        case Tools::Tools_PathParser::EFileType::MODEL:
        case Tools::Tools_PathParser::EFileType::TEXTURE:
        case Tools::Tools_PathParser::EFileType::MATERIAL:
            m_previewButton->mEnabled = true;
            break;
        default:
            m_previewButton->mEnabled = false;
            break;
    }

    m_headerSeparator->mEnabled = m_applyButton->mEnabled || m_resetButton->mEnabled || m_revertButton->mEnabled || m_previewButton->mEnabled;
    m_headerLineBreak->mEnabled = m_headerSeparator->mEnabled;
}

void Editor::Editor_AssetProperties::Preview()
{
    auto& assetView = EDITOR_PANEL(Editor_AssetView, "Asset View");

    const auto fileType = Tools::Tools_PathParser::GetFileType(m_resource);

    if (fileType == Tools::Tools_PathParser::EFileType::MODEL)
    {
        if (auto resource = OVSERVICE(Core::Core_ModelManager).GetResource(m_resource))
        {
            assetView.SetResource(resource);
        }
    }
    else if (fileType == Tools::Tools_PathParser::EFileType::TEXTURE)
    {
        if (auto resource = OVSERVICE(Core::Core_TextureManager).GetResource(m_resource))
        {
            assetView.SetResource(resource);
        }
    }

    assetView.Open();
}

void Editor::Editor_AssetProperties::CreateHeaderButtons()
{
    m_applyButton = &CreateWidget<UI::UI_Button>("Apply");
    m_applyButton->mIdleBackgroundColor = { 0.0f, 0.5f, 0.0f, 1.f };
    m_applyButton->mEnabled = false;
    m_applyButton->mLineBreak = false;
    m_applyButton->mClickedEvent += std::bind(&Editor_AssetProperties::Apply, this);

    m_revertButton = &CreateWidget<UI::UI_Button>("Revert");
    m_revertButton->mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
    m_revertButton->mEnabled = false;
    m_revertButton->mLineBreak = false;
    m_revertButton->mClickedEvent += std::bind(&Editor_AssetProperties::SetTarget, this, m_resource);

    m_previewButton = &CreateWidget<UI::UI_Button>("Preview");
    m_previewButton->mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
    m_previewButton->mEnabled = false;
    m_previewButton->mLineBreak = false;
    m_previewButton->mClickedEvent += std::bind(&Editor_AssetProperties::Preview, this);

    m_resetButton = &CreateWidget<UI::UI_Button>("Reset to default");
    m_resetButton->mIdleBackgroundColor = { 0.5f, 0.0f, 0.0f, 1.f };
    m_resetButton->mEnabled = false;
    m_resetButton->mLineBreak = false;
    m_resetButton->mClickedEvent += [this]
        {
            m_metadata->RemoveAll();
            CreateSettings();
        };

    m_headerLineBreak = &CreateWidget<UI::UI_NewLine>();
    m_headerLineBreak->mEnabled = false;
}

void Editor::Editor_AssetProperties::CreateAssetSelector()
{
    auto& columns = CreateWidget<UI::UI_Columns<2>>();
    columns.mWidths[0] = 150;
    m_assetSelector = &Core::Core_GUIDrawer::DrawAsset(columns, "Target", m_resource, &m_targetChanged);
}

void Editor::Editor_AssetProperties::CreateSettings()
{
    m_settingsColumns->RemoveAllWidgets();

    const auto fileType = Tools::Tools_PathParser::GetFileType(m_resource);

    m_settings->mEnabled = true;

    if (fileType == Tools::Tools_PathParser::EFileType::MODEL)
    {
        CreateModelSettings();
    }
    else if (fileType == Tools::Tools_PathParser::EFileType::TEXTURE)
    {
        CreateTextureSettings();
    }
    else
    {
        m_settings->mEnabled = false;
    }
}

void Editor::Editor_AssetProperties::CreateInfo()
{
    const auto realPath = EDITOR_EXEC(GetRealPath(m_resource));

    m_infoColumns->RemoveAllWidgets();

    if (std::filesystem::exists(realPath))
    {
        m_info->mEnabled = true;

        Core::Core_GUIDrawer::CreateTitle(*m_infoColumns, "Path");
        m_infoColumns->CreateWidget<UI::UI_Text>(realPath);

        Core::Core_GUIDrawer::CreateTitle(*m_infoColumns, "Size");
        const auto [size, unit] = Tools::Tools_SizeConverter::ConvertToOptimalUnit(static_cast<float>(std::filesystem::file_size(realPath)), Tools::Tools_SizeConverter::ESizeUnit::BYTE);
        m_infoColumns->CreateWidget<UI::UI_Text>(std::to_string(size) + " " + Tools::Tools_SizeConverter::UnitToString(unit));

        Core::Core_GUIDrawer::CreateTitle(*m_infoColumns, "Metadata");
        m_infoColumns->CreateWidget<UI::UI_Text>(std::filesystem::exists(realPath + ".meta") ? "Yes" : "No");
    }
    else
    {
        m_info->mEnabled = false;
    }
}

#define MODEL_FLAG_ENTRY(setting) Core::Core_GUIDrawer::DrawBoolean(*m_settingsColumns, setting, [&]() { return m_metadata->Get<bool>(setting); }, [&](bool value) { m_metadata->Set<bool>(setting, value); })

void Editor::Editor_AssetProperties::CreateModelSettings()
{
    m_metadata->Add("CALC_TANGENT_SPACE", true);
    m_metadata->Add("JOIN_IDENTICAL_VERTICES", true);
    m_metadata->Add("MAKE_LEFT_HANDED", false);
    m_metadata->Add("TRIANGULATE", true);
    m_metadata->Add("REMOVE_COMPONENT", false);
    m_metadata->Add("GEN_NORMALS", false);
    m_metadata->Add("GEN_SMOOTH_NORMALS", true);
    m_metadata->Add("SPLIT_LARGE_MESHES", false);
    m_metadata->Add("PRE_TRANSFORM_VERTICES", true);
    m_metadata->Add("LIMIT_BONE_WEIGHTS", false);
    m_metadata->Add("VALIDATE_DATA_STRUCTURE", false);
    m_metadata->Add("IMPROVE_CACHE_LOCALITY", true);
    m_metadata->Add("REMOVE_REDUNDANT_MATERIALS", false);
    m_metadata->Add("FIX_INFACING_NORMALS", false);
    m_metadata->Add("SORT_BY_PTYPE", false);
    m_metadata->Add("FIND_DEGENERATES", false);
    m_metadata->Add("FIND_INVALID_DATA", true);
    m_metadata->Add("GEN_UV_COORDS", true);
    m_metadata->Add("TRANSFORM_UV_COORDS", false);
    m_metadata->Add("FIND_INSTANCES", true);
    m_metadata->Add("OPTIMIZE_MESHES", true);
    m_metadata->Add("OPTIMIZE_GRAPH", true);
    m_metadata->Add("FLIP_UVS", false);
    m_metadata->Add("FLIP_WINDING_ORDER", false);
    m_metadata->Add("SPLIT_BY_BONE_COUNT", false);
    m_metadata->Add("DEBONE", true);
    m_metadata->Add("GLOBAL_SCALE", true);
    m_metadata->Add("EMBED_TEXTURES", false);
    m_metadata->Add("FORCE_GEN_NORMALS", false);
    m_metadata->Add("DROP_NORMALS", false);
    m_metadata->Add("GEN_BOUNDING_BOXES", false);

    MODEL_FLAG_ENTRY("CALC_TANGENT_SPACE");
    MODEL_FLAG_ENTRY("JOIN_IDENTICAL_VERTICES");
    MODEL_FLAG_ENTRY("MAKE_LEFT_HANDED");
    MODEL_FLAG_ENTRY("TRIANGULATE");
    MODEL_FLAG_ENTRY("REMOVE_COMPONENT");
    MODEL_FLAG_ENTRY("GEN_NORMALS");
    MODEL_FLAG_ENTRY("GEN_SMOOTH_NORMALS");
    MODEL_FLAG_ENTRY("SPLIT_LARGE_MESHES");
    MODEL_FLAG_ENTRY("PRE_TRANSFORM_VERTICES");
    MODEL_FLAG_ENTRY("LIMIT_BONE_WEIGHTS");
    MODEL_FLAG_ENTRY("VALIDATE_DATA_STRUCTURE");
    MODEL_FLAG_ENTRY("IMPROVE_CACHE_LOCALITY");
    MODEL_FLAG_ENTRY("REMOVE_REDUNDANT_MATERIALS");
    MODEL_FLAG_ENTRY("FIX_INFACING_NORMALS");
    MODEL_FLAG_ENTRY("SORT_BY_PTYPE");
    MODEL_FLAG_ENTRY("FIND_DEGENERATES");
    MODEL_FLAG_ENTRY("FIND_INVALID_DATA");
    MODEL_FLAG_ENTRY("GEN_UV_COORDS");
    MODEL_FLAG_ENTRY("TRANSFORM_UV_COORDS");
    MODEL_FLAG_ENTRY("FIND_INSTANCES");
    MODEL_FLAG_ENTRY("OPTIMIZE_MESHES");
    MODEL_FLAG_ENTRY("OPTIMIZE_GRAPH");
    MODEL_FLAG_ENTRY("FLIP_UVS");
    MODEL_FLAG_ENTRY("FLIP_WINDING_ORDER");
    MODEL_FLAG_ENTRY("SPLIT_BY_BONE_COUNT");
    MODEL_FLAG_ENTRY("DEBONE");
    MODEL_FLAG_ENTRY("GLOBAL_SCALE");
    MODEL_FLAG_ENTRY("EMBED_TEXTURES");
    MODEL_FLAG_ENTRY("FORCE_GEN_NORMALS");
    MODEL_FLAG_ENTRY("DROP_NORMALS");
    MODEL_FLAG_ENTRY("GEN_BOUNDING_BOXES");
}

void Editor::Editor_AssetProperties::CreateTextureSettings()
{
    m_metadata->Add("MIN_FILTER", static_cast<int>(Render::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
    m_metadata->Add("MAG_FILTER", static_cast<int>(Render::ETextureFilteringMode::LINEAR));
    m_metadata->Add("ENABLE_MIPMAPPING", true);

    std::map<int, std::string> filteringModes
    {
        {0x2600, "NEAREST"},
        {0x2601, "LINEAR"},
        {0x2700, "NEAREST_MIPMAP_NEAREST"},
        {0x2703, "LINEAR_MIPMAP_LINEAR"},
        {0x2701, "LINEAR_MIPMAP_NEAREST"},
        {0x2702, "NEAREST_MIPMAP_LINEAR"}
    };

    Core::Core_GUIDrawer::CreateTitle(*m_settingsColumns, "MIN_FILTER");
    auto& minFilter = m_settingsColumns->CreateWidget<UI::UI_ComboBox>(m_metadata->Get<int>("MIN_FILTER"));
    minFilter.mChoices = filteringModes;
    minFilter.mValueChangedEvent += [this](int p_choice)
        {
            m_metadata->Set("MIN_FILTER", p_choice);
        };

    Core::Core_GUIDrawer::CreateTitle(*m_settingsColumns, "MAG_FILTER");
    auto& magFilter = m_settingsColumns->CreateWidget<UI::UI_ComboBox>(m_metadata->Get<int>("MAG_FILTER"));
    magFilter.mChoices = filteringModes;
    magFilter.mValueChangedEvent += [this](int p_choice)
        {
            m_metadata->Set("MAG_FILTER", p_choice);
        };

    Core::Core_GUIDrawer::DrawBoolean(*m_settingsColumns, "ENABLE_MIPMAPPING", [&]() { return m_metadata->Get<bool>("ENABLE_MIPMAPPING"); }, [&](bool value) { m_metadata->Set<bool>("ENABLE_MIPMAPPING", value); });
}

void Editor::Editor_AssetProperties::Apply()
{
    m_metadata->Rewrite();

    const auto resourcePath = EDITOR_EXEC(GetResourcePath(m_resource));
    const auto fileType = Tools::Tools_PathParser::GetFileType(m_resource);

    if (fileType == Tools::Tools_PathParser::EFileType::MODEL)
    {
        auto& modelManager = OVSERVICE(Core::Core_ModelManager);
        if (modelManager.IsResourceRegistered(resourcePath))
        {
            modelManager.Core_AResourceManager::ReloadResource(resourcePath);
        }
    }
    else if (fileType == Tools::Tools_PathParser::EFileType::TEXTURE)
    {
        auto& textureManager = OVSERVICE(Core::Core_TextureManager);
        if (textureManager.IsResourceRegistered(resourcePath))
        {
            textureManager.Core_AResourceManager::ReloadResource(resourcePath);
        }
    }

    Refresh();
}
