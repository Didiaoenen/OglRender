#pragma once

#include <OglRenderer/Render_Camera.h>
#include <OglRenderer/Render_TextureLoader.h>

#include <OglCore/Core_SceneManager.h>

#include <OglUI/UI_Group.h>
#include <OglUI/UI_TreeNode.h>
#include <OglUI/UI_PanelWindow.h>

namespace Editor
{
	class Editor_Hierarchy : public UI::UI_PanelWindow
	{
	public:
		Editor_Hierarchy
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		void Clear();

		void UnselectActorsWidgets();

		void SelectActorByInstance(Core::Core_Actor& pActor);

		void SelectActorByWidget(UI::UI_TreeNode& p_widget);

		void AttachActorToParent(Core::Core_Actor& pActor);

		void DetachFromParent(Core::Core_Actor& pActor);

		void DeleteActorByInstance(Core::Core_Actor& pActor);

		void AddActorByInstance(Core::Core_Actor& pActor);

	public:
		Tools::Tools_Event<Core::Core_Actor&> ActorSelectedEvent;
		Tools::Tools_Event<Core::Core_Actor&> ActorUnselectedEvent;

	private:
		UI::UI_TreeNode* m_sceneRoot;

		std::unordered_map<Core::Core_Actor*, UI::UI_TreeNode*> m_widgetActorLink;
	};
}

