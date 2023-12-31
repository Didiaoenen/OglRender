#pragma once

#include "Render_Mesh.h"
#include "Render_Renderer.h"

namespace Render
{
	class Render_ShapeDrawer
	{
	public:
		Render_ShapeDrawer(Render_Renderer& pRenderer);

		~Render_ShapeDrawer();

		void SetViewProjection(const glm::mat4& pViewProjection);

		void DrawLine(const glm::vec3& p_start, const glm::vec3& pEnd, const glm::vec3& pColor, float pLineWidth = 1.0f);

		void DrawGrid(const glm::vec3& pViewPos, const glm::vec3& pColor, int32_t pGridSize = 50, float pLinear = 0.0f, float pQuadratic = 0.0f, float pFadeThreshold = 0.0f, float pLineWidth = 1.0f);

	private:
		Render_Shader* mLineShader{ nullptr };
		Render_Shader* mGridShader{ nullptr };
		Render_Mesh* mLineMesh{ nullptr };
		Render_Renderer& mRenderer;
	};
}

