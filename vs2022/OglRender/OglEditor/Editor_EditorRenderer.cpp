#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CMaterialRenderer.h>
#include <OglCore/Core_CDirectionalLight.h>

#include <OglMaths/Maths_FQuaternionExt.h>

#include "Editor_AView.h"
#include "Editor_GameView.h"
#include "Editor_EditorActions.h"
#include "Editor_GizmoBehaviour.h"
#include "Editor_EditorSettings.h"
#include "Editor_EditorResources.h"

#include "Editor_EditorRenderer.h"

const glm::vec3 DEBUG_BOUNDS_COLOR = { 1.0f, 0.0f, 0.0f };
const glm::vec3 LIGHT_VOLUME_COLOR = { 1.0f, 1.0f, 0.0f };
const glm::vec3 COLLIDER_COLOR = { 0.0f, 1.0f, 0.0f };
const glm::vec3 FRUSTUM_COLOR = { 1.0f, 1.0f, 1.0f };

Editor::Editor_EditorRenderer::Editor_EditorRenderer(Editor_Context& pContext) :
	mContext(pContext)
{
	mContext.mRenderer->SetCapability(Render::ERenderingCapability::STENCIL_TEST, true);
	mContext.mRenderer->SetStencilOperations(Render::EOperation::KEEP, Render::EOperation::KEEP, Render::EOperation::REPLACE);
	mContext.mRenderer->SetStencilAlgorithm(Render::EComparaisonAlgorithm::ALWAYS, 1, 0xFF);

	InitMaterials();

	mContext.mRenderer->RegisterModelMatrixSender([this](const glm::mat4& pModelMatrix)
		{
			mContext.mEngineUBO->SetSubData(glm::transpose(pModelMatrix), 0);
		});

	mContext.mRenderer->RegisterUserMatrixSender([this](const glm::mat4& p_userMatrix)
		{
			mContext.mEngineUBO->SetSubData
			(
				p_userMatrix,

				sizeof(glm::mat4) +
				sizeof(glm::mat4) +
				sizeof(glm::mat4) +
				sizeof(glm::vec3) +
				sizeof(float)
			);
		});
}

void Editor::Editor_EditorRenderer::InitMaterials()
{
	m_defaultMaterial.SetShader(mContext.mShaderManager[":Shaders\\Standard.glsl"]);
	m_defaultMaterial.Set("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_defaultMaterial.Set("u_Shininess", 100.0f);
	m_defaultMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);

	m_emptyMaterial.SetShader(mContext.mShaderManager[":Shaders\\Unlit.glsl"]);
	m_emptyMaterial.Set("u_Diffuse", glm::vec4(1.f, 0.f, 1.f, 1.0f));
	m_emptyMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);

	m_gridMaterial.SetShader(mContext.mEditorResources->GetShader("Grid"));
	m_gridMaterial.SetBlendable(true);
	m_gridMaterial.SetBackfaceCulling(false);
	m_gridMaterial.SetDepthTest(false);

	m_cameraMaterial.SetShader(mContext.mShaderManager[":Shaders\\Lambert.glsl"]);
	m_cameraMaterial.Set("u_Diffuse", glm::vec4(0.0f, 0.3f, 0.7f, 1.0f));
	m_cameraMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);

	m_lightMaterial.SetShader(mContext.mEditorResources->GetShader("Billboard"));
	m_lightMaterial.Set("u_Diffuse", glm::vec4(1.f, 1.f, 0.5f, 0.5f));
	m_lightMaterial.SetBackfaceCulling(false);
	m_lightMaterial.SetBlendable(true);
	m_lightMaterial.SetDepthTest(false);

	m_stencilFillMaterial.SetShader(mContext.mShaderManager[":Shaders\\Unlit.glsl"]);
	m_stencilFillMaterial.SetBackfaceCulling(true);
	m_stencilFillMaterial.SetDepthTest(false);
	m_stencilFillMaterial.SetColorWriting(false);
	m_stencilFillMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);

	m_textureMaterial.SetShader(mContext.mShaderManager[":Shaders\\Unlit.glsl"]);
	m_textureMaterial.Set("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_textureMaterial.SetBackfaceCulling(false);
	m_textureMaterial.SetBlendable(true);
	m_textureMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);

	m_outlineMaterial.SetShader(mContext.mShaderManager[":Shaders\\Unlit.glsl"]);
	m_outlineMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);
	m_outlineMaterial.SetDepthTest(false);

	m_gizmoArrowMaterial.SetShader(mContext.mEditorResources->GetShader("Gizmo"));
	m_gizmoArrowMaterial.SetGPUInstances(3);
	m_gizmoArrowMaterial.Set("u_IsBall", false);
	m_gizmoArrowMaterial.Set("u_IsPickable", false);

	m_gizmoBallMaterial.SetShader(mContext.mEditorResources->GetShader("Gizmo"));
	m_gizmoBallMaterial.Set("u_IsBall", true);
	m_gizmoBallMaterial.Set("u_IsPickable", false);

	m_gizmoPickingMaterial.SetShader(mContext.mEditorResources->GetShader("Gizmo"));
	m_gizmoPickingMaterial.SetGPUInstances(3);
	m_gizmoPickingMaterial.Set("u_IsBall", false);
	m_gizmoPickingMaterial.Set("u_IsPickable", true);

	m_actorPickingMaterial.SetShader(mContext.mShaderManager[":Shaders\\Unlit.glsl"]);
	m_actorPickingMaterial.Set("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.0f));
	m_actorPickingMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);
	m_actorPickingMaterial.SetFrontfaceCulling(false);
	m_actorPickingMaterial.SetBackfaceCulling(false);
}

void Editor::Editor_EditorRenderer::PreparePickingMaterial(Core::Core_Actor& p_actor, Core::Core_Material& pMaterial)
{
	uint32_t actorID = static_cast<uint32_t>(p_actor.GetID());

	auto bytes = reinterpret_cast<uint8_t*>(&actorID);
	auto color = glm::vec4{ bytes[0] / 255.0f, bytes[1] / 255.0f, bytes[2] / 255.0f, 1.0f };

	pMaterial.Set("u_Diffuse", color);
}

glm::mat4 Editor::Editor_EditorRenderer::CalculateCameraModelMatrix(Core::Core_Actor& p_actor)
{
	auto translation = glm::translate(glm::identity<glm::mat4>(), p_actor.transform.GetWorldPosition());
	auto rotation = glm::mat4(glm::quat(p_actor.transform.GetWorldRotation()));

	return translation * rotation;
}

void Editor::Editor_EditorRenderer::RenderScene(const glm::vec3& pCameraPosition, const Render::Render_Camera& pCamera, const Render::Render_Frustum* pCustomFrustum)
{
	mContext.mLightSSBO->Bind(0);
	mContext.mRenderer->RenderScene(*mContext.mSceneManager.GetCurrentScene(), pCameraPosition, pCamera, pCustomFrustum, &m_emptyMaterial);
	mContext.mLightSSBO->Unbind();
}

void Editor::Editor_EditorRenderer::RenderSceneForActorPicking()
{
	auto& scene = *mContext.mSceneManager.GetCurrentScene();

	for (auto modelRenderer : scene.GetFastAccessComponents().modelRenderers)
	{
		auto& actor = modelRenderer->mOwner;

		if (actor.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->mOwner.GetComponent<Core::Core_CMaterialRenderer>())
				{
					const Core::Core_CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();
					const auto& modelMatrix = actor.transform.GetWorldMatrix();

					PreparePickingMaterial(actor, m_actorPickingMaterial);

					for (auto mesh : model->GetMeshes())
					{
						Core::Core_Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = &m_emptyMaterial;
						}

						if (material)
						{
							m_actorPickingMaterial.SetBackfaceCulling(material->HasBackfaceCulling());
							m_actorPickingMaterial.SetFrontfaceCulling(material->HasFrontfaceCulling());
							m_actorPickingMaterial.SetColorWriting(material->HasColorWriting());
							m_actorPickingMaterial.SetDepthTest(material->HasDepthTest());
							m_actorPickingMaterial.SetDepthWriting(material->HasDepthWriting());

							mContext.mRenderer->DrawMesh(*mesh, m_actorPickingMaterial, &modelMatrix);
						}
					}
				}
			}
		}
	}

	for (auto camera : mContext.mSceneManager.GetCurrentScene()->GetFastAccessComponents().cameras)
	{
		auto& actor = camera->mOwner;

		if (actor.IsActive())
		{
			PreparePickingMaterial(actor, m_actorPickingMaterial);
			auto& model = *mContext.mEditorResources->GetModel("Camera");
			auto modelMatrix = CalculateCameraModelMatrix(actor);

			mContext.mRenderer->DrawModelWithSingleMaterial(model, m_actorPickingMaterial, &modelMatrix);
		}
	}

	if (Editor_EditorSettings::LightBillboardScale > 0.001f)
	{
		mContext.mRenderer->Clear(false, true, false);

		m_lightMaterial.SetDepthTest(true);
		m_lightMaterial.Set<float>("u_Scale", Editor_EditorSettings::LightBillboardScale * 0.1f);
		m_lightMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", nullptr);

		for (auto light : mContext.mSceneManager.GetCurrentScene()->GetFastAccessComponents().lights)
		{
			auto& actor = light->mOwner;

			if (actor.IsActive())
			{
				PreparePickingMaterial(actor, m_lightMaterial);
				auto& model = *mContext.mEditorResources->GetModel("Vertical_Plane");
				auto modelMatrix = glm::translate(glm::identity<glm::mat4>(), actor.transform.GetWorldPosition());
				mContext.mRenderer->DrawModelWithSingleMaterial(model, m_lightMaterial, &modelMatrix);
			}
		}
	}
}

void Editor::Editor_EditorRenderer::RenderUI()
{
	mContext.mUIManager->Render();
}

void Editor::Editor_EditorRenderer::RenderCameras()
{
	for (auto camera : mContext.mSceneManager.GetCurrentScene()->GetFastAccessComponents().cameras)
	{
		auto& actor = camera->mOwner;

		if (actor.IsActive())
		{
			auto& model = *mContext.mEditorResources->GetModel("Camera");
			auto modelMatrix = CalculateCameraModelMatrix(actor);

			mContext.mRenderer->DrawModelWithSingleMaterial(model, m_cameraMaterial, &modelMatrix);
		}
	}
}

void Editor::Editor_EditorRenderer::RenderLights()
{
	m_lightMaterial.SetDepthTest(false);
	m_lightMaterial.Set<float>("u_Scale", Editor_EditorSettings::LightBillboardScale * 0.1f);

	for (auto light : mContext.mSceneManager.GetCurrentScene()->GetFastAccessComponents().lights)
	{
		auto& actor = light->mOwner;

		if (actor.IsActive())
		{
			auto& model = *mContext.mEditorResources->GetModel("Vertical_Plane");
			auto modelMatrix = glm::translate(glm::identity<glm::mat4>(), actor.transform.GetWorldPosition());

			Render::Render_Texture* texture = nullptr;

			switch (static_cast<Render::Render_Light::ELightType>(static_cast<int>(light->GetData().mType)))
			{
			case Render::Render_Light::ELightType::POINT:				texture = mContext.mEditorResources->GetTexture("Bill_Point_Light");			break;
			case Render::Render_Light::ELightType::SPOT:				texture = mContext.mEditorResources->GetTexture("Bill_Spot_Light");				break;
			case Render::Render_Light::ELightType::DIRECTIONAL:		texture = mContext.mEditorResources->GetTexture("Bill_Directional_Light");		break;
			case Render::Render_Light::ELightType::AMBIENT_BOX:		texture = mContext.mEditorResources->GetTexture("Bill_Ambient_Box_Light");		break;
			case Render::Render_Light::ELightType::AMBIENT_SPHERE:	texture = mContext.mEditorResources->GetTexture("Bill_Ambient_Sphere_Light");	break;
			}

			const auto& lightColor = light->GetColor();
			m_lightMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", texture);
			m_lightMaterial.Set<glm::vec4>("u_Diffuse", glm::vec4(lightColor.x, lightColor.y, lightColor.z, 0.75f));
			mContext.mRenderer->DrawModelWithSingleMaterial(model, m_lightMaterial, &modelMatrix);
		}
	}
}

void Editor::Editor_EditorRenderer::RenderGizmo(const glm::vec3& pPosition, const glm::quat& pRotation, EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis)
{
	glm::mat4 model = glm::translate(glm::identity<glm::mat4>(), pPosition) * glm::mat4(glm::normalize(pRotation));

	Render::Render_Model* arrowModel = nullptr;

	if (!p_pickable)
	{
		glm::mat4 sphereModel = model * glm::scale(glm::identity<glm::mat4>(), {0.1f, 0.1f, 0.1f});
		mContext.mRenderer->DrawModelWithSingleMaterial(*mContext.mEditorResources->GetModel("Sphere"), m_gizmoBallMaterial, &sphereModel);
		m_gizmoArrowMaterial.Set("u_HighlightedAxis", p_highlightedAxis);

		switch (p_operation)
		{
		case EGizmoOperation::TRANSLATE:
			arrowModel = mContext.mEditorResources->GetModel("Arrow_Translate");
			break;
		case EGizmoOperation::ROTATE:
			arrowModel = mContext.mEditorResources->GetModel("Arrow_Rotate");
			break;
		case EGizmoOperation::SCALE:
			arrowModel = mContext.mEditorResources->GetModel("Arrow_Scale");
			break;
		}
	}
	else
	{
		arrowModel = mContext.mEditorResources->GetModel("Arrow_Picking");
	}

	if (arrowModel)
	{
		mContext.mRenderer->DrawModelWithSingleMaterial(*arrowModel, p_pickable ? m_gizmoPickingMaterial : m_gizmoArrowMaterial, &model);
	}
}

void Editor::Editor_EditorRenderer::RenderModelToStencil(const glm::mat4& p_worldMatrix, Render::Render_Model& pModel)
{
	mContext.mRenderer->SetStencilMask(0xFF);
	mContext.mRenderer->DrawModelWithSingleMaterial(pModel, m_stencilFillMaterial, &p_worldMatrix);
	mContext.mRenderer->SetStencilMask(0x00);
}

void Editor::Editor_EditorRenderer::RenderModelOutline(const glm::mat4& p_worldMatrix, Render::Render_Model& pModel, float pWidth)
{
	mContext.mRenderer->SetStencilAlgorithm(Render::EComparaisonAlgorithm::NOTEQUAL, 1, 0xFF);
	mContext.mRenderer->SetRasterizationMode(Render::ERasterizationMode::LINE);
	mContext.mRenderer->SetRasterizationLinesWidth(pWidth);
	mContext.mRenderer->DrawModelWithSingleMaterial(pModel, m_outlineMaterial, &p_worldMatrix);
	mContext.mRenderer->SetRasterizationLinesWidth(1.f);
	mContext.mRenderer->SetRasterizationMode(Render::ERasterizationMode::FILL);
	mContext.mRenderer->SetStencilAlgorithm(Render::EComparaisonAlgorithm::ALWAYS, 1, 0xFF);
}

void Editor::Editor_EditorRenderer::RenderActorOutlinePass(Core::Core_Actor& p_actor, bool p_toStencil, bool p_isSelected)
{
	float outlineWidth = p_isSelected ? 5.0f : 2.5f;

	m_outlineMaterial.Set("u_Diffuse", p_isSelected ? glm::vec4(1.f, 0.7f, 0.f, 1.0f) : glm::vec4(1.f, 1.f, 0.f, 1.0f));

	if (p_actor.IsActive())
	{
		if (auto modelRenderer = p_actor.GetComponent<Core::Core_CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			if (p_toStencil)
				RenderModelToStencil(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel());
			else
				RenderModelOutline(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel(), outlineWidth);

			if (p_isSelected && Editor_EditorSettings::ShowGeometryBounds && !p_toStencil)
			{
				RenderBoundingSpheres(*modelRenderer);
			}
		}

		if (auto cameraComponent = p_actor.GetComponent<Core::Core_CCamera>(); cameraComponent)
		{
			auto model = CalculateCameraModelMatrix(p_actor);

			if (p_toStencil)
				RenderModelToStencil(model, *mContext.mEditorResources->GetModel("Camera"));
			else
				RenderModelOutline(model, *mContext.mEditorResources->GetModel("Camera"), outlineWidth);

			if (p_isSelected && !p_toStencil)
			{
				RenderCameraFrustum(*cameraComponent);
			}
		}

		if (p_isSelected && !p_toStencil)
		{
			if (auto ambientBoxComp = p_actor.GetComponent<Core::Core_CAmbientBoxLight>())
			{
				RenderAmbientBoxVolume(*ambientBoxComp);
			}

			if (auto ambientSphereComp = p_actor.GetComponent<Core::Core_CAmbientSphereLight>())
			{
				RenderAmbientSphereVolume(*ambientSphereComp);
			}

			if (Editor_EditorSettings::ShowLightBounds)
			{
				if (auto light = p_actor.GetComponent<Core::Core_CLight>())
				{
					RenderLightBounds(*light);
				}
			}
		}

		for (auto& child : p_actor.GetChildren())
		{
			RenderActorOutlinePass(*child, p_toStencil, p_isSelected);
		}
	}
}

void DrawFrustumLines(Render::Render_ShapeDrawer& p_drawer,
	const glm::vec3& pos,
	const glm::vec3& forward,
	float near,
	const float far,
	const glm::vec3& a,
	const glm::vec3& b,
	const glm::vec3& c,
	const glm::vec3& d,
	const glm::vec3& e,
	const glm::vec3& f,
	const glm::vec3& g,
	const glm::vec3& h)
{
	auto draw = [&](const glm::vec3& p_start, const glm::vec3& p_end, const float planeDistance)
		{
			auto offset = pos + forward * planeDistance;
			auto start = offset + p_start;
			auto end = offset + p_end;
			p_drawer.DrawLine(start, end, FRUSTUM_COLOR);
		};

	draw(a, b, near);
	draw(b, d, near);
	draw(d, c, near);
	draw(c, a, near);

	draw(e, f, far);
	draw(f, h, far);
	draw(h, g, far);
	draw(g, e, far);

	draw(a + forward * near, e + forward * far, 0);
	draw(b + forward * near, f + forward * far, 0);
	draw(c + forward * near, g + forward * far, 0);
	draw(d + forward * near, h + forward * far, 0);
}

void Editor::Editor_EditorRenderer::RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& pSize, Core::Core_CCamera& pCamera)
{

}

void Editor::Editor_EditorRenderer::RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& pSize, Core::Core_CCamera& pCamera)
{

}

void Editor::Editor_EditorRenderer::RenderCameraFrustum(Core::Core_CCamera& pCamera)
{
	auto& gameView = EDITOR_PANEL(Editor_GameView, "Game View");
	auto gameViewSize = gameView.GetSafeSize();

	if (gameViewSize.first == 0 || gameViewSize.second == 0)
	{
		gameViewSize = { 16, 9 };
	}

	switch (pCamera.GetProjectionMode())
	{
	case Render::EProjectionMode::ORTHOGRAPHIC:
		RenderCameraOrthographicFrustum(gameViewSize, pCamera);
		break;

	case Render::EProjectionMode::PERSPECTIVE:
		RenderCameraPerspectiveFrustum(gameViewSize, pCamera);
		break;
	}
}

void Editor::Editor_EditorRenderer::RenderActorCollider(Core::Core_Actor& p_actor)
{
}

void Editor::Editor_EditorRenderer::RenderLightBounds(Core::Core_CLight& p_light)
{
	bool depthTestBackup = mContext.mRenderer->GetCapability(Render::ERenderingCapability::DEPTH_TEST);
	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_light.GetData();

	glm::quat rotation = data.GetTransform().GetWorldRotation();
	glm::vec3 center = data.GetTransform().GetWorldPosition();
	float radius = data.GetEffectRange();

	if (!std::isinf(radius))
	{
		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			mContext.mShapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), DEBUG_BOUNDS_COLOR, 1.f);
			mContext.mShapeDrawer->DrawLine(center + rotation * (glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), DEBUG_BOUNDS_COLOR, 1.f);
			mContext.mShapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), DEBUG_BOUNDS_COLOR, 1.f);
		}
	}

	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void Editor::Editor_EditorRenderer::RenderAmbientBoxVolume(Core::Core_CAmbientBoxLight& p_ambientBoxLight)
{
	bool depthTestBackup = mContext.mRenderer->GetCapability(Render::ERenderingCapability::DEPTH_TEST);
	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_ambientBoxLight.GetData();

	glm::mat4 model =
		glm::translate(glm::identity<glm::mat4>(), p_ambientBoxLight.mOwner.transform.GetWorldPosition()) *
		glm::scale(glm::identity<glm::mat4>(), {data.mConstant * 2.f, data.mLinear * 2.f, data.mQuadratic * 2.f});

	glm::vec3 center = p_ambientBoxLight.mOwner.transform.GetWorldPosition();
	glm::vec3 size = { data.mConstant * 2.f, data.mLinear * 2.f, data.mQuadratic * 2.f };
	glm::vec3 actorScale = p_ambientBoxLight.mOwner.transform.GetWorldScale();
	glm::vec3 halfSize = size / 2.f;

	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, halfSize.y, -halfSize.z }, center + glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + glm::vec3{ -halfSize.x, +halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z }, center + glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z }, center + glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, halfSize.y, -halfSize.z }, center + glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z }, center + glm::vec3{ +halfSize.x, +halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }, center + glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, +halfSize.y, -halfSize.z }, center + glm::vec3{ +halfSize.x, +halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z }, center + glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	mContext.mShapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }, center + glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);

	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void Editor::Editor_EditorRenderer::RenderAmbientSphereVolume(Core::Core_CAmbientSphereLight& p_ambientSphereLight)
{
	bool depthTestBackup = mContext.mRenderer->GetCapability(Render::ERenderingCapability::DEPTH_TEST);
	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_ambientSphereLight.GetData();

	glm::quat rotation = p_ambientSphereLight.mOwner.transform.GetWorldRotation();
	glm::vec3 center = p_ambientSphereLight.mOwner.transform.GetWorldPosition();
	float radius = data.mConstant;

	for (float i = 0; i <= 360.0f; i += 10.0f)
	{
		mContext.mShapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), LIGHT_VOLUME_COLOR, 1.f);
		mContext.mShapeDrawer->DrawLine(center + rotation * (glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), LIGHT_VOLUME_COLOR, 1.f);
		mContext.mShapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), LIGHT_VOLUME_COLOR, 1.f);
	}

	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void Editor::Editor_EditorRenderer::RenderBoundingSpheres(Core::Core_CModelRenderer& p_modelRenderer)
{
	bool depthTestBackup = mContext.mRenderer->GetCapability(Render::ERenderingCapability::DEPTH_TEST);
	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, false);

	if (auto model = p_modelRenderer.GetModel())
	{
		auto& actor = p_modelRenderer.mOwner;

		glm::vec3 actorScale = actor.transform.GetWorldScale();
		glm::quat actorRotation = actor.transform.GetWorldRotation();
		glm::vec3 actorPosition = actor.transform.GetWorldPosition();

		const auto& modelBoundingsphere =
			p_modelRenderer.GetFrustumBehaviour() == Core::Core_CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ?
			p_modelRenderer.GetCustomBoundingSphere() :
			model->GetBoundingSphere();

		float radiusScale = std::max(std::max(std::max(actorScale.x, actorScale.y), actorScale.z), 0.0f);
		float scaledRadius = modelBoundingsphere.radius * radiusScale;
		auto sphereOffset = Maths::Maths_FQuaternionExt::RotatePoint(modelBoundingsphere.position, actorRotation) * radiusScale;

		glm::vec3 boundingSphereCenter = actorPosition + sphereOffset;

		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			mContext.mShapeDrawer->DrawLine(boundingSphereCenter + actorRotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *scaledRadius), boundingSphereCenter + actorRotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
			mContext.mShapeDrawer->DrawLine(boundingSphereCenter + actorRotation * (glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
			mContext.mShapeDrawer->DrawLine(boundingSphereCenter + actorRotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
		}

		if (p_modelRenderer.GetFrustumBehaviour() == Core::Core_CModelRenderer::EFrustumBehaviour::CULL_MESHES)
		{
			const auto& meshes = model->GetMeshes();

			if (meshes.size() > 1)
			{
				for (auto mesh : meshes)
				{
					auto& meshBoundingSphere = mesh->GetBoundingSphere();
					float scaledRadius = meshBoundingSphere.radius * radiusScale;
					auto sphereOffset = Maths::Maths_FQuaternionExt::RotatePoint(meshBoundingSphere.position, actorRotation) * radiusScale;

					glm::vec3 boundingSphereCenter = actorPosition + sphereOffset;

					for (float i = 0; i <= 360.0f; i += 10.0f)
					{
						mContext.mShapeDrawer->DrawLine(boundingSphereCenter + actorRotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *scaledRadius), boundingSphereCenter + actorRotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
						mContext.mShapeDrawer->DrawLine(boundingSphereCenter + actorRotation * (glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
						mContext.mShapeDrawer->DrawLine(boundingSphereCenter + actorRotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
					}
				}
			}
		}
	}

	mContext.mRenderer->SetCapability(Render::ERenderingCapability::DEPTH_TEST, depthTestBackup);
	mContext.mRenderer->SetRasterizationLinesWidth(1.0f);
}

void Editor::Editor_EditorRenderer::RenderModelAsset(Render::Render_Model& pModel)
{
	glm::mat4 model = glm::scale(glm::identity<glm::mat4>(), {3.f, 3.f, 3.f});
	mContext.mRenderer->DrawModelWithSingleMaterial(pModel, m_defaultMaterial, &model);
}

void Editor::Editor_EditorRenderer::RenderTextureAsset(Render::Render_Texture& pTexture)
{
	glm::mat4 model = glm::rotate(glm::scale(glm::identity<glm::mat4>(), { 5.f, 5.f, 5.f }), glm::radians(90.f * 0.0174f), glm::vec3(1, 0, 0));

	m_textureMaterial.Set<Render::Render_Texture*>("u_DiffuseMap", &pTexture);
	mContext.mRenderer->DrawModelWithSingleMaterial(*mContext.mEditorResources->GetModel("Plane"), m_textureMaterial, &model);
}

void Editor::Editor_EditorRenderer::RenderMaterialAsset(Core::Core_Material& pMaterial)
{
	glm::mat4 model = glm::scale(glm::identity<glm::mat4>(), {3.f, 3.f, 3.f});
	mContext.mRenderer->DrawModelWithSingleMaterial(*mContext.mEditorResources->GetModel("Sphere"), pMaterial, &model, &m_emptyMaterial);
}

void Editor::Editor_EditorRenderer::RenderGrid(const glm::vec3& p_viewPos, const glm::vec3& pColor)
{
	constexpr float gridSize = 5000.0f;

	glm::mat4 model = 
		glm::translate(glm::identity<glm::mat4>(), {p_viewPos.x, 0.0f, p_viewPos.z}) * 
		glm::scale(glm::identity<glm::mat4>(), {gridSize * 2.0f, 1.f, gridSize * 2.0f});
	m_gridMaterial.Set("u_Color", pColor);
	mContext.mRenderer->DrawModelWithSingleMaterial(*mContext.mEditorResources->GetModel("Plane"), m_gridMaterial, &model);

	mContext.mShapeDrawer->DrawLine(glm::vec3(-gridSize + p_viewPos.x, 0.0f, 0.0f), glm::vec3(gridSize + p_viewPos.x, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
	mContext.mShapeDrawer->DrawLine(glm::vec3(0.0f, -gridSize + p_viewPos.y, 0.0f), glm::vec3(0.0f, gridSize + p_viewPos.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
	mContext.mShapeDrawer->DrawLine(glm::vec3(0.0f, 0.0f, -gridSize + p_viewPos.z), glm::vec3(0.0f, 0.0f, gridSize + p_viewPos.z), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
}

void Editor::Editor_EditorRenderer::UpdateLights(Core::Core_Scene& pScene)
{
	//PROFILER_SPY("Light SSBO Update");
	auto lightMatrices = mContext.mRenderer->FindLightMatrices(pScene);
	mContext.mLightSSBO->SendBlocks<glm::mat4>(lightMatrices.data(), lightMatrices.size() * sizeof(glm::mat4));
}

void Editor::Editor_EditorRenderer::UpdateLightsInFrustum(Core::Core_Scene& pScene, const Render::Render_Frustum& pFrustum)
{
	//PROFILER_SPY("Light SSBO Update (Frustum culled)");
	auto lightMatrices = mContext.mRenderer->FindLightMatricesInFrustum(pScene, pFrustum);
	mContext.mLightSSBO->SendBlocks<glm::mat4>(lightMatrices.data(), lightMatrices.size() * sizeof(glm::mat4));
}
