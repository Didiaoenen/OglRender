
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
    mTargetChanged += [this]() { SetTarget(mAssetSelector->mContent); };

    CreateHeaderButtons();

    mHeaderSeparator = &CreateWidget<UI::UI_Separator>();
    mHeaderSeparator->mEnabled = false;

    CreateAssetSelector();

    mSettings = &CreateWidget<UI::UI_GroupCollapsable>("Settings");
    mSettingsColumns = &mSettings->CreateWidget<UI::UI_Columns<2>>();
    mSettingsColumns->mWidths[0] = 150;

    mInfo = &CreateWidget<UI::UI_GroupCollapsable>("Info");
    mInfoColumns = &mInfo->CreateWidget<UI::UI_Columns<2>>();
    mInfoColumns->mWidths[0] = 150;

    mSettings->mEnabled = mInfo->mEnabled = false;
}

void Editor::Editor_AssetProperties::SetTarget(const std::string& pPath)
{
    mResource = pPath == "" ? pPath : EDITOR_EXEC(GetResourcePath(pPath));

    if (mAssetSelector)
    {
        mAssetSelector->mContent = mResource;
    }

    Refresh();
}

void Editor::Editor_AssetProperties::Refresh()
{
    mMetadata.reset(new Tools::Tools_IniFile(EDITOR_EXEC(GetRealPath(mResource)) + ".meta"));

    CreateSettings();
    CreateInfo();

    mApplyButton->mEnabled = mSettings->mEnabled;
    mResetButton->mEnabled = mSettings->mEnabled;
    mRevertButton->mEnabled = mSettings->mEnabled;

    switch (Tools::Tools_PathParser::GetFileType(mResource))
    {
        case Tools::Tools_PathParser::EFileType::MODEL:
        case Tools::Tools_PathParser::EFileType::TEXTURE:
        case Tools::Tools_PathParser::EFileType::MATERIAL:
            mPreviewButton->mEnabled = true;
            break;
        default:
            mPreviewButton->mEnabled = false;
            break;
    }

    mHeaderSeparator->mEnabled = mApplyButton->mEnabled || mResetButton->mEnabled || mRevertButton->mEnabled || mPreviewButton->mEnabled;
    mHeaderLineBreak->mEnabled = mHeaderSeparator->mEnabled;
}

void Editor::Editor_AssetProperties::Preview()
{
    auto& assetView = EDITOR_PANEL(Editor_AssetView, "Asset");

    const auto fileType = Tools::Tools_PathParser::GetFileType(mResource);

    if (fileType == Tools::Tools_PathParser::EFileType::MODEL)
    {
        if (auto resource = OVSERVICE(Core::Core_ModelManager).GetResource(mResource))
        {
            assetView.SetResource(resource);
        }
    }
    else if (fileType == Tools::Tools_PathParser::EFileType::TEXTURE)
    {
        if (auto resource = OVSERVICE(Core::Core_TextureManager).GetResource(mResource))
        {
            assetView.SetResource(resource);
        }
    }

    assetView.Open();
}

void Editor::Editor_AssetProperties::CreateHeaderButtons()
{
    mApplyButton = &CreateWidget<UI::UI_Button>("Apply");
    mApplyButton->mIdleBackgroundColor = { 0.0f, 0.5f, 0.0f, 1.f };
    mApplyButton->mEnabled = false;
    mApplyButton->mLineBreak = false;
    mApplyButton->mClickedEvent += std::bind(&Editor_AssetProperties::Apply, this);

    mRevertButton = &CreateWidget<UI::UI_Button>("Revert");
    mRevertButton->mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
    mRevertButton->mEnabled = false;
    mRevertButton->mLineBreak = false;
    mRevertButton->mClickedEvent += std::bind(&Editor_AssetProperties::SetTarget, this, mResource);

    mPreviewButton = &CreateWidget<UI::UI_Button>("Preview");
    mPreviewButton->mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
    mPreviewButton->mEnabled = false;
    mPreviewButton->mLineBreak = false;
    mPreviewButton->mClickedEvent += std::bind(&Editor_AssetProperties::Preview, this);

    mResetButton = &CreateWidget<UI::UI_Button>("Reset to default");
    mResetButton->mIdleBackgroundColor = { 0.5f, 0.0f, 0.0f, 1.f };
    mResetButton->mEnabled = false;
    mResetButton->mLineBreak = false;
    mResetButton->mClickedEvent += [this]
        {
            mMetadata->RemoveAll();
            CreateSettings();
        };

    mHeaderLineBreak = &CreateWidget<UI::UI_NewLine>();
    mHeaderLineBreak->mEnabled = false;
}

void Editor::Editor_AssetProperties::CreateAssetSelector()
{
    auto& columns = CreateWidget<UI::UI_Columns<2>>();
    columns.mWidths[0] = 150;
    mAssetSelector = &Core::Core_GUIDrawer::DrawAsset(columns, "Target", mResource, &mTargetChanged);
}

void Editor::Editor_AssetProperties::CreateSettings()
{
    mSettingsColumns->RemoveAllWidgets();

    const auto fileType = Tools::Tools_PathParser::GetFileType(mResource);

    mSettings->mEnabled = true;

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
        mSettings->mEnabled = false;
    }
}

void Editor::Editor_AssetProperties::CreateInfo()
{
    const auto realPath = EDITOR_EXEC(GetRealPath(mResource));

    mInfoColumns->RemoveAllWidgets();

    if (std::filesystem::exists(realPath))
    {
        mInfo->mEnabled = true;

        Core::Core_GUIDrawer::CreateTitle(*mInfoColumns, "Path");
        mInfoColumns->CreateWidget<UI::UI_Text>(realPath);

        Core::Core_GUIDrawer::CreateTitle(*mInfoColumns, "Size");
        const auto [size, unit] = Tools::Tools_SizeConverter::ConvertToOptimalUnit(static_cast<float>(std::filesystem::file_size(realPath)), Tools::Tools_SizeConverter::ESizeUnit::BYTE);
        mInfoColumns->CreateWidget<UI::UI_Text>(std::to_string(size) + " " + Tools::Tools_SizeConverter::UnitToString(unit));

        Core::Core_GUIDrawer::CreateTitle(*mInfoColumns, "Metadata");
        mInfoColumns->CreateWidget<UI::UI_Text>(std::filesystem::exists(realPath + ".meta") ? "Yes" : "No");
    }
    else
    {
        mInfo->mEnabled = false;
    }
}

#define MODEL_FLAG_ENTRY(setting) Core::Core_GUIDrawer::DrawBoolean(*mSettingsColumns, setting, [&]() { return mMetadata->Get<bool>(setting); }, [&](bool value) { mMetadata->Set<bool>(setting, value); })

void Editor::Editor_AssetProperties::CreateModelSettings()
{
    mMetadata->Add("CALC_TANGENT_SPACE", true);
    mMetadata->Add("JOIN_IDENTICAL_VERTICES", true);
    mMetadata->Add("MAKE_LEFT_HANDED", false);
    mMetadata->Add("TRIANGULATE", true);
    mMetadata->Add("REMOVE_COMPONENT", false);
    mMetadata->Add("GEN_NORMALS", false);
    mMetadata->Add("GEN_SMOOTH_NORMALS", true);
    mMetadata->Add("SPLIT_LARGE_MESHES", false);
    mMetadata->Add("PRE_TRANSFORM_VERTICES", true);
    mMetadata->Add("LIMIT_BONE_WEIGHTS", false);
    mMetadata->Add("VALIDATE_DATA_STRUCTURE", false);
    mMetadata->Add("IMPROVE_CACHE_LOCALITY", true);
    mMetadata->Add("REMOVE_REDUNDANT_MATERIALS", false);
    mMetadata->Add("FIX_INFACING_NORMALS", false);
    mMetadata->Add("SORT_BY_PTYPE", false);
    mMetadata->Add("FIND_DEGENERATES", false);
    mMetadata->Add("FIND_INVALID_DATA", true);
    mMetadata->Add("GEN_UV_COORDS", true);
    mMetadata->Add("TRANSFORM_UV_COORDS", false);
    mMetadata->Add("FIND_INSTANCES", true);
    mMetadata->Add("OPTIMIZE_MESHES", true);
    mMetadata->Add("OPTIMIZE_GRAPH", true);
    mMetadata->Add("FLIP_UVS", false);
    mMetadata->Add("FLIP_WINDING_ORDER", false);
    mMetadata->Add("SPLIT_BY_BONE_COUNT", false);
    mMetadata->Add("DEBONE", true);
    mMetadata->Add("GLOBAL_SCALE", true);
    mMetadata->Add("EMBED_TEXTURES", false);
    mMetadata->Add("FORCE_GEN_NORMALS", false);
    mMetadata->Add("DROP_NORMALS", false);
    mMetadata->Add("GEN_BOUNDING_BOXES", false);

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
    mMetadata->Add("MIN_FILTER", static_cast<int>(Render::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
    mMetadata->Add("MAG_FILTER", static_cast<int>(Render::ETextureFilteringMode::LINEAR));
    mMetadata->Add("ENABLE_MIPMAPPING", true);

    std::map<int, std::string> filteringModes
    {
        {0x2600, "NEAREST"},
        {0x2601, "LINEAR"},
        {0x2700, "NEAREST_MIPMAP_NEAREST"},
        {0x2703, "LINEAR_MIPMAP_LINEAR"},
        {0x2701, "LINEAR_MIPMAP_NEAREST"},
        {0x2702, "NEAREST_MIPMAP_LINEAR"}
    };

    Core::Core_GUIDrawer::CreateTitle(*mSettingsColumns, "MIN_FILTER");
    auto& minFilter = mSettingsColumns->CreateWidget<UI::UI_ComboBox>(mMetadata->Get<int>("MIN_FILTER"));
    minFilter.mChoices = filteringModes;
    minFilter.mValueChangedEvent += [this](int p_choice)
        {
            mMetadata->Set("MIN_FILTER", p_choice);
        };

    Core::Core_GUIDrawer::CreateTitle(*mSettingsColumns, "MAG_FILTER");
    auto& magFilter = mSettingsColumns->CreateWidget<UI::UI_ComboBox>(mMetadata->Get<int>("MAG_FILTER"));
    magFilter.mChoices = filteringModes;
    magFilter.mValueChangedEvent += [this](int p_choice)
        {
            mMetadata->Set("MAG_FILTER", p_choice);
        };

    Core::Core_GUIDrawer::DrawBoolean(*mSettingsColumns, "ENABLE_MIPMAPPING", [&]() { return mMetadata->Get<bool>("ENABLE_MIPMAPPING"); }, [&](bool value) { mMetadata->Set<bool>("ENABLE_MIPMAPPING", value); });
}

void Editor::Editor_AssetProperties::Apply()
{
    mMetadata->Rewrite();

    const auto resourcePath = EDITOR_EXEC(GetResourcePath(mResource));
    const auto fileType = Tools::Tools_PathParser::GetFileType(mResource);

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
