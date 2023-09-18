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

		Core_Renderer(Render::Render_Driver& pDriver);

		~Core_Renderer();

		Core::Core_CCamera* FindMainCamera(const Core::Core_Scene& pScene);

		std::vector<glm::mat4> FindLightMatrices(const Core::Core_Scene& pScene);

		std::vector<glm::mat4> FindLightMatricesInFrustum(const Core::Core_Scene& pScene, const Render::Render_Frustum& pFrustum);

		void RenderScene
		(
			Core::Core_Scene& pScene,
			const glm::vec3& pCameraPosition,
			const Render::Render_Camera& pCamera,
			const Render::Render_Frustum* pCustomFrustum = nullptr,
			Core::Core_Material* pDefaultMaterial = nullptr
		);

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortFrustumCulledDrawables
		(
			const Core::Core_Scene& pScene,
			const glm::vec3& pCameraPosition,
			const Render::Render_Frustum& pFrustum,
			Core::Core_Material* pDefaultMaterial
		);

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const Core::Core_Scene& pScene,
			const glm::vec3& pCameraPosition,
			Core::Core_Material* pDefaultMaterial
		);

		void DrawDrawable(const Drawable& pToDraw);

		void DrawModelWithSingleMaterial(Render::Render_Model& pModel, Core::Core_Material& pMaterial, glm::mat4 const* pModelMatrix, Core::Core_Material* pDefaultMaterial = nullptr);

		void DrawModelWithMaterials(Render::Render_Model& pModel, std::vector<Core::Core_Material*> pMaterials, glm::mat4 const* pModelMatrix, Core::Core_Material* pDefaultMaterial = nullptr);

		void DrawMesh(Render::Render_Mesh& pMesh, Core::Core_Material& pMaterial, glm::mat4 const* pModelMatrix);

		void RegisterModelMatrixSender(std::function<void(glm::mat4)> pModelMatrixSender);

		void RegisterUserMatrixSender(std::function<void(glm::mat4)> pUserMatrixSender);

	private:
		std::function<void(glm::mat4)> mModelMatrixSender;
		std::function<void(glm::mat4)> mUserMatrixSender;
		Render::Render_Texture* mEmptyTexture{ nullptr };
	};
}

