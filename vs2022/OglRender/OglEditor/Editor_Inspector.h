#pragma once

#include <OglRenderer/Render_TextureLoader.h>

#include <OglUI/UI_Button.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_ComboBox.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_InputText.h>
#include <OglUI/UI_PanelWindow.h>
#include <OglUI/UI_GroupCollapsable.h>

#include "Editor_AssetBrowser.h"
#include "Editor_Hierarchy.h"

namespace Editor
{
	class Editor_Inspector : public UI::UI_PanelWindow
	{
	public:
		Editor_Inspector
		(
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		~Editor_Inspector();

		void FocusActor(Core::Core_Actor& p_target);

		void UnFocus();

		void SoftUnFocus();

		Core::Core_Actor* GetTargetActor() const;

		void CreateActorInspector(Core::Core_Actor& p_target);

		void DrawComponent(Core::Core_AComponent& p_component);

		void DrawBehaviour(Core::Core_Behaviour& p_behaviour);

		void Refresh();

	private:
		Core::Core_Actor* m_targetActor = nullptr;
		UI::UI_Group* m_actorInfo;
		UI::UI_Group* m_inspectorHeader;
		UI::UI_ComboBox* m_componentSelectorWidget;
		UI::UI_InputText* m_scriptSelectorWidget;

		uint64_t m_componentAddedListener = 0;
		uint64_t m_componentRemovedListener = 0;
		uint64_t m_behaviourAddedListener = 0;
		uint64_t m_behaviourRemovedListener = 0;
		uint64_t m_destroyedListener = 0;
	};
}

