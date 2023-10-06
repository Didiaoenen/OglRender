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
	class Core_APass;

	class Core_Renderer : public Render::Render_Renderer
	{
	public:
		using Drawable = std::tuple<glm::mat4, Render::Render_Mesh*, Core_Material*, glm::mat4>;
		using OpaqueDrawables = std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables = std::multimap<float, Drawable, std::greater<float>>;

		Core_Renderer(Render::Render_Driver& pDriver);

		~Core_Renderer();

		Core_CCamera* FindMainCamera(const Core_Scene& pScene);

		std::vector<glm::mat4> FindLightMatrices(const Core_Scene& pScene);

		std::vector<glm::mat4> FindLightMatricesInFrustum(const Core_Scene& pScene, const Render::Render_Frustum& pFrustum);

		void RenderScene
		(
			Core_Scene& pScene,
			const glm::vec3& pCameraPosition,
			const Render::Render_Camera& pCamera,
			const Render::Render_Frustum* pCustomFrustum = nullptr,
			Core_Material* pDefaultMaterial = nullptr
		);

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortFrustumCulledDrawables
		(
			const Core_Scene& pScene,
			const glm::vec3& pCameraPosition,
			const Render::Render_Frustum& pFrustum,
			Core_Material* pDefaultMaterial
		);

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const Core_Scene& pScene,
			const glm::vec3& pCameraPosition,
			Core_Material* pDefaultMaterial
		);

		void DrawDrawable(const Drawable& pToDraw, const std::string& pProgramName);

		void DrawModelWithSingleMaterial(Render::Render_Model& pModel, Core_Material& pMaterial, glm::mat4 const* pModelMatrix, Core_Material* pDefaultMaterial = nullptr);

		void DrawModelWithMaterials(Render::Render_Model& pModel, std::vector<Core_Material*> pMaterials, glm::mat4 const* pModelMatrix, Core_Material* pDefaultMaterial = nullptr);

		void DrawMesh(Render::Render_Mesh& pMesh, Core_Material& pMaterial, const std::string& pProgramName, glm::mat4 const* pModelMatrix);

		void RegisterModelMatrixSender(std::function<void(glm::mat4)> pModelMatrixSender);

		void RegisterUserMatrixSender(std::function<void(glm::mat4)> pUserMatrixSender);

	private:
		std::function<void(glm::mat4)> mModelMatrixSender;
		std::function<void(glm::mat4)> mUserMatrixSender;
		Render::Render_Texture* mEmptyTexture{ nullptr };

		std::vector<Core_APass*> mPasses;
	};
}

