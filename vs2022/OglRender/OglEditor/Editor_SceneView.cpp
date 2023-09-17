
#include <OglUI/UI_DDTarget.h>

#include "Editor_EditorSettings.h"
#include "Editor_EditorRenderer.h"
#include "Editor_EditorActions.h"
#include "Editor_GameView.h"
#include "Editor_SceneView.h"

Editor::Editor_SceneView::Editor_SceneView(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	Editor_AViewControllable(p_title, p_opened, p_windowSettings, true), m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
	SetIcon(ICON_MDI_GAMEPAD_VARIANT " ");

	mCamera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	mCamera.SetFar(5000.0f);

	m_image->AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [this](auto p_data)
		{
			std::string path = p_data.first;

			switch (Tools::Tools_PathParser::GetFileType(path))
			{
				case Tools::Tools_PathParser::EFileType::SCENE:	EDITOR_EXEC(LoadSceneFromDisk(path));			break;
				case Tools::Tools_PathParser::EFileType::MODEL:	EDITOR_EXEC(CreateActorWithModel(path, true));	break;
			}
		};
}

void Editor::Editor_SceneView::Update(float pDeltaTime)
{
	Editor_AViewControllable::Update(pDeltaTime);

	if (IsFocused() && !m_cameraController.IsRightMousePressed())
	{
		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(Window::EKey::KEY_W))
		{
			m_currentOperation = Editor::EGizmoOperation::TRANSLATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(Window::EKey::KEY_E))
		{
			m_currentOperation = Editor::EGizmoOperation::ROTATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(Window::EKey::KEY_R))
		{
			m_currentOperation = Editor::EGizmoOperation::SCALE;
		}
	}
}

void Editor::Editor_SceneView::_Render_Impl()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);
	HandleActorPicking();
	baseRenderer.ApplyStateMask(glState);
	RenderScene(glState);
	baseRenderer.ApplyStateMask(glState);
}

void Editor::Editor_SceneView::RenderScene(uint8_t p_defaultRenderState)
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();
	auto& currentScene = *m_sceneManager.GetCurrentScene();
	auto& gameView = EDITOR_PANEL(Editor::Editor_GameView, "Game View");

	if (auto gameViewFrustum = gameView.GetActiveFrustum(); gameViewFrustum.has_value() && gameView.GetCamera().HasFrustumLightCulling() && Editor::Editor_EditorSettings::ShowLightFrustumCullingInSceneView)
	{
		mEditorRenderer.UpdateLightsInFrustum(currentScene, gameViewFrustum.value());
	}
	else
	{
		mEditorRenderer.UpdateLights(currentScene);
	}

	m_fbo.Bind();

	baseRenderer.SetStencilMask(0xFF);
	baseRenderer.Clear(mCamera);
	baseRenderer.SetStencilMask(0x00);

	mEditorRenderer.RenderGrid(m_cameraPosition, m_gridColor);
	mEditorRenderer.RenderCameras();

	if (auto gameViewFrustum = gameView.GetActiveFrustum(); gameViewFrustum.has_value() && gameView.GetCamera().HasFrustumGeometryCulling() && Editor::Editor_EditorSettings::ShowGeometryFrustumCullingInSceneView)
	{
		mCamera.SetFrustumGeometryCulling(gameView.HasCamera() ? gameView.GetCamera().HasFrustumGeometryCulling() : false);
		mEditorRenderer.RenderScene(m_cameraPosition, mCamera, &gameViewFrustum.value());
		mCamera.SetFrustumGeometryCulling(false);
	}
	else
	{
		mEditorRenderer.RenderScene(m_cameraPosition, mCamera);
	}

	mEditorRenderer.RenderLights();

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());

		if (selectedActor.IsActive())
		{
			mEditorRenderer.RenderActorOutlinePass(selectedActor, true, true);
			baseRenderer.ApplyStateMask(p_defaultRenderState);
			mEditorRenderer.RenderActorOutlinePass(selectedActor, false, true);
		}

		baseRenderer.ApplyStateMask(p_defaultRenderState);
		baseRenderer.Clear(false, true, false);

		int highlightedAxis = -1;

		if (m_highlightedGizmoDirection.has_value())
		{
			highlightedAxis = static_cast<int>(m_highlightedGizmoDirection.value());
		}

		mEditorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, false, highlightedAxis);
	}

	if (m_highlightedActor.has_value())
	{
		mEditorRenderer.RenderActorOutlinePass(m_highlightedActor.value().get(), true, false);
		baseRenderer.ApplyStateMask(p_defaultRenderState);
		mEditorRenderer.RenderActorOutlinePass(m_highlightedActor.value().get(), false, false);
	}

	m_fbo.Unbind();
}

void Editor::Editor_SceneView::RenderSceneForActorPicking()
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	auto [winWidth, winHeight] = GetSafeSize();

	m_actorPickingFramebuffer.Resize(winWidth, winHeight);
	m_actorPickingFramebuffer.Bind();
	baseRenderer.SetClearColor(1.0f, 1.0f, 1.0f);
	baseRenderer.Clear();
	mEditorRenderer.RenderSceneForActorPicking();

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());
		baseRenderer.Clear(false, true, false);
		mEditorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, true);
	}

	m_actorPickingFramebuffer.Unbind();
}

bool IsResizing()
{
	auto cursor = ImGui::GetMouseCursor();

	return
		cursor == ImGuiMouseCursor_ResizeEW ||
		cursor == ImGuiMouseCursor_ResizeNS ||
		cursor == ImGuiMouseCursor_ResizeNWSE ||
		cursor == ImGuiMouseCursor_ResizeNESW ||
		cursor == ImGuiMouseCursor_ResizeAll;;
}

void Editor::Editor_SceneView::HandleActorPicking()
{
	auto& inputManager = *EDITOR_CONTEXT(inputManager);

	if (inputManager.IsMouseButtonReleased(Window::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		m_gizmoOperations.StopPicking();
	}

	if (IsHovered() && !IsResizing())
	{
		RenderSceneForActorPicking();

		auto [mouseX, mouseY] = inputManager.GetMousePosition();
		mouseX -= mPosition.x;
		mouseY -= mPosition.y;
		mouseY = GetSafeSize().second - mouseY + 25;

		m_actorPickingFramebuffer.Bind();
		uint8_t pixel[3];
		EDITOR_CONTEXT(renderer)->ReadPixels(static_cast<int>(mouseX), static_cast<int>(mouseY), 1, 1, Render::EPixelDataFormat::RGB, Render::EPixelDataType::UNSIGNED_BYTE, pixel);
		m_actorPickingFramebuffer.Unbind();

		uint32_t actorID = (0 << 24) | (pixel[2] << 16) | (pixel[1] << 8) | (pixel[0] << 0);
		auto actorUnderMouse = EDITOR_CONTEXT(sceneManager).GetCurrentScene()->FindActorByID(actorID);
		auto direction = m_gizmoOperations.IsPicking() ? m_gizmoOperations.GetDirection() : EDITOR_EXEC(IsAnyActorSelected()) && pixel[0] == 255 && pixel[1] == 255 && pixel[2] >= 252 && pixel[2] <= 254 ? static_cast<Editor_GizmoBehaviour::EDirection>(pixel[2] - 252) : std::optional<Editor_GizmoBehaviour::EDirection>{};

		m_highlightedActor = {};
		m_highlightedGizmoDirection = {};

		if (!m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				m_highlightedGizmoDirection = direction;

			}
			else if (actorUnderMouse != nullptr)
			{
				m_highlightedActor = std::ref(*actorUnderMouse);
			}
		}

		if (inputManager.IsMouseButtonPressed(Window::EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				m_gizmoOperations.StartPicking(EDITOR_EXEC(GetSelectedActor()), m_cameraPosition, m_currentOperation, direction.value());
			}
			else
			{

				if (actorUnderMouse)
				{
					EDITOR_EXEC(SelectActor(*actorUnderMouse));
				}
				else
				{
					EDITOR_EXEC(UnselectActor());
				}
			}
		}
	}

	if (m_gizmoOperations.IsPicking())
	{
		auto mousePosition = EDITOR_CONTEXT(inputManager)->GetMousePosition();

		auto [winWidth, winHeight] = GetSafeSize();

		m_gizmoOperations.SetCurrentMouse({ static_cast<float>(mousePosition.first), static_cast<float>(mousePosition.second) });
		m_gizmoOperations.ApplyOperation(mCamera.GetViewMatrix(), mCamera.GetProjectionMatrix(), { static_cast<float>(winWidth), static_cast<float>(winHeight) });
	}
}
