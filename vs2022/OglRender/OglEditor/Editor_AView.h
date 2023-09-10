#pragma once

#include <optional>

#include <OglUI/UI_Image.h>
#include <OglUI/UI_PanelWindow.h>
#include <OglRenderer/Render_Camera.h>
#include <OglRenderer/Render_Framebuffer.h>
#include <OglRenderer/Render_UniformBuffer.h>

namespace Editor
{
	class Editor_EditorRenderer;

	class Editor_AView : UI::UI_PanelWindow
	{
	public:
		Editor_AView
		(
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		virtual void Update(float p_deltaTime);

		void _Draw_Impl() override;

		virtual void _Render_Impl() = 0;

		void Render();

		void SetCameraPosition(const glm::vec3& p_position);

		void SetCameraRotation(const glm::quat& p_rotation);

		const glm::vec3& GetCameraPosition() const;

		const glm::quat& GetCameraRotation() const;

		Render::Render_Camera& GetCamera();

		std::pair<uint16_t, uint16_t> GetSafeSize() const;

		const glm::vec3& GetGridColor() const;

		void SetGridColor(const glm::vec3& p_color);

		void FillEngineUBO();

	protected:
		void PrepareCamera();

	protected:
		Editor_EditorRenderer& m_editorRenderer;
		Render::Render_Camera m_camera;
		glm::vec3 m_cameraPosition;
		glm::quat m_cameraRotation;
		UI::UI_Image* m_image;

		glm::vec3 m_gridColor = glm::vec3{ 0.176f, 0.176f, 0.176f };

		Render::Render_Framebuffer m_fbo;
	};
}

