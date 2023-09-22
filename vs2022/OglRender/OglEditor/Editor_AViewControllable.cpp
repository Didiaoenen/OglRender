#include "Editor_AViewControllable.h"

Editor::Editor_AViewControllable::Editor_AViewControllable(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings, bool pEnableFocusInputs) :
	Editor_AView(pTitle, pOpened, pWindowSettings), mCameraController(*this, mCamera, mCameraPosition, mCameraRotation, pEnableFocusInputs)
{
}

void Editor::Editor_AViewControllable::Update(float pDeltaTime)
{
	mCameraController.HandleInputs(pDeltaTime);

	Editor_AView::Update(pDeltaTime);
}

Editor::Editor_CameraController& Editor::Editor_AViewControllable::GetCameraController()
{
	return mCameraController;
}
