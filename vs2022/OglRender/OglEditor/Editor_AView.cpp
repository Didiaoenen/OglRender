#include <GL/glew.h>

#include "Editor_AView.h"
#include "Editor_EditorActions.h"

Editor::Editor_AView::Editor_AView(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	UI_PanelWindow(p_title, p_opened, p_windowSettings), m_editorRenderer(EDITOR_RENDERER())
{
}

void Editor::Editor_AView::Update(float p_deltaTime)
{
}

void Editor::Editor_AView::_Draw_Impl()
{
}

void Editor::Editor_AView::Render()
{
}

void Editor::Editor_AView::SetCameraPosition(const glm::vec3& p_position)
{
}

void Editor::Editor_AView::SetCameraRotation(const glm::quat& p_rotation)
{
}

const glm::vec3& Editor::Editor_AView::GetCameraPosition() const
{
	return m_cameraPosition;
}

const glm::quat& Editor::Editor_AView::GetCameraRotation() const
{
	return m_cameraRotation;
}

Render::Render_Camera& Editor::Editor_AView::GetCamera()
{
	return m_camera;
}

std::pair<uint16_t, uint16_t> Editor::Editor_AView::GetSafeSize() const
{
	return std::pair<uint16_t, uint16_t>();
}

const glm::vec3& Editor::Editor_AView::GetGridColor() const
{
	return m_gridColor;
}

void Editor::Editor_AView::SetGridColor(const glm::vec3& p_color)
{
}

void Editor::Editor_AView::FillEngineUBO()
{
}

void Editor::Editor_AView::PrepareCamera()
{
}
