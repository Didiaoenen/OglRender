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

		void SetTarget(Core::Core_Material& pNewTarget);

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
		Core::Core_Material* mTarget{ nullptr };
		Render::Render_Shader* mShader{ nullptr };

		UI::UI_Text* mTargetMaterialText{ nullptr };
		UI::UI_Text* mShaderText{ nullptr };

		Tools::Tools_Event<> mMaterialDroppedEvent;
		Tools::Tools_Event<> mShaderDroppedEvent;

		UI::UI_Group* mSettings{ nullptr };
		UI::UI_Group* mMaterialSettings{ nullptr };
		UI::UI_Group* mShaderSettings{ nullptr };

		UI::UI_Columns<2>* mShaderSettingsColumns{ nullptr };
		UI::UI_Columns<2>* mMaterialSettingsColumns{ nullptr };
	};
}

