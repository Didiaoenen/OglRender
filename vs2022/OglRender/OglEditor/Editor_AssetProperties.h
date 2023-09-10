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
	class Editor_AssetProperties
	{
	public:
		using EditableAssets = std::variant<Render::Render_Model*, Render::Render_Texture*>;

		Editor_AssetProperties
		(
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		void SetTarget(const std::string& p_path);

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
		std::string m_resource;

		Tools::Tools_Event<> m_targetChanged;
		UI::UI_Group* m_settings = nullptr;
		UI::UI_Group* m_info = nullptr;
		UI::UI_Button* m_applyButton = nullptr;
		UI::UI_Button* m_revertButton = nullptr;
		UI::UI_Button* m_previewButton = nullptr;
		UI::UI_Button* m_resetButton = nullptr;
		UI::UI_AWidget* m_headerSeparator = nullptr;
		UI::UI_AWidget* m_headerLineBreak = nullptr;
		UI::UI_Columns<2>* m_settingsColumns = nullptr;
		UI::UI_Columns<2>* m_infoColumns = nullptr;
		UI::UI_Text* m_assetSelector = nullptr;
		std::unique_ptr<Tools::Tools_IniFile> m_metadata;
	};
}

