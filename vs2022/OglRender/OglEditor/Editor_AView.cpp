#include <GL/glew.h>

#include "Editor_AView.h"
#include "Editor_EditorActions.h"

Editor::Editor_AView::Editor_AView(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	UI_PanelWindow(pTitle, pOpened, pWindowSettings), mEditorRenderer(EDITOR_RENDERER())
{
	mCameraPosition = { -10.0f, 3.0f, 10.0f };
	mCameraRotation = glm::quat({ 0.0f, 135.0f, 0.0f });

	mImage = &CreateWidget<UI::UI_Image>(mFbo.GetTextureID(), glm::vec2{ 0.f, 0.f });

	mScrollable = false;
}

void Editor::Editor_AView::Update(float pDeltaTime)
{
	auto [winWidth, winHeight] = GetSafeSize();

	mImage->mSize = glm::vec2(static_cast<float>(winWidth), static_cast<float>(winHeight));

	mFbo.Resize(winWidth, winHeight);
}

void Editor::Editor_AView::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	UI::UI_PanelWindow::_Draw_Impl();

	ImGui::PopStyleVar();
}

void Editor::Editor_AView::Render()
{
	FillEngineUBO();

	auto [winWidth, winHeight] = GetSafeSize();

	EDITOR_CONTEXT(shapeDrawer)->SetViewProjection(mCamera.GetProjectionMatrix() * mCamera.GetViewMatrix());

	EDITOR_CONTEXT(renderer)->SetViewPort(0, 0, winWidth, winHeight);

	_Render_Impl();
}

void Editor::Editor_AView::SetCameraPosition(const glm::vec3& pPosition)
{
	mCameraPosition = pPosition;
}

void Editor::Editor_AView::SetCameraRotation(const glm::quat& pRotation)
{
	mCameraRotation = pRotation;
}

const glm::vec3& Editor::Editor_AView::GetCameraPosition() const
{
	return mCameraPosition;
}

const glm::quat& Editor::Editor_AView::GetCameraRotation() const
{
	return mCameraRotation;
}

Render::Render_Camera& Editor::Editor_AView::GetCamera()
{
	return mCamera;
}

std::pair<uint16_t, uint16_t> Editor::Editor_AView::GetSafeSize() const
{
	auto result = GetSize() - glm::vec2{ 0.f, 25.f };
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

const glm::vec3& Editor::Editor_AView::GetGridColor() const
{
	return mGridColor;
}

void Editor::Editor_AView::SetGridColor(const glm::vec3& pColor)
{
	mGridColor = pColor;
}

void Editor::Editor_AView::FillEngineUBO()
{
	auto& engineUBO = *EDITOR_CONTEXT(engineUBO);

	auto [winWidth, winHeight] = GetSafeSize();

	size_t offset = sizeof(glm::mat4);
	engineUBO.SetSubData(glm::transpose(mCamera.GetViewMatrix()), std::ref(offset));
	engineUBO.SetSubData(glm::transpose(mCamera.GetProjectionMatrix()), std::ref(offset));
	engineUBO.SetSubData(mCameraPosition, std::ref(offset));
}

void Editor::Editor_AView::PrepareCamera()
{
	auto [winWidth, winHeight] = GetSafeSize();
	mCamera.CacheMatrices(winWidth, winHeight, mCameraPosition, mCameraRotation);
}
