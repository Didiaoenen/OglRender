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
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		~Editor_Inspector();

		void FocusActor(Core::Core_Actor& pTarget);

		void UnFocus();

		void SoftUnFocus();

		Core::Core_Actor* GetTargetActor() const;

		void CreateActorInspector(Core::Core_Actor& pTarget);

		void DrawComponent(Core::Core_AComponent& pComponent);

		void DrawBehaviour(Core::Core_Behaviour& pBehaviour);

		void Refresh();

	private:
		Core::Core_Actor* mTargetActor{ nullptr };

		UI::UI_Group* mActorInfo;
		UI::UI_Group* mInspectorHeader;
		UI::UI_ComboBox* mComponentSelectorWidget;
		UI::UI_InputText* mScriptSelectorWidget;

		uint64_t mComponentAddedListener{ 0 };
		uint64_t mComponentRemovedListener{ 0 };
		uint64_t mBehaviourAddedListener{ 0 };
		uint64_t mBehaviourRemovedListener{ 0 };
		uint64_t mDestroyedListener{ 0 };
	};
}

