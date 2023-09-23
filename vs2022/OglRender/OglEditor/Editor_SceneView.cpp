
#include <OglUI/UI_DDTarget.h>

#include "Editor_EditorSettings.h"
#include "Editor_EditorRenderer.h"
#include "Editor_EditorActions.h"
#include "Editor_GameView.h"
#include "Editor_SceneView.h"

Editor::Editor_SceneView::Editor_SceneView(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	Editor_AViewControllable(pTitle, pOpened, pWindowSettings, true), mSceneManager(EDITOR_CONTEXT(mSceneManager))
{
	SetIcon(ICON_MDI_GAMEPAD_VARIANT " ");

	mCamera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	mCamera.SetFar(5000.0f);

	mImage->AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [this](auto pData)
		{
			std::string path = pData.first;

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

	if (IsFocused() && !mCameraController.IsRightMousePressed())
	{
		if (EDITOR_CONTEXT(mInputManager)->IsKeyPressed(Window::EKey::KEY_W))
		{
			mCurrentOperation = Editor::EGizmoOperation::TRANSLATE;
		}

		if (EDITOR_CONTEXT(mInputManager)->IsKeyPressed(Window::EKey::KEY_E))
		{
			mCurrentOperation = Editor::EGizmoOperation::ROTATE;
		}

		if (EDITOR_CONTEXT(mInputManager)->IsKeyPressed(Window::EKey::KEY_R))
		{
			mCurrentOperation = Editor::EGizmoOperation::SCALE;
		}
	}
}

void Editor::Editor_SceneView::_Render_Impl()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(mRenderer).get();

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);
	HandleActorPicking();
	baseRenderer.ApplyStateMask(glState);
	RenderScene(glState);
	baseRenderer.ApplyStateMask(glState);
}

void Editor::Editor_SceneView::RenderScene(uint8_t pDefaultRenderState)
{
	auto& baseRenderer = *EDITOR_CONTEXT(mRenderer).get();
	auto& currentScene = *mSceneManager.GetCurrentScene();
	auto& gameView = EDITOR_PANEL(Editor::Editor_GameView, "Game");

	auto gameViewFrustum = gameView.GetActiveFrustum();
	if (gameViewFrustum && gameViewFrustum.has_value() && gameView.GetCamera().HasFrustumLightCulling() && Editor::Editor_EditorSettings::ShowLightFrustumCullingInSceneView)
	{
		mEditorRenderer.UpdateLightsInFrustum(currentScene, gameViewFrustum.value());
	}
	else
	{
		mEditorRenderer.UpdateLights(currentScene);
	}

	mFbo.Bind();

	baseRenderer.SetStencilMask(0xFF);
	baseRenderer.Clear(mCamera);
	baseRenderer.SetStencilMask(0x00);

	mEditorRenderer.RenderGrid(mCameraPosition, mGridColor);
	mEditorRenderer.RenderCameras();

	if (gameViewFrustum && gameViewFrustum.has_value() && gameView.GetCamera().HasFrustumGeometryCulling() && Editor::Editor_EditorSettings::ShowGeometryFrustumCullingInSceneView)
	{
		mCamera.SetFrustumGeometryCulling(gameView.HasCamera() ? gameView.GetCamera().HasFrustumGeometryCulling() : false);
		mEditorRenderer.RenderScene(mCameraPosition, mCamera, &gameViewFrustum.value());
		mCamera.SetFrustumGeometryCulling(false);
	}
	else
	{
		mEditorRenderer.RenderScene(mCameraPosition, mCamera);
	}

	mEditorRenderer.RenderLights();

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());

		if (selectedActor.IsActive())
		{
			mEditorRenderer.RenderActorOutlinePass(selectedActor, true, true);
			baseRenderer.ApplyStateMask(pDefaultRenderState);
			mEditorRenderer.RenderActorOutlinePass(selectedActor, false, true);
		}

		baseRenderer.ApplyStateMask(pDefaultRenderState);
		baseRenderer.Clear(false, true, false);

		int highlightedAxis = -1;

		if (mHighlightedGizmoDirection.has_value())
		{
			highlightedAxis = static_cast<int>(mHighlightedGizmoDirection.value());
		}

		mEditorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), mCurrentOperation, false, highlightedAxis);
	}

	if (mHighlightedActor.has_value())
	{
		mEditorRenderer.RenderActorOutlinePass(mHighlightedActor.value().get(), true, false);
		baseRenderer.ApplyStateMask(pDefaultRenderState);
		mEditorRenderer.RenderActorOutlinePass(mHighlightedActor.value().get(), false, false);
	}

	mFbo.Unbind();
}

void Editor::Editor_SceneView::RenderSceneForActorPicking()
{
	auto& baseRenderer = *EDITOR_CONTEXT(mRenderer).get();

	auto [winWidth, winHeight] = GetSafeSize();

	mActorPickingFramebuffer.Resize(winWidth, winHeight);
	mActorPickingFramebuffer.Bind();
	baseRenderer.SetClearColor(1.0f, 1.0f, 1.0f);
	baseRenderer.Clear();
	mEditorRenderer.RenderSceneForActorPicking();

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());
		baseRenderer.Clear(false, true, false);
		mEditorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), mCurrentOperation, true);
	}

	mActorPickingFramebuffer.Unbind();
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
	auto& mInputManager = *EDITOR_CONTEXT(mInputManager);

	if (mInputManager.IsMouseButtonReleased(Window::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		mGizmoOperations.StopPicking();
	}

	if (IsHovered() && !IsResizing())
	{
		RenderSceneForActorPicking();

		auto [mouseX, mouseY] = mInputManager.GetMousePosition();
		mouseX -= mPosition.x;
		mouseY -= mPosition.y;
		mouseY = GetSafeSize().second - mouseY + 25;

		mActorPickingFramebuffer.Bind();
		uint8_t pixel[3];
		EDITOR_CONTEXT(mRenderer)->ReadPixels(static_cast<int>(mouseX), static_cast<int>(mouseY), 1, 1, Render::EPixelDataFormat::RGB, Render::EPixelDataType::UNSIGNED_BYTE, pixel);
		mActorPickingFramebuffer.Unbind();

		uint32_t actorID = (0 << 24) | (pixel[2] << 16) | (pixel[1] << 8) | (pixel[0] << 0);
		auto actorUnderMouse = EDITOR_CONTEXT(mSceneManager).GetCurrentScene()->FindActorByID(actorID);
		auto direction = mGizmoOperations.IsPicking() ? 
			mGizmoOperations.GetDirection() : EDITOR_EXEC(IsAnyActorSelected()) && pixel[0] == 255 && pixel[1] == 255 && pixel[2] >= 252 && pixel[2] <= 254 ? static_cast<Editor_GizmoBehaviour::EDirection>(pixel[2] - 252) : std::optional<Editor_GizmoBehaviour::EDirection>{};

		mHighlightedActor = {};
		mHighlightedGizmoDirection = {};

		if (!mCameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				mHighlightedGizmoDirection = direction;
			}
			else if (actorUnderMouse != nullptr)
			{
				mHighlightedActor = std::ref(*actorUnderMouse);
			}
		}

		if (mInputManager.IsMouseButtonPressed(Window::EMouseButton::MOUSE_BUTTON_LEFT) && !mCameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				mGizmoOperations.StartPicking(EDITOR_EXEC(GetSelectedActor()), mCameraPosition, mCurrentOperation, direction.value());
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

	if (mGizmoOperations.IsPicking())
	{
		auto mousePosition = EDITOR_CONTEXT(mInputManager)->GetMousePosition();

		auto [winWidth, winHeight] = GetSafeSize();

		mGizmoOperations.SetCurrentMouse({ static_cast<float>(mousePosition.first), static_cast<float>(mousePosition.second) });
		mGizmoOperations.ApplyOperation(mCamera.GetViewMatrix(), mCamera.GetProjectionMatrix(), { static_cast<float>(winWidth), static_cast<float>(winHeight) });
	}
}
