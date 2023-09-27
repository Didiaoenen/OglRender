#include "Render_ShapeDrawer.h"

Render::Render_ShapeDrawer::Render_ShapeDrawer(Render_Renderer& pRenderer) :
	mRenderer(pRenderer)
{
}

Render::Render_ShapeDrawer::~Render_ShapeDrawer()
{
}

void Render::Render_ShapeDrawer::SetViewProjection(const glm::mat4& pViewProjection)
{
}

void Render::Render_ShapeDrawer::DrawLine(const glm::vec3& pStart, const glm::vec3& pEnd, const glm::vec3& pColor, float pLineWidth)
{
}

void Render::Render_ShapeDrawer::DrawGrid(const glm::vec3& pViewPos, const glm::vec3& pColor, int32_t pGridSize, float pLinear, float pQuadratic, float pFadeThreshold, float pLineWidth)
{
}
