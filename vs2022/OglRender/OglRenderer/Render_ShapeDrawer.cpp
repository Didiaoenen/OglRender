#include "Render_ShapeDrawer.h"

Render::Render_ShapeDrawer::Render_ShapeDrawer(Render_Renderer& p_renderer) :
	m_renderer(p_renderer)
{
}

Render::Render_ShapeDrawer::~Render_ShapeDrawer()
{
}

void Render::Render_ShapeDrawer::SetViewProjection(const glm::mat4& p_viewProjection)
{
}

void Render::Render_ShapeDrawer::DrawLine(const glm::vec3& p_start, const glm::vec3& p_end, const glm::vec3& pColor, float p_lineWidth)
{
}

void Render::Render_ShapeDrawer::DrawGrid(const glm::vec3& p_viewPos, const glm::vec3& pColor, int32_t p_gridSize, float pLinear, float pQuadratic, float p_fadeThreshold, float p_lineWidth)
{
}
