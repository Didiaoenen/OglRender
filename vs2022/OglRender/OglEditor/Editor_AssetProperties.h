#pragma once

#include <variant>

#include <OglTools/Tools_Event.h>
#include <OglTools/Tools_IniFile.h>

#include <OglUI/UI_Text.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_PanelWindow.h>

#include <OglRenderer/Render_Model.h>
#include <OglRenderer/Render_Texture.h>

namespace Editor
{
	class Editor_AssetProperties : public UI::UI_PanelWindow
	{
	public:
		using EditableAssets = std::variant<Render::Render_Model*, Render::Render_Texture*>;

		Editor_AssetProperties
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		void SetTarget(const std::string& pPath);

		void Refresh();

		void Preview();

	private:
		void CreateHeaderButtons();
		void CreateAssetSelector();
		void CreateSettings();
		void CreateInfo();
		void CreateModelSettings();
		void CreateTextureSettings();
		void Apply();

	private:
		std::string mResource;

		Tools::Tools_Event<> mTargetChanged;
		UI::UI_Group* mSettings{ nullptr };
		UI::UI_Group* mInfo{ nullptr };
		UI::UI_Button* mApplyButton{ nullptr };
		UI::UI_Button* mRevertButton{ nullptr };
		UI::UI_Button* mPreviewButton{ nullptr };
		UI::UI_Button* mResetButton{ nullptr };
		UI::UI_AWidget* mHeaderSeparator{ nullptr };
		UI::UI_AWidget* mHeaderLineBreak{ nullptr };
		UI::UI_Columns<2>* mSettingsColumns{ nullptr };
		UI::UI_Columns<2>* mInfoColumns{ nullptr };
		UI::UI_Text* mAssetSelector{ nullptr };
		std::unique_ptr<Tools::Tools_IniFile> mMetadata;
	};
}

