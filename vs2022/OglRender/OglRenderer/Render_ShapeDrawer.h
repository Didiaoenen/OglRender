#pragma once

#include "Render_Mesh.h"
#include "Render_Renderer.h"

namespace Render
{
	class Render_ShapeDrawer
	{
	public:
		Render_ShapeDrawer(Render_Renderer& p_renderer);

		~Render_ShapeDrawer();

		void SetViewProjection(const glm::mat4& p_viewProjection);

		void DrawLine(const glm::vec3& p_start, const glm::vec3& p_end, const glm::vec3& pColor, float p_lineWidth = 1.0f);

		void DrawGrid(const glm::vec3& p_viewPos, const glm::vec3& pColor, int32_t p_gridSize = 50, float pLinear = 0.0f, float pQuadratic = 0.0f, float p_fadeThreshold = 0.0f, float p_lineWidth = 1.0f);

	private:
		Render_Shader* m_lineShader = nullptr;
		Render_Shader* m_gridShader = nullptr;
		Render_Mesh* m_lineMesh = nullptr;
		Render_Renderer& mRenderer;
	};
}

