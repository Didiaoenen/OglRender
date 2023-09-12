#pragma once

#include <map>

#include <OglRenderer/Render_Mesh.h>
#include <OglRenderer/Render_Renderer.h>
#include <OglRenderer/Render_Frustum.h>

#include "Core_Material.h"
#include "Core_CCamera.h"
#include "Core_Actor.h"
#include "Core_Scene.h"

namespace Core
{
	class Core_Renderer : public Render::Render_Renderer
	{
	public:
		using Drawable = std::tuple<glm::mat4, Render::Render_Mesh*, Core::Core_Material*, glm::mat4>;
		using OpaqueDrawables = std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables = std::multimap<float, Drawable, std::greater<float>>;

		Core_Renderer(Render::Render_Driver& p_driver);

		~Core_Renderer();

		Core::Core_CCamera* FindMainCamera(const Core::Core_Scene& p_scene);

		std::vector<glm::mat4> FindLightMatrices(const Core::Core_Scene& p_scene);

		std::vector<glm::mat4> FindLightMatricesInFrustum(const Core::Core_Scene& p_scene, const Render::Render_Frustum& p_frustum);

		void RenderScene
		(
			Core::Core_Scene& p_scene,
			const glm::vec3& p_cameraPosition,
			const Render::Render_Camera& p_camera,
			const Render::Render_Frustum* p_customFrustum = nullptr,
			Core::Core_Material* p_defaultMaterial = nullptr
		);

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortFrustumCulledDrawables
		(
			const Core::Core_Scene& p_scene,
			const glm::vec3& p_cameraPosition,
			const Render::Render_Frustum& p_frustum,
			Core::Core_Material* p_defaultMaterial
		);

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const Core::Core_Scene& p_scene,
			const glm::vec3& p_cameraPosition,
			Core::Core_Material* p_defaultMaterial
		);

		void DrawDrawable(const Drawable& p_toDraw);

		void DrawModelWithSingleMaterial(Render::Render_Model& pModel, Core::Core_Material& pMaterial, glm::mat4 const* p_modelMatrix, Core::Core_Material* p_defaultMaterial = nullptr);

		void DrawModelWithMaterials(Render::Render_Model& pModel, std::vector<Core::Core_Material*> p_materials, glm::mat4 const* p_modelMatrix, Core::Core_Material* p_defaultMaterial = nullptr);

		void DrawMesh(Render::Render_Mesh& p_mesh, Core::Core_Material& pMaterial, glm::mat4 const* p_modelMatrix);

		void RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender);

		void RegisterUserMatrixSender(std::function<void(glm::mat4)> p_userMatrixSender);

	private:
		std::function<void(glm::mat4)> m_modelMatrixSender;
		std::function<void(glm::mat4)> m_userMatrixSender;
		Render::Render_Texture* m_emptyTexture{ nullptr };
	};
}

