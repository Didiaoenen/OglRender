#pragma once

#include <queue>
#include <filesystem>
#include <unordered_map>

#include <OglUI/UI_TreeNode.h>
#include <OglUI/UI_PanelWindow.h>
#include <OglRenderer/Render_TextureLoader.h>

namespace Editor
{
	class Editor_AssetBrowser : public UI::UI_PanelWindow
	{
		public:
			Editor_AssetBrowser
			(
				const std::string& pTitle,
				bool pOpened,
				const UI::UI_PanelWindowSettings& pWindowSettings,
				const std::string& p_engineAssetFolder = "",
				const std::string& p_projectAssetFolder = "",
				const std::string& p_projectScriptFolder = ""
			);

			void Fill();

			void Clear();

			void Refresh();

	private:
		void ParseFolder(UI::UI_TreeNode& pRoot, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder = false);
		void ConsiderItem(UI::UI_TreeNode* pRoot, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen = false, bool p_scriptFolder = false);

	public:
		static const std::string __FILENAMES_CHARS;

	private:
		std::string m_engineAssetFolder;
		std::string m_projectAssetFolder;
		std::string m_projectScriptFolder;
		UI::UI_Group* m_assetList;
		std::unordered_map<UI::UI_TreeNode*, std::string> m_pathUpdate;
	};
}

