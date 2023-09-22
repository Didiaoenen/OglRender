
#include <OglRenderer/Render_Frustum.h>
#include <OglRenderer/Render_TextureLoader.h>

#include "Core_CMaterialRenderer.h"
#include "Core_CModelRenderer.h"
#include "Core_Renderer.h"

Core::Core_Renderer::Core_Renderer(Render::Render_Driver& pDriver) :
	Render_Renderer(pDriver),
	mEmptyTexture(Render::Render_TextureLoader::CreateColor
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
	Render::Render_TextureLoader::Destroy(mEmptyTexture);
}

Core::Core_CCamera* Core::Core_Renderer::FindMainCamera(const Core_Scene& pScene)
{
	for (Core_CCamera* camera : pScene.GetFastAccessComponents().cameras)
	{
		if (camera->mOwner.IsActive())
		{
			return camera;
		}
	}
	return nullptr;
}

std::vector<glm::mat4> Core::Core_Renderer::FindLightMatrices(const Core_Scene& pScene)
{
	std::vector<glm::mat4> result;

	const auto& facs = pScene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->mOwner.IsActive())
		{
			result.push_back(light->GetData().GenerateMatrix());
		}
	}

	return result;
}

std::vector<glm::mat4> Core::Core_Renderer::FindLightMatricesInFrustum(const Core_Scene& pScene, const Render::Render_Frustum& pFrustum)
{
	std::vector<glm::mat4> result;

	const auto& facs = pScene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->mOwner.IsActive())
		{
			const auto& lightData = light->GetData();
			const auto& position = lightData.GetTransform().GetWorldPosition();
			auto effectRange = lightData.GetEffectRange();

			if (std::isinf(effectRange) || pFrustum.SphereInFrustum(position.x, position.y, position.z, lightData.GetEffectRange()))
			{
				result.push_back(lightData.GenerateMatrix());
			}
		}
	}

	return result;
}

void Core::Core_Renderer::RenderScene(Core_Scene& pScene, const glm::vec3& pCameraPosition, const Render::Render_Camera& pCamera, const Render::Render_Frustum* pCustomFrustum, Core_Material* pDefaultMaterial)
{
	OpaqueDrawables	opaqueMeshes;
	TransparentDrawables transparentMeshes;

	if (pCamera.HasFrustumGeometryCulling())
	{
		const auto& frustum = pCustomFrustum ? *pCustomFrustum : pCamera.GetFrustum();
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortFrustumCulledDrawables(pScene, pCameraPosition, frustum, pDefaultMaterial);
	}
	else
	{
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(pScene, pCameraPosition, pDefaultMaterial);
	}

	for (const auto& [distance, drawable] : opaqueMeshes)
	{
		DrawDrawable(drawable);
	}

	for (const auto& [distance, drawable] : transparentMeshes)
	{
		DrawDrawable(drawable);
	}
}

std::pair<Core::Core_Renderer::OpaqueDrawables, Core::Core_Renderer::TransparentDrawables> Core::Core_Renderer::FindAndSortFrustumCulledDrawables(const Core_Scene& pScene, const glm::vec3& pCameraPosition, const Render::Render_Frustum& pFrustum, Core_Material* pDefaultMaterial)
{
	OpaqueDrawables opaqueDrawables;
	TransparentDrawables transparentDrawables;

	for (Core_CModelRenderer* modelRenderer : pScene.GetFastAccessComponents().modelRenderers)
	{
		auto& mOwner = modelRenderer->mOwner;

		if (mOwner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->mOwner.GetComponent<Core_CMaterialRenderer>())
				{
					auto& transform = mOwner.transform.GetFTransform();

					Render::ECullingOptions cullingOptions = Render::ECullingOptions::NONE;

					if (modelRenderer->GetFrustumBehaviour() != Core_CModelRenderer::EFrustumBehaviour::DISABLED)
					{
						cullingOptions |= Render::ECullingOptions::FRUSTUM_PER_MODEL;
					}

					if (modelRenderer->GetFrustumBehaviour() == Core_CModelRenderer::EFrustumBehaviour::CULL_MESHES)
					{
						cullingOptions |= Render::ECullingOptions::FRUSTUM_PER_MESH;
					}

					const auto& modelBoundingSphere = modelRenderer->GetFrustumBehaviour() == Core_CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ? modelRenderer->GetCustomBoundingSphere() : model->GetBoundingSphere();

					std::vector<std::reference_wrapper<Render::Render_Mesh>> meshes;

					{
						//PROFILER_SPY("Frustum Culling");
						meshes = GetMeshesInFrustum(*model, modelBoundingSphere, transform, pFrustum, cullingOptions);
					}

					if (!meshes.empty())
					{
						float distanceToActor = glm::distance(transform.GetWorldPosition(), pCameraPosition);
						const Core_CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						for (const auto& mesh : meshes)
						{
							Core_Material* material = nullptr;

							if (mesh.get().GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh.get().GetMaterialIndex());
								if (!material || !material->GetShader())
								{
									material = pDefaultMaterial;
								}
							}

							if (material)
							{
								Drawable element = { transform.GetWorldMatrix(), &mesh.get(), material, materialRenderer->GetUserMatrix() };

								if (material->IsBlendable())
								{
									transparentDrawables.emplace(distanceToActor, element);
								}
								else
								{
									opaqueDrawables.emplace(distanceToActor, element);
								}
							}
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

std::pair<Core::Core_Renderer::OpaqueDrawables, Core::Core_Renderer::TransparentDrawables> Core::Core_Renderer::FindAndSortDrawables(const Core_Scene& pScene, const glm::vec3& pCameraPosition, Core_Material* pDefaultMaterial)
{
	OpaqueDrawables opaqueDrawables;
	TransparentDrawables transparentDrawables;

	for (Core_CModelRenderer* modelRenderer : pScene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->mOwner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = glm::distance(modelRenderer->mOwner.transform.GetWorldPosition(), pCameraPosition);

				if (auto materialRenderer = modelRenderer->mOwner.GetComponent<Core_CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->mOwner.transform.GetFTransform();

					const Core_CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					for (auto mesh : model->GetMeshes())
					{
						Core_Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
							{
								material = pDefaultMaterial;
							}
						}

						if (material)
						{
							Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix() };

							if (material->IsBlendable())
							{
								transparentDrawables.emplace(distanceToActor, element);
							}
							else
							{
								opaqueDrawables.emplace(distanceToActor, element);
							}
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

void Core::Core_Renderer::DrawDrawable(const Drawable& pToDraw)
{
	mUserMatrixSender(std::get<3>(pToDraw));
	DrawMesh(*std::get<1>(pToDraw), *std::get<2>(pToDraw), &std::get<0>(pToDraw));
}

void Core::Core_Renderer::DrawModelWithSingleMaterial(Render::Render_Model& pModel, Core_Material& pMaterial, glm::mat4 const* pModelMatrix, Core_Material* pDefaultMaterial)
{
	if (pModelMatrix)
	{
		mModelMatrixSender(*pModelMatrix);
	}

	for (auto mesh : pModel.GetMeshes())
	{
		Core_Material* material = pMaterial.GetShader() ? &pMaterial : pDefaultMaterial;

		if (material)
		{
			DrawMesh(*mesh, *material, nullptr);
		}
	}
}

void Core::Core_Renderer::DrawModelWithMaterials(Render::Render_Model& pModel, std::vector<Core_Material*> pMaterials, glm::mat4 const* pModelMatrix, Core_Material* pDefaultMaterial)
{
	if (pModelMatrix)
	{
		mModelMatrixSender(*pModelMatrix);
	}

	for (auto mesh : pModel.GetMeshes())
	{
		Core_Material* material = pMaterials.size() > mesh->GetMaterialIndex() ? pMaterials[mesh->GetMaterialIndex()] : pDefaultMaterial;
		if (material)
		{
			DrawMesh(*mesh, *material, nullptr);
		}
	}
}

void Core::Core_Renderer::DrawMesh(Render::Render_Mesh& pMesh, Core_Material& pMaterial, glm::mat4 const* pModelMatrix)
{
	if (pMaterial.HasShader() && pMaterial.GetGPUInstances() > 0)
	{
		if (pModelMatrix)
		{
			mModelMatrixSender(*pModelMatrix);
		}

		uint8_t stateMask = pMaterial.GenerateStateMask();
		ApplyStateMask(stateMask);

		pMaterial.Bind(mEmptyTexture);
		Draw(pMesh, Render::EPrimitiveMode::TRIANGLES, pMaterial.GetGPUInstances());
		pMaterial.UnBind();
	}
}

void Core::Core_Renderer::RegisterModelMatrixSender(std::function<void(glm::mat4)> pModelMatrixSender)
{
	mModelMatrixSender = pModelMatrixSender;
}

void Core::Core_Renderer::RegisterUserMatrixSender(std::function<void(glm::mat4)> pUserMatrixSender)
{
	mUserMatrixSender = pUserMatrixSender;
}
