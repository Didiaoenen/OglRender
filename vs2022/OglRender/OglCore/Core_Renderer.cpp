
#include <OglRenderer/Render_Frustum.h>
#include <OglRenderer/Render_TextureLoader.h>

#include "Core_CMaterialRenderer.h"
#include "Core_CModelRenderer.h"
#include "Core_Renderer.h"

Core::Core_Renderer::Core_Renderer(Render::Render_Driver& p_driver) :
	Render_Renderer(p_driver),
	m_emptyTexture(Render::Render_TextureLoader::CreateColor
	(
		(255 << 24) | (255 << 16) | (255 << 8) | 255,
		Render::ETextureFilteringMode::NEAREST,
		Render::ETextureFilteringMode::NEAREST,
		false
	))
{
}

Core::Core_Renderer::~Core_Renderer()
{
}

Core::Core_CCamera* Core::Core_Renderer::FindMainCamera(const Core::Core_Scene& p_scene)
{
	return nullptr;
}

std::vector<glm::mat4> Core::Core_Renderer::FindLightMatrices(const Core::Core_Scene& p_scene)
{
	return std::vector<glm::mat4>();
}

std::vector<glm::mat4> Core::Core_Renderer::FindLightMatricesInFrustum(const Core::Core_Scene& p_scene, const Render::Render_Frustum& p_frustum)
{
	return std::vector<glm::mat4>();
}

void Core::Core_Renderer::RenderScene(Core::Core_Scene& p_scene, const glm::vec3& p_cameraPosition, const Render::Render_Camera& p_camera, const Render::Render_Frustum* p_customFrustum, Core::Core_Material* p_defaultMaterial)
{
}

std::pair<Core::Core_Renderer::OpaqueDrawables, Core::Core_Renderer::TransparentDrawables> Core::Core_Renderer::FindAndSortFrustumCulledDrawables(const Core::Core_Scene& p_scene, const glm::vec3& p_cameraPosition, const Render::Render_Frustum& p_frustum, Core::Core_Material* p_defaultMaterial)
{
	return std::pair<OpaqueDrawables, TransparentDrawables>();
}

std::pair<Core::Core_Renderer::OpaqueDrawables, Core::Core_Renderer::TransparentDrawables> Core::Core_Renderer::FindAndSortDrawables(const Core::Core_Scene& p_scene, const glm::vec3& p_cameraPosition, Core::Core_Material* p_defaultMaterial)
{
	return std::pair<OpaqueDrawables, TransparentDrawables>();
}

void Core::Core_Renderer::DrawDrawable(const Drawable& p_toDraw)
{
}

void Core::Core_Renderer::DrawModelWithSingleMaterial(Render::Render_Model& p_model, Core::Core_Material& p_material, glm::mat4 const* p_modelMatrix, Core::Core_Material* p_defaultMaterial)
{
}

void Core::Core_Renderer::DrawModelWithMaterials(Render::Render_Model& p_model, std::vector<Core::Core_Material*> p_materials, glm::mat4 const* p_modelMatrix, Core::Core_Material* p_defaultMaterial)
{
}

void Core::Core_Renderer::DrawMesh(Render::Render_Mesh& p_mesh, Core::Core_Material& p_material, glm::mat4 const* p_modelMatrix)
{
}

void Core::Core_Renderer::RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender)
{
}

void Core::Core_Renderer::RegisterUserMatrixSender(std::function<void(glm::mat4)> p_userMatrixSender)
{
}
