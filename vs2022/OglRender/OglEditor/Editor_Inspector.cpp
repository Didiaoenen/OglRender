#include <map>

#include <OglUI/UI_Text.h>
#include <OglUI/UI_Image.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_Spacing.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_InputFloat.h>
#include <OglUI/UI_DragMultipleFloats.h>

#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_TextureManager.h>
#include <OglCore/Core_ShaderManager.h>
#include <OglCore/Core_ModelManager.h>

#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CTransform.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CModelRenderer.h>
#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CDirectionalLight.h>
#include <OglCore/Core_CMaterialRenderer.h>

#include <OglWindow/Window_MessageBox.h>

#include "Editor_EditorActions.h"
#include "Editor_Inspector.h"

Editor::Editor_Inspector::Editor_Inspector(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) : 
	UI::UI_PanelWindow(pTitle, pOpened, pWindowSettings)
{
	SetIcon(ICON_MDI_INFORMATION " ");

	mInspectorHeader = &CreateWidget<UI::UI_Group>();
	mInspectorHeader->mEnabled = false;
	mActorInfo = &CreateWidget<UI::UI_Group>();

	auto& headerColumns = mInspectorHeader->CreateWidget<UI::UI_Columns<2>>();

	auto nameGatherer = [this] { return mTargetActor ? mTargetActor->GetName() : "%undef%"; };
	auto nameProvider = [this](const std::string& p_newName) { if (mTargetActor) mTargetActor->SetName(p_newName); };
	Core::Core_GUIDrawer::DrawString(headerColumns, "Name", nameGatherer, nameProvider);

	auto tagGatherer = [this] { return mTargetActor ? mTargetActor->GetTag() : "%undef%"; };
	auto tagProvider = [this](const std::string& p_newName) { if (mTargetActor) mTargetActor->SetTag(p_newName); };
	Core::Core_GUIDrawer::DrawString(headerColumns, "Tag", tagGatherer, tagProvider);

	auto activeGatherer = [this] { return mTargetActor ? mTargetActor->IsSelfActive() : false; };
	auto activeProvider = [this](bool p_active) { if (mTargetActor) mTargetActor->SetActive(p_active); };
	Core::Core_GUIDrawer::DrawBoolean(headerColumns, "Active", activeGatherer, activeProvider);

	{
		auto& componentSelectorWidget = mInspectorHeader->CreateWidget<UI::UI_ComboBox>(0);
		componentSelectorWidget.mLineBreak = false;
		componentSelectorWidget.mChoices.emplace(0, "Model Renderer");
		componentSelectorWidget.mChoices.emplace(1, "Camera");
		componentSelectorWidget.mChoices.emplace(5, "Point Light");
		componentSelectorWidget.mChoices.emplace(6, "Directional Light");
		componentSelectorWidget.mChoices.emplace(7, "Spot Light");
		componentSelectorWidget.mChoices.emplace(8, "Ambient Box Light");
		componentSelectorWidget.mChoices.emplace(9, "Ambient Sphere Light");
		componentSelectorWidget.mChoices.emplace(10, "Material Renderer");

		auto& addComponentButton = mInspectorHeader->CreateWidget<UI::UI_Button>("Add Component", glm::vec2{ 100.f, 0 });
		addComponentButton.mIdleBackgroundColor = UI::Color{ 0.7f, 0.5f, 0.f, 1.f };
		addComponentButton.mTextColor = UI::White;
		addComponentButton.mClickedEvent += [&componentSelectorWidget, this]
			{
				switch (componentSelectorWidget.mCurrentChoice)
				{
					case 0: 
						GetTargetActor()->AddComponent<Core::Core_CModelRenderer>(); 
						GetTargetActor()->AddComponent<Core::Core_CMaterialRenderer>(); 
						break;
					case 1: GetTargetActor()->AddComponent<Core::Core_CCamera>();				break;
					case 5: GetTargetActor()->AddComponent<Core::Core_CPointLight>();			break;
					case 6: GetTargetActor()->AddComponent<Core::Core_CDirectionalLight>();		break;
					case 7: GetTargetActor()->AddComponent<Core::Core_CSpotLight>();			break;
					case 8: GetTargetActor()->AddComponent<Core::Core_CAmbientBoxLight>();		break;
					case 9: GetTargetActor()->AddComponent<Core::Core_CAmbientSphereLight>();	break;
					case 10: GetTargetActor()->AddComponent<Core::Core_CMaterialRenderer>();	break;
				}

				componentSelectorWidget.mValueChangedEvent.Invoke(componentSelectorWidget.mCurrentChoice);
			};

		componentSelectorWidget.mValueChangedEvent += [this, &addComponentButton](int p_value)
			{
				auto defineButtonsStates = [&addComponentButton](bool p_componentExists)
					{
						addComponentButton.mDisabled = p_componentExists;
						addComponentButton.mIdleBackgroundColor = !p_componentExists ? UI::Color{ 0.7f, 0.5f, 0.f, 1.f } : UI::Color{ 0.1f, 0.1f, 0.1f, 1.f };
					};

				switch (p_value)
				{
					case 0: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CModelRenderer>());		return;
					case 1: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CCamera>());				return;
					case 5: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CPointLight>());			return;
					case 6: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CDirectionalLight>());	return;
					case 7: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CSpotLight>());			return;
					case 8: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CAmbientBoxLight>());		return;
					case 9: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CAmbientSphereLight>());	return;
					case 10: defineButtonsStates(GetTargetActor()->GetComponent<Core::Core_CMaterialRenderer>());	return;
				}
			};

		mComponentSelectorWidget = &componentSelectorWidget;
	}

	{
		mScriptSelectorWidget = &mInspectorHeader->CreateWidget<UI::UI_InputText>("");
		mScriptSelectorWidget->mLineBreak = false;
		auto& ddTarget = mScriptSelectorWidget->AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File");

		auto& addScriptButton = mInspectorHeader->CreateWidget<UI::UI_Button>("Add Script", glm::vec2{ 100.f, 0 });
		addScriptButton.mIdleBackgroundColor = UI::Color{ 0.7f, 0.5f, 0.f, 1.f };
		addScriptButton.mTextColor = UI::White;

		const auto updateAddScriptButton = [&addScriptButton, this](const std::string& p_script)
			{
				const std::string realScriptPath = EDITOR_CONTEXT(mProjectScriptsPath) + p_script + ".lua";

				const auto targetActor = GetTargetActor();
				const bool isScriptValid = std::filesystem::exists(realScriptPath) && targetActor && !targetActor->GetBehaviour(p_script);

				addScriptButton.mDisabled = !isScriptValid;
				addScriptButton.mIdleBackgroundColor = isScriptValid ? UI::Color{ 0.7f, 0.5f, 0.f, 1.f } : UI::Color{ 0.1f, 0.1f, 0.1f, 1.f };
			};

		mScriptSelectorWidget->mContentChangedEvent += updateAddScriptButton;

		addScriptButton.mClickedEvent += [updateAddScriptButton, this]
			{
				const std::string realScriptPath = EDITOR_CONTEXT(mProjectScriptsPath) + mScriptSelectorWidget->mContent + ".lua";

				if (std::filesystem::exists(realScriptPath))
				{
					GetTargetActor()->AddBehaviour(mScriptSelectorWidget->mContent);
					updateAddScriptButton(mScriptSelectorWidget->mContent);
				}
			};

		ddTarget.mDataReceivedEvent += [updateAddScriptButton, this](std::pair<std::string, UI::UI_Group*> pData)
			{
				mScriptSelectorWidget->mContent = EDITOR_EXEC(GetScriptPath(pData.first));
				updateAddScriptButton(mScriptSelectorWidget->mContent);
			};
	}

	mInspectorHeader->CreateWidget<UI::UI_Separator>();

	mDestroyedListener = Core::Core_Actor::mDestroyedEvent += [this](Core::Core_Actor& p_destroyed)
		{
			if (&p_destroyed == mTargetActor)
			{
				UnFocus();
			}
		};
}

Editor::Editor_Inspector::~Editor_Inspector()
{
	Core::Core_Actor::mDestroyedEvent -= mDestroyedListener;

	UnFocus();
}

void Editor::Editor_Inspector::FocusActor(Core::Core_Actor& pTarget)
{
	if (mTargetActor)
	{
		UnFocus();
	}

	mActorInfo->RemoveAllWidgets();

	mTargetActor = &pTarget;

	mComponentAddedListener = mTargetActor->mComponentAddedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	mBehaviourAddedListener = mTargetActor->mBehaviourAddedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	mComponentRemovedListener = mTargetActor->mComponentRemovedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	mBehaviourRemovedListener = mTargetActor->mBehaviourRemovedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };

	mInspectorHeader->mEnabled = true;

	CreateActorInspector(pTarget);

	mComponentSelectorWidget->mValueChangedEvent.Invoke(mComponentSelectorWidget->mCurrentChoice);
	mScriptSelectorWidget->mContentChangedEvent.Invoke(mScriptSelectorWidget->mContent);

	EDITOR_EVENT(ActorSelectedEvent).Invoke(*mTargetActor);
}

void Editor::Editor_Inspector::UnFocus()
{
	if (mTargetActor)
	{
		mTargetActor->mComponentAddedEvent -= mComponentAddedListener;
		mTargetActor->mComponentRemovedEvent -= mComponentRemovedListener;
		mTargetActor->mBehaviourAddedEvent -= mBehaviourAddedListener;
		mTargetActor->mBehaviourRemovedEvent -= mBehaviourRemovedListener;
	}

	SoftUnFocus();
}

void Editor::Editor_Inspector::SoftUnFocus()
{
	if (mTargetActor)
	{
		EDITOR_EVENT(ActorUnselectedEvent).Invoke(*mTargetActor);
		mInspectorHeader->mEnabled = false;
		mTargetActor = nullptr;
		mActorInfo->RemoveAllWidgets();
	}
}

Core::Core_Actor* Editor::Editor_Inspector::GetTargetActor() const
{
	return mTargetActor;
}

void Editor::Editor_Inspector::CreateActorInspector(Core::Core_Actor& pTarget)
{
	std::map<std::string, Core::Core_AComponent*> components;

	for (auto component : pTarget.GetComponents())
	{
		if (component->GetName() != "Transform")
		{
			components[component->GetName()] = component.get();
		}
	}

	auto transform = pTarget.GetComponent<Core::Core_CTransform>();
	if (transform)
	{
		DrawComponent(*transform);
	}

	for (auto& [name, instance] : components)
	{
		DrawComponent(*instance);
	}

	auto& behaviours = pTarget.GetBehaviours();

	for (auto& [name, behaviour] : behaviours)
	{
		DrawBehaviour(behaviour);
	}
}

void Editor::Editor_Inspector::DrawComponent(Core::Core_AComponent& pComponent)
{
	if (auto inspectorItem = dynamic_cast<Core::Core_IInspectorItem*>(&pComponent); inspectorItem)
	{
		auto& header = mActorInfo->CreateWidget<UI::UI_GroupCollapsable>(pComponent.GetName());
		header.mClosable = !dynamic_cast<Core::Core_CTransform*>(&pComponent);
		header.mCloseEvent += [this, &header, &pComponent]
			{
				if (pComponent.mOwner.RemoveComponent(pComponent))
				{
					mComponentSelectorWidget->mValueChangedEvent.Invoke(mComponentSelectorWidget->mCurrentChoice);
				}
			};
		auto& columns = header.CreateWidget<UI::UI_Columns<2>>();
		columns.mWidths[0] = 200;
		inspectorItem->OnInspector(columns);
	}
}

void Editor::Editor_Inspector::DrawBehaviour(Core::Core_Behaviour& pBehaviour)
{
	if (auto inspectorItem = dynamic_cast<Core::Core_IInspectorItem*>(&pBehaviour); inspectorItem)
	{
		auto& header = mActorInfo->CreateWidget<UI::UI_GroupCollapsable>(pBehaviour.mName);
		header.mClosable = true;
		header.mCloseEvent += [this, &header, &pBehaviour]
			{
				pBehaviour.mOwner.RemoveBehaviour(pBehaviour);
			};

		auto& columns = header.CreateWidget<UI::UI_Columns<2>>();
		columns.mWidths[0] = 200;
		inspectorItem->OnInspector(columns);
	}
}

void Editor::Editor_Inspector::Refresh()
{
	if (mTargetActor)
	{
		mActorInfo->RemoveAllWidgets();
		CreateActorInspector(*mTargetActor);
	}
}
