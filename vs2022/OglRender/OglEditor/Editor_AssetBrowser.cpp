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

#define FILENAMES_CHARS Editor::Editor_AssetBrowser::__FILENAMES_CHARS

const std::string FILENAMES_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-_=+ 0123456789()[]";

std::string GetAssociatedMetaFile(const std::string& p_assetPath)
{
	return p_assetPath + ".meta";
}

void RenameAsset(const std::string& p_prev, const std::string& p_new)
{
	std::filesystem::rename(p_prev, p_new);

	if (const std::string previousMetaPath = GetAssociatedMetaFile(p_prev); std::filesystem::exists(previousMetaPath))
	{
		if (const std::string newMetaPath = GetAssociatedMetaFile(p_new); !std::filesystem::exists(newMetaPath))
		{
			std::filesystem::rename(previousMetaPath, newMetaPath);
		}
		else
		{
			//OVLOG_ERROR(newMetaPath + " is already existing, .meta creation failed");
		}
	}
}

void RemoveAsset(const std::string& p_toDelete)
{
	std::filesystem::remove(p_toDelete);

	if (const std::string metaPath = GetAssociatedMetaFile(p_toDelete); std::filesystem::exists(metaPath))
	{
		std::filesystem::remove(metaPath);
	}
}

Editor::Editor_AssetBrowser::Editor_AssetBrowser(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings, const std::string& p_engineAssetFolder, const std::string& p_projectAssetFolder, const std::string& p_projectScriptFolder) :
	UI::UI_PanelWindow(p_title, p_opened, p_windowSettings),
	m_engineAssetFolder(p_engineAssetFolder),
	m_projectAssetFolder(p_projectAssetFolder),
	m_projectScriptFolder(p_projectScriptFolder)
{
	if (!std::filesystem::exists(m_projectAssetFolder))
	{
		std::filesystem::create_directories(m_projectAssetFolder);

		Window::Window_MessageBox message
		(
			"Assets folder not found",
			"The \"Assets/\" folders hasn't been found in your project directory.\nIt has been automatically generated",
			Window::Window_MessageBox::EMessageType::WARNING,
			Window::Window_MessageBox::EButtonLayout::OK
		);
	}

	if (!std::filesystem::exists(m_projectScriptFolder))
	{
		std::filesystem::create_directories(m_projectScriptFolder);

		Window::Window_MessageBox message
		(
			"Scripts folder not found",
			"The \"Scripts/\" folders hasn't been found in your project directory.\nIt has been automatically generated",
			Window::Window_MessageBox::EMessageType::WARNING,
			Window::Window_MessageBox::EButtonLayout::OK
		);
	}

	auto& refreshButton = CreateWidget<UI::UI_Button>("Rescan assets");
	refreshButton.mClickedEvent += std::bind(&Editor_AssetBrowser::Refresh, this);
	refreshButton.mLineBreak = false;
	refreshButton.mIdleBackgroundColor = { 0.f, 0.5f, 0.0f, 1.f };

	auto& importButton = CreateWidget<UI::UI_Button>("Import asset");
	importButton.mClickedEvent += EDITOR_BIND(ImportAsset, m_projectAssetFolder);
	importButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };

	m_assetList = &CreateWidget<UI::UI_Group>();

	Fill();
}

void Editor::Editor_AssetBrowser::Fill()
{
	m_assetList->CreateWidget<UI::UI_Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_engineAssetFolder), true);
	m_assetList->CreateWidget<UI::UI_Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectAssetFolder), false);
	m_assetList->CreateWidget<UI::UI_Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectScriptFolder), false, false, true);
}

void Editor::Editor_AssetBrowser::Clear()
{
	m_assetList->RemoveAllWidgets();
}

void Editor::Editor_AssetBrowser::Refresh()
{
	Clear();
	Fill();
}

void Editor::Editor_AssetBrowser::ParseFolder(UI::UI_TreeNode& pRoot, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder)
{
	for (auto& item : std::filesystem::directory_iterator(p_directory))
	{
		if (item.is_directory())
		{
			ConsiderItem(&pRoot, item, p_isEngineItem, false, p_scriptFolder);
		}
	}

	for (auto& item : std::filesystem::directory_iterator(p_directory))
	{
		if (!item.is_directory())
		{
			ConsiderItem(&pRoot, item, p_isEngineItem, false, p_scriptFolder);
		}
	}
}

void Editor::Editor_AssetBrowser::ConsiderItem(UI::UI_TreeNode* pRoot, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen, bool p_scriptFolder)
{

}

