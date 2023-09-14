#include <OglCore/Core_CCamera.h>

#include "Editor_EditorActions.h"
#include "Editor_EditorSettings.h"
#include "Editor_EditorRenderer.h"
#include "Editor_GameView.h"

Editor::Editor_GameView::Editor_GameView(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	Editor_AView(p_title, p_opened, p_windowSettings), m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
}

void Editor::Editor_GameView::Update(float pDeltaTime)
{
	Editor_AView::Update(pDeltaTime);

	auto currentScene = EDITOR_CONTEXT(sceneManager).GetCurrentScene();

	if (currentScene)
	{
		auto cameraComponent = EDITOR_CONTEXT(renderer)->FindMainCamera(*currentScene);
		if (cameraComponent)
		{
			mCamera = cameraComponent->GetCamera();
			m_cameraPosition = cameraComponent->mOwner.transform.GetWorldPosition();
			m_cameraRotation = cameraComponent->mOwner.transform.GetWorldRotation();
			m_hasCamera = true;
			PrepareCamera();
		}
		else
		{
			mCamera.SetClearColor({ 0.f, 0.f, 0.f });
			m_hasCamera = false;
		}
	}
}

void Editor::Editor_GameView::_Render_Impl()
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();
	auto& currentScene = *m_sceneManager.GetCurrentScene();

	m_fbo.Bind();

	baseRenderer.Clear(mCamera);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	if (m_hasCamera)
	{
		if (mCamera.HasFrustumLightCulling())
		{
			mEditorRenderer.UpdateLightsInFrustum(currentScene, mCamera.GetFrustum());
		}
		else
		{
			mEditorRenderer.UpdateLights(currentScene);
		}

		mEditorRenderer.RenderScene(m_cameraPosition, mCamera);
	}

	baseRenderer.ApplyStateMask(glState);

	m_fbo.Unbind();
}

bool Editor::Editor_GameView::HasCamera() const
{
	return m_hasCamera;
}

std::optional<Render::Render_Frustum> Editor::Editor_GameView::GetActiveFrustum() const
{
	return m_hasCamera ? mCamera.GetFrustum() : std::optional<Render::Render_Frustum>{};
}
