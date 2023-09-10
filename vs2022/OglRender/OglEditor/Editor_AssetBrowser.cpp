#include <fstream>
#include <iostream>

#include <OglUI/UI_Image.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_DDSource.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_TextClickable.h>
#include <OglUI/UI_ContextualMenu.h>

#include <OglWindow/Window_MessageBox.h>
#include <OglWindow/Window_SaveFileDialog.h>
#include <OglWindow/Window_OpenFileDialog.h>

#include <OglTools/Tools_SystemCalls.h>
#include <OglTools/Tools_PathParser.h>
#include <OglTools/Tools_String.h>

#include <OglCore/Core_ModelManager.h>
#include <OglCore/Core_ShaderManager.h>
#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_TextureManager.h>

#include "Editor_AssetView.h"
#include "Editor_AssetBrowser.h"
#include "Editor_EditorActions.h"
#include "Editor_MaterialEditor.h"
#include "Editor_AssetProperties.h"
#include "Editor_EditorResources.h"

Editor::Editor_AssetBrowser::Editor_AssetBrowser(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings, const std::string& p_engineAssetFolder, const std::string& p_projectAssetFolder, const std::string& p_projectScriptFolder)
{
}

void Editor::Editor_AssetBrowser::Fill()
{
}

void Editor::Editor_AssetBrowser::Clear()
{
}

void Editor::Editor_AssetBrowser::Refresh()
{
}

void Editor::Editor_AssetBrowser::ParseFolder(UI::UI_TreeNode& p_root, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder)
{
}

void Editor::Editor_AssetBrowser::ConsiderItem(UI::UI_TreeNode* p_root, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen, bool p_scriptFolder)
{
}

