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
				const std::string& pEngineAssetFolder = "",
				const std::string& pProjectAssetFolder = "",
				const std::string& pProjectScriptFolder = ""
			);

			void Fill();

			void Clear();

			void Refresh();

	private:
		void ParseFolder(UI::UI_TreeNode& pRoot, const std::filesystem::directory_entry& pDirectory, bool pIsEngineItem, bool pScriptFolder = false);
		void ConsiderItem(UI::UI_TreeNode* pRoot, const std::filesystem::directory_entry& pEntry, bool pIsEngineItem, bool pAutoOpen = false, bool pScriptFolder = false);

	public:
		static const std::string __FILENAMES_CHARS;

	private:
		std::string mEngineAssetFolder;
		std::string mProjectAssetFolder;
		std::string mProjectScriptFolder;
		UI::UI_Group* mAssetList;
		std::unordered_map<UI::UI_TreeNode*, std::string> mPathUpdate;
	};
}

