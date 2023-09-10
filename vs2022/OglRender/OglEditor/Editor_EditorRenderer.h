#pragma once

#include <OglRenderer/Render_Camera.h>

#include <OglCore/Core_Actor.h>
#include <OglCore/Core_Material.h>
#include <OglCore/Core_SceneManager.h>
#include <OglCore/Core_CModelRenderer.h>
#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include "Editor_Context.h"

namespace Editor
{
	class Editor_AView;

	enum class EGizmoOperation;

	class Editor_EditorRenderer
	{
	public:
		Editor_EditorRenderer(Editor_Context& p_context);

		void InitMaterials();

		void PreparePickingMaterial(Core::Core_Actor& p_actor, Core::Core_Material& p_material);

		glm::mat4 CalculateCameraModelMatrix(Core::Core_Actor& p_actor);

		void RenderScene(const glm::vec3& p_cameraPosition, const Render::Render_Camera& p_camera, const Render::Render_Frustum* p_customFrustum = nullptr);

		void RenderSceneForActorPicking();

		void RenderUI();

		void RenderCameras();

		void RenderLights();

		void RenderGizmo(const glm::vec3& p_position, const glm::quat& p_rotation, EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis = -1);

		void RenderModelToStencil(const glm::mat4& p_worldMatrix, Render::Render_Model& p_model);

		void RenderModelOutline(const glm::mat4& p_worldMatrix, Render::Render_Model& p_model, float p_width);

		void RenderActorOutlinePass(Core::Core_Actor& p_actor, bool p_toStencil, bool p_isSelected = false);

		void RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, Core::Core_CCamera& p_camera);
		
		void RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, Core::Core_CCamera& p_camera);

		void RenderCameraFrustum(Core::Core_CCamera& p_camera);

		void RenderActorCollider(Core::Core_Actor& p_actor);

		void RenderLightBounds(Core::Core_CLight& p_light);

		void RenderAmbientBoxVolume(Core::Core_CAmbientBoxLight& p_ambientBoxLight);

		void RenderAmbientSphereVolume(Core::Core_CAmbientSphereLight& p_ambientSphereLight);

		void RenderBoundingSpheres(Core::Core_CModelRenderer& p_modelRenderer);

		void RenderModelAsset(Render::Render_Model& p_model);

		void RenderTextureAsset(Render::Render_Texture& p_texture);

		void RenderMaterialAsset(Core::Core_Material& p_material);

		void RenderGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color);

		void UpdateLights(Core::Core_Scene& p_scene);

		void UpdateLightsInFrustum(Core::Core_Scene& p_scene, const Render::Render_Frustum& p_frustum);

	private:
		Editor_Context& m_context;

		Core::Core_Material m_gridMaterial;
		Core::Core_Material m_stencilFillMaterial;
		Core::Core_Material m_textureMaterial;
		Core::Core_Material m_outlineMaterial;
		Core::Core_Material m_emptyMaterial;
		Core::Core_Material m_defaultMaterial;
		Core::Core_Material m_cameraMaterial;
		Core::Core_Material m_lightMaterial;
		Core::Core_Material m_gizmoArrowMaterial;
		Core::Core_Material m_gizmoBallMaterial;
		Core::Core_Material m_gizmoPickingMaterial;
		Core::Core_Material m_actorPickingMaterial;
	};
}

