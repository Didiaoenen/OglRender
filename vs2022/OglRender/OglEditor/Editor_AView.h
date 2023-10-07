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

	class Editor_AView : public UI::UI_PanelWindow
	{
	public:
		Editor_AView
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		virtual void Update(float pDeltaTime);

		void _Draw_Impl() override;

		virtual void _Render_Impl() = 0;

		void Render();

		void SetCameraPosition(const glm::vec3& pPosition);

		void SetCameraRotation(const glm::quat& pRotation);

		const glm::vec3& GetCameraPosition() const;

		const glm::quat& GetCameraRotation() const;

		Render::Render_Camera& GetCamera();

		std::pair<uint16_t, uint16_t> GetSafeSize() const;

		const glm::vec3& GetGridColor() const;

		void SetGridColor(const glm::vec3& pColor);

		void FillEngineUBO();

	protected:
		void PrepareCamera();

	protected:
		Editor_EditorRenderer& mEditorRenderer;
		Render::Render_Camera mCamera;
		glm::vec3 mCameraPosition;
		glm::quat mCameraRotation;
		UI::UI_Image* mImage;

		glm::vec3 mGridColor = glm::vec3{ 1.f, 0.f, 0.f };

		Render::Render_Framebuffer mFbo;
	};
}

