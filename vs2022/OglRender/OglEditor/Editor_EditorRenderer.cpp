#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CMaterialRenderer.h>
#include <OglCore/Core_CDirectionalLight.h>

#include "Editor_AView.h"
#include "Editor_GameView.h"
#include "Editor_EditorActions.h"
#include "Editor_GizmoBehaviour.h"
#include "Editor_EditorSettings.h"
#include "Editor_EditorResources.h"

#include "Editor_EditorRenderer.h"

Editor::Editor_EditorRenderer::Editor_EditorRenderer(Editor_Context& p_context) :
	m_context(p_context)
{
}

void Editor::Editor_EditorRenderer::InitMaterials()
{
}

void Editor::Editor_EditorRenderer::PreparePickingMaterial(Core::Core_Actor& p_actor, Core::Core_Material& p_material)
{
}

glm::mat4 Editor::Editor_EditorRenderer::CalculateCameraModelMatrix(Core::Core_Actor& p_actor)
{
	return glm::mat4();
}

void Editor::Editor_EditorRenderer::RenderScene(const glm::vec3& p_cameraPosition, const Render::Render_Camera& p_camera, const Render::Render_Frustum* p_customFrustum)
{
}

void Editor::Editor_EditorRenderer::RenderSceneForActorPicking()
{
}

void Editor::Editor_EditorRenderer::RenderUI()
{
}

void Editor::Editor_EditorRenderer::RenderCameras()
{
}

void Editor::Editor_EditorRenderer::RenderLights()
{
}

void Editor::Editor_EditorRenderer::RenderGizmo(const glm::vec3& p_position, const glm::quat& p_rotation, EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis)
{
}

void Editor::Editor_EditorRenderer::RenderModelToStencil(const glm::mat4& p_worldMatrix, Render::Render_Model& p_model)
{
}

void Editor::Editor_EditorRenderer::RenderModelOutline(const glm::mat4& p_worldMatrix, Render::Render_Model& p_model, float p_width)
{
}

void Editor::Editor_EditorRenderer::RenderActorOutlinePass(Core::Core_Actor& p_actor, bool p_toStencil, bool p_isSelected)
{
}

void Editor::Editor_EditorRenderer::RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, Core::Core_CCamera& p_camera)
{
}

void Editor::Editor_EditorRenderer::RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, Core::Core_CCamera& p_camera)
{
}

void Editor::Editor_EditorRenderer::RenderCameraFrustum(Core::Core_CCamera& p_camera)
{
}

void Editor::Editor_EditorRenderer::RenderActorCollider(Core::Core_Actor& p_actor)
{
}

void Editor::Editor_EditorRenderer::RenderLightBounds(Core::Core_CLight& p_light)
{
}

void Editor::Editor_EditorRenderer::RenderAmbientBoxVolume(Core::Core_CAmbientBoxLight& p_ambientBoxLight)
{
}

void Editor::Editor_EditorRenderer::RenderAmbientSphereVolume(Core::Core_CAmbientSphereLight& p_ambientSphereLight)
{
}

void Editor::Editor_EditorRenderer::RenderBoundingSpheres(Core::Core_CModelRenderer& p_modelRenderer)
{
}

void Editor::Editor_EditorRenderer::RenderModelAsset(Render::Render_Model& p_model)
{
}

void Editor::Editor_EditorRenderer::RenderTextureAsset(Render::Render_Texture& p_texture)
{
}

void Editor::Editor_EditorRenderer::RenderMaterialAsset(Core::Core_Material& p_material)
{
}

void Editor::Editor_EditorRenderer::RenderGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color)
{
}

void Editor::Editor_EditorRenderer::UpdateLights(Core::Core_Scene& p_scene)
{
}

void Editor::Editor_EditorRenderer::UpdateLightsInFrustum(Core::Core_Scene& p_scene, const Render::Render_Frustum& p_frustum)
{
}
