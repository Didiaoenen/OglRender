#include <OglCore/Core_CCamera.h>

#include "Editor_EditorActions.h"
#include "Editor_EditorSettings.h"
#include "Editor_EditorRenderer.h"
#include "Editor_GameView.h"

Editor::Editor_GameView::Editor_GameView(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	Editor_AView(pTitle, pOpened, pWindowSettings), mSceneManager(EDITOR_CONTEXT(mSceneManager))
{
	SetIcon(ICON_MDI_GAMEPAD_VARIANT " ");
}

void Editor::Editor_GameView::Update(float pDeltaTime)
{
	Editor_AView::Update(pDeltaTime);

	auto currentScene = EDITOR_CONTEXT(mSceneManager).GetCurrentScene();

	if (currentScene)
	{
		auto cameraComponent = EDITOR_CONTEXT(mRenderer)->FindMainCamera(*currentScene);
		if (cameraComponent)
		{
			mCamera = cameraComponent->GetCamera();
			mCameraPosition = cameraComponent->mOwner.transform.GetWorldPosition();
			mCameraRotation = cameraComponent->mOwner.transform.GetWorldRotation();
			mHasCamera = true;
			PrepareCamera();
		}
		else
		{
			mCamera.SetClearColor({ 0.f, 0.f, 0.f });
			mHasCamera = false;
		}
	}
}

void Editor::Editor_GameView::_Render_Impl()
{
	auto& baseRenderer = *EDITOR_CONTEXT(mRenderer).get();
	auto& currentScene = *mSceneManager.GetCurrentScene();

	mFbo.Bind();

	baseRenderer.Clear(mCamera);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	if (mHasCamera)
	{
		if (mCamera.HasFrustumLightCulling())
		{
			mEditorRenderer.UpdateLightsInFrustum(currentScene, mCamera.GetFrustum());
		}
		else
		{
			mEditorRenderer.UpdateLights(currentScene);
		}

		mEditorRenderer.RenderScene(mCameraPosition, mCamera);
	}

	baseRenderer.ApplyStateMask(glState);

	mFbo.Unbind();
}

bool Editor::Editor_GameView::HasCamera() const
{
	return mHasCamera;
}

std::optional<Render::Render_Frustum> Editor::Editor_GameView::GetActiveFrustum() const
{
	return mHasCamera ? mCamera.GetFrustum() : std::optional<Render::Render_Frustum>{};
}
