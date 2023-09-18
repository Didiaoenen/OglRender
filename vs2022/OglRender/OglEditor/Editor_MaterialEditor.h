#pragma once

#include <OglUI/UI_Text.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_PanelWindow.h>

#include <OglRenderer/Render_Shader.h>

namespace Core
{
	class Core_Material;
}

namespace Editor
{
	class Editor_MaterialEditor : public UI::UI_PanelWindow
	{
	public:
		Editor_MaterialEditor
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		void Refresh();

		void SetTarget(Core::Core_Material& p_newTarget);

		Core::Core_Material* GetTarget() const;

		void RemoveTarget();

		void Preview();

		void Reset();

	private:
		void OnMaterialDropped();
		void OnShaderDropped();

		void CreateHeaderButtons();
		void CreateMaterialSelector();
		void CreateShaderSelector();
		void CreateMaterialSettings();
		void CreateShaderSettings();

		void GenerateShaderSettingsContent();
		void GenerateMaterialSettingsContent();

	private:
		Core::Core_Material* m_target = nullptr;
		Render::Render_Shader* m_shader = nullptr;

		UI::UI_Text* m_targetMaterialText = nullptr;
		UI::UI_Text* m_shaderText = nullptr;

		Tools::Tools_Event<> m_materialDroppedEvent;
		Tools::Tools_Event<> m_shaderDroppedEvent;

		UI::UI_Group* m_settings = nullptr;
		UI::UI_Group* m_materialSettings = nullptr;
		UI::UI_Group* m_shaderSettings = nullptr;

		UI::UI_Columns<2>* m_shaderSettingsColumns = nullptr;
		UI::UI_Columns<2>* m_materialSettingsColumns = nullptr;
	};
}

