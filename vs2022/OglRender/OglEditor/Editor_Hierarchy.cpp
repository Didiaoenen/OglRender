
#include <OglUI/UI_Button.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_DDSource.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_ContextualMenu.h>

#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CDirectionalLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include "Editor_ActorCreationMenu.h"
#include "Editor_EditorActions.h"
#include "Editor_Hierarchy.h"

class Editor_HierarchyContextualMenu : public UI::UI_ContextualMenu
{
public:
	Editor_HierarchyContextualMenu(Core::Core_Actor* p_target, UI::UI_TreeNode& p_treeNode, bool p_panelMenu = false) :
		m_target(p_target),
		m_treeNode(p_treeNode)
	{
		if (m_target)
		{
			auto& focusButton = CreateWidget<UI::UI_MenuItem>("Focus");
			focusButton.mClickedEvent += [this]
				{
					EDITOR_EXEC(MoveToTarget(*m_target));
				};

			auto& duplicateButton = CreateWidget<UI::UI_MenuItem>("Duplicate");
			duplicateButton.mClickedEvent += [this]
				{
					EDITOR_EXEC(DelayAction(EDITOR_BIND(DuplicateActor, std::ref(*m_target), nullptr, true), 0));
				};

			auto& deleteButton = CreateWidget<UI::UI_MenuItem>("Delete");
			deleteButton.mClickedEvent += [this]
				{
					EDITOR_EXEC(DestroyActor(std::ref(*m_target)));
				};
		}

		auto& createActor = CreateWidget<UI::UI_MenuList>("Create...");
		Editor::Editor_ActorCreationMenu::GenerateActorCreationMenu(createActor, m_target, std::bind(&UI::UI_TreeNode::Open, &m_treeNode));
	}

	virtual void Execute() override
	{
		if (mWidgets.size() > 0)
		{
			UI::UI_ContextualMenu::Execute();
		}
	}

private:
	Core::Core_Actor* m_target;
	UI::UI_TreeNode& m_treeNode;
};

void ExpandTreeNode(UI::UI_TreeNode& p_toExpand, const UI::UI_TreeNode* p_root)
{
	p_toExpand.Open();

	if (&p_toExpand != p_root && p_toExpand.HasParent())
	{
		ExpandTreeNode(*static_cast<UI::UI_TreeNode*>(p_toExpand.GetParent()), p_root);
	}
}

std::vector<UI::UI_TreeNode*> nodesToCollapse;
std::vector<UI::UI_TreeNode*> founds;

void ExpandTreeNodeAndEnable(UI::UI_TreeNode& p_toExpand, const UI::UI_TreeNode* p_root)
{
	if (!p_toExpand.IsOpened())
	{
		p_toExpand.Open();
		nodesToCollapse.push_back(&p_toExpand);
	}

	p_toExpand.mEnabled = true;

	if (&p_toExpand != p_root && p_toExpand.HasParent())
	{
		ExpandTreeNodeAndEnable(*static_cast<UI::UI_TreeNode*>(p_toExpand.GetParent()), p_root);
	}
}

Editor::Editor_Hierarchy::Editor_Hierarchy(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	UI::UI_PanelWindow(p_title, p_opened, p_windowSettings)
{
	auto& searchBar = CreateWidget<UI::UI_InputText>();
	searchBar.mContentChangedEvent += [this](const std::string& p_content)
		{
			founds.clear();
			auto content = p_content;
			std::transform(content.begin(), content.end(), content.begin(), ::tolower);

			for (auto& [actor, item] : m_widgetActorLink)
			{
				if (!p_content.empty())
				{
					auto itemName = item->mName;
					std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);

					if (itemName.find(content) != std::string::npos)
					{
						founds.push_back(item);
					}

					item->mEnabled = false;
				}
				else
				{
					item->mEnabled = true;
				}
			}

			for (auto node : founds)
			{
				node->mEnabled = true;

				if (node->HasParent())
				{
					ExpandTreeNodeAndEnable(*static_cast<UI::UI_TreeNode*>(node->GetParent()), m_sceneRoot);
				}
			}

			if (p_content.empty())
			{
				for (auto node : nodesToCollapse)
				{
					node->Close();
				}

				nodesToCollapse.clear();
			}
		};

	m_sceneRoot = &CreateWidget<UI::UI_TreeNode>("Root", true);
	static_cast<UI::UI_TreeNode*>(m_sceneRoot)->Open();
	m_sceneRoot->AddPlugin<UI::UI_DDTarget<std::pair<Core::Core_Actor*, UI::UI_TreeNode*>>>("Actor").mDataReceivedEvent += [this](std::pair<Core::Core_Actor*, UI::UI_TreeNode*> p_element)
		{
			if (p_element.second->HasParent())
				p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

			m_sceneRoot->ConsiderWidget(*p_element.second);

			p_element.first->DetachFromParent();
		};
	m_sceneRoot->AddPlugin<Editor_HierarchyContextualMenu>(nullptr, *m_sceneRoot);

	EDITOR_EVENT(ActorUnselectedEvent) += std::bind(&Editor_Hierarchy::UnselectActorsWidgets, this);
	EDITOR_CONTEXT(sceneManager).SceneUnloadEvent += std::bind(&Editor_Hierarchy::Clear, this);
	Core::Core_Actor::mCreatedEvent += std::bind(&Editor_Hierarchy::AddActorByInstance, this, std::placeholders::_1);
	Core::Core_Actor::mDestroyedEvent += std::bind(&Editor_Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);
	EDITOR_EVENT(ActorSelectedEvent) += std::bind(&Editor_Hierarchy::SelectActorByInstance, this, std::placeholders::_1);
	Core::Core_Actor::mAttachEvent += std::bind(&Editor_Hierarchy::AttachActorToParent, this, std::placeholders::_1);
	Core::Core_Actor::mDettachEvent += std::bind(&Editor_Hierarchy::DetachFromParent, this, std::placeholders::_1);
}

void Editor::Editor_Hierarchy::Clear()
{
	EDITOR_EXEC(UnselectActor());

	m_sceneRoot->RemoveAllWidgets();
	m_widgetActorLink.clear();
}

void Editor::Editor_Hierarchy::UnselectActorsWidgets()
{
	for (auto& widget : m_widgetActorLink)
	{
		widget.second->mSelected = false;
	}
}

void Editor::Editor_Hierarchy::SelectActorByInstance(Core::Core_Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
	{
		if (result->second)
		{
			SelectActorByWidget(*result->second);
		}
	}
}

void Editor::Editor_Hierarchy::SelectActorByWidget(UI::UI_TreeNode& p_widget)
{
	UnselectActorsWidgets();

	p_widget.mSelected = true;

	if (p_widget.HasParent())
	{
		ExpandTreeNode(*static_cast<UI::UI_TreeNode*>(p_widget.GetParent()), m_sceneRoot);
	}
}

void Editor::Editor_Hierarchy::AttachActorToParent(Core::Core_Actor& p_actor)
{
	auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		if (widget->HasParent())
		{
			widget->GetParent()->UnconsiderWidget(*widget);
		}

		if (p_actor.HasParent())
		{
			auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
			parentWidget->mLeaf = false;
			parentWidget->ConsiderWidget(*widget);
		}
	}
}

void Editor::Editor_Hierarchy::DetachFromParent(Core::Core_Actor& p_actor)
{
	if (auto actorWidget = m_widgetActorLink.find(&p_actor); actorWidget != m_widgetActorLink.end())
	{
		if (p_actor.HasParent() && p_actor.GetParent()->GetChildren().size() == 1)
		{
			if (auto parentWidget = m_widgetActorLink.find(p_actor.GetParent()); parentWidget != m_widgetActorLink.end())
			{
				parentWidget->second->mLeaf = true;
			}
		}

		auto widget = actorWidget->second;

		if (widget->HasParent())
		{
			widget->GetParent()->UnconsiderWidget(*widget);
		}

		m_sceneRoot->ConsiderWidget(*widget);
	}
}

void Editor::Editor_Hierarchy::DeleteActorByInstance(Core::Core_Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
	{
		if (result->second)
		{
			result->second->Destroy();
		}

		m_widgetActorLink.erase(result);
	}
}

void Editor::Editor_Hierarchy::AddActorByInstance(Core::Core_Actor& p_actor)
{
	auto& textSelectable = m_sceneRoot->CreateWidget<UI::UI_TreeNode>(p_actor.GetName(), true);
	textSelectable.mLeaf = true;
	textSelectable.AddPlugin<Editor_HierarchyContextualMenu>(&p_actor, textSelectable);
	textSelectable.AddPlugin<UI::UI_DDSource<std::pair<Core::Core_Actor*, UI::UI_TreeNode*>>>("Actor", "Attach to...", std::make_pair(&p_actor, &textSelectable));
	textSelectable.AddPlugin<UI::UI_DDTarget<std::pair<Core::Core_Actor*, UI::UI_TreeNode*>>>("Actor").mDataReceivedEvent += [&p_actor, &textSelectable](std::pair<Core::Core_Actor*, UI::UI_TreeNode*> p_element)
		{
			if (p_element.second->HasParent())
			{
				p_element.second->GetParent()->UnconsiderWidget(*p_element.second);
			}

			textSelectable.ConsiderWidget(*p_element.second);

			p_element.first->SetParent(p_actor);
		};
	auto& dispatcher = textSelectable.AddPlugin<UI::UI_DataDispatcher<std::string>>();

	Core::Core_Actor* targetPtr = &p_actor;
	dispatcher.RegisterGatherer([targetPtr] { return targetPtr->GetName(); });

	m_widgetActorLink[targetPtr] = &textSelectable;

	textSelectable.mClickedEvent += EDITOR_BIND(SelectActor, std::ref(p_actor));
	textSelectable.mDoubleClickedEvent += EDITOR_BIND(MoveToTarget, std::ref(p_actor));
}
