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
		Editor_EditorRenderer(Editor_Context& pContext);

		void InitMaterials();

		void PreparePickingMaterial(Core::Core_Actor& pActor, Core::Core_Material& pMaterial);

		glm::mat4 CalculateCameraModelMatrix(Core::Core_Actor& pActor);

		void RenderScene(const glm::vec3& pCameraPosition, const Render::Render_Camera& pCamera, const Render::Render_Frustum* pCustomFrustum = nullptr);

		void RenderSceneForActorPicking();

		void RenderUI();

		void RenderCameras();

		void RenderLights();

		void RenderGizmo(const glm::vec3& pPosition, const glm::quat& pRotation, EGizmoOperation pOperation, bool pPickable, int pHighlightedAxis = -1);

		void RenderModelToStencil(const glm::mat4& pWorldMatrix, Render::Render_Model& pModel);

		void RenderModelOutline(const glm::mat4& pWorldMatrix, Render::Render_Model& pModel, float pWidth);

		void RenderActorOutlinePass(Core::Core_Actor& pActor, bool pToStencil, bool pIsSelected = false);

		void RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& pSize, Core::Core_CCamera& pCamera);
		
		void RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& pSize, Core::Core_CCamera& pCamera);

		void RenderCameraFrustum(Core::Core_CCamera& pCamera);

		void RenderActorCollider(Core::Core_Actor& pActor);

		void RenderLightBounds(Core::Core_CLight& pLight);

		void RenderAmbientBoxVolume(Core::Core_CAmbientBoxLight& pAmbientBoxLight);

		void RenderAmbientSphereVolume(Core::Core_CAmbientSphereLight& pAmbientSphereLight);

		void RenderBoundingSpheres(Core::Core_CModelRenderer& pModelRenderer);

		void RenderModelAsset(Render::Render_Model& pModel);

		void RenderTextureAsset(Render::Render_Texture& pTexture);

		void RenderMaterialAsset(Core::Core_Material& pMaterial);

		void RenderGrid(const glm::vec3& pViewPos, const glm::vec3& pColor);

		void UpdateLights(Core::Core_Scene& pScene);

		void UpdateLightsInFrustum(Core::Core_Scene& pScene, const Render::Render_Frustum& pFrustum);

	private:
		Editor_Context& mContext;

		Core::Core_Material mGridMaterial;
		Core::Core_Material mStencilFillMaterial;
		Core::Core_Material mTextureMaterial;
		Core::Core_Material mOutlineMaterial;
		Core::Core_Material mEmptyMaterial;
		Core::Core_Material mDefaultMaterial;
		Core::Core_Material mCameraMaterial;
		Core::Core_Material mLightMaterial;
		Core::Core_Material mGizmoArrowMaterial;
		Core::Core_Material mGizmoBallMaterial;
		Core::Core_Material mGizmoPickingMaterial;
		Core::Core_Material mActorPickingMaterial;
	};
}

