
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
	Render::Render_TextureLoader::Destroy(m_emptyTexture);
}

Core::Core_CCamera* Core::Core_Renderer::FindMainCamera(const Core::Core_Scene& p_scene)
{
	for (Core_CCamera* camera : p_scene.GetFastAccessComponents().cameras)
	{
		if (camera->mOwner.IsActive())
		{
			return camera;
		}
	}
	return nullptr;
}

std::vector<glm::mat4> Core::Core_Renderer::FindLightMatrices(const Core::Core_Scene& p_scene)
{
	std::vector<glm::mat4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->mOwner.IsActive())
		{
			result.push_back(light->GetData().GenerateMatrix());
		}
	}

	return result;
}

std::vector<glm::mat4> Core::Core_Renderer::FindLightMatricesInFrustum(const Core::Core_Scene& p_scene, const Render::Render_Frustum& p_frustum)
{
	std::vector<glm::mat4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->mOwner.IsActive())
		{
			const auto& lightData = light->GetData();
			const auto& position = lightData.GetTransform().GetWorldPosition();
			auto effectRange = lightData.GetEffectRange();

			if (std::isinf(effectRange) || p_frustum.SphereInFrustum(position.x, position.y, position.z, lightData.GetEffectRange()))
			{
				result.push_back(lightData.GenerateMatrix());
			}
		}
	}

	return result;
}

void Core::Core_Renderer::RenderScene(Core::Core_Scene& p_scene, const glm::vec3& p_cameraPosition, const Render::Render_Camera& p_camera, const Render::Render_Frustum* p_customFrustum, Core::Core_Material* p_defaultMaterial)
{
	OpaqueDrawables	opaqueMeshes;
	TransparentDrawables transparentMeshes;

	if (p_camera.HasFrustumGeometryCulling())
	{
		const auto& frustum = p_customFrustum ? *p_customFrustum : p_camera.GetFrustum();
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortFrustumCulledDrawables(p_scene, p_cameraPosition, frustum, p_defaultMaterial);
	}
	else
	{
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(p_scene, p_cameraPosition, p_defaultMaterial);
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

std::pair<Core::Core_Renderer::OpaqueDrawables, Core::Core_Renderer::TransparentDrawables> Core::Core_Renderer::FindAndSortFrustumCulledDrawables(const Core::Core_Scene& p_scene, const glm::vec3& p_cameraPosition, const Render::Render_Frustum& p_frustum, Core::Core_Material* p_defaultMaterial)
{
	OpaqueDrawables opaqueDrawables;
	TransparentDrawables transparentDrawables;

	for (Core_CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
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
						meshes = GetMeshesInFrustum(*model, modelBoundingSphere, transform, p_frustum, cullingOptions);
					}

					if (!meshes.empty())
					{
						float distanceToActor = glm::distance(transform.GetWorldPosition(), p_cameraPosition);
						const Core_CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						for (const auto& mesh : meshes)
						{
							Core_Material* material = nullptr;

							if (mesh.get().GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh.get().GetMaterialIndex());
								if (!material || !material->GetShader())
									material = p_defaultMaterial;
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

std::pair<Core::Core_Renderer::OpaqueDrawables, Core::Core_Renderer::TransparentDrawables> Core::Core_Renderer::FindAndSortDrawables(const Core::Core_Scene& p_scene, const glm::vec3& p_cameraPosition, Core::Core_Material* p_defaultMaterial)
{
	OpaqueDrawables opaqueDrawables;
	TransparentDrawables transparentDrawables;

	for (Core_CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->mOwner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = glm::distance(modelRenderer->mOwner.transform.GetWorldPosition(), p_cameraPosition);

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
								material = p_defaultMaterial;
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

void Core::Core_Renderer::DrawDrawable(const Drawable& p_toDraw)
{
	m_userMatrixSender(std::get<3>(p_toDraw));
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw));
}

void Core::Core_Renderer::DrawModelWithSingleMaterial(Render::Render_Model& pModel, Core::Core_Material& pMaterial, glm::mat4 const* p_modelMatrix, Core::Core_Material* p_defaultMaterial)
{
	if (p_modelMatrix)
	{
		m_modelMatrixSender(*p_modelMatrix);
	}

	for (auto mesh : pModel.GetMeshes())
	{
		Core_Material* material = pMaterial.GetShader() ? &pMaterial : p_defaultMaterial;

		if (material)
		{
			DrawMesh(*mesh, *material, nullptr);
		}
	}
}

void Core::Core_Renderer::DrawModelWithMaterials(Render::Render_Model& pModel, std::vector<Core::Core_Material*> p_materials, glm::mat4 const* p_modelMatrix, Core::Core_Material* p_defaultMaterial)
{
	if (p_modelMatrix)
	{
		m_modelMatrixSender(*p_modelMatrix);
	}

	for (auto mesh : pModel.GetMeshes())
	{
		Core_Material* material = p_materials.size() > mesh->GetMaterialIndex() ? p_materials[mesh->GetMaterialIndex()] : p_defaultMaterial;
		if (material)
		{
			DrawMesh(*mesh, *material, nullptr);
		}
	}
}

void Core::Core_Renderer::DrawMesh(Render::Render_Mesh& p_mesh, Core::Core_Material& pMaterial, glm::mat4 const* p_modelMatrix)
{
	if (pMaterial.HasShader() && pMaterial.GetGPUInstances() > 0)
	{
		if (p_modelMatrix)
		{
			m_modelMatrixSender(*p_modelMatrix);
		}

		uint8_t stateMask = pMaterial.GenerateStateMask();
		ApplyStateMask(stateMask);

		pMaterial.Bind(m_emptyTexture);
		Draw(p_mesh, Render::EPrimitiveMode::TRIANGLES, pMaterial.GetGPUInstances());
		pMaterial.UnBind();
	}
}

void Core::Core_Renderer::RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender)
{
	m_modelMatrixSender = p_modelMatrixSender;
}

void Core::Core_Renderer::RegisterUserMatrixSender(std::function<void(glm::mat4)> p_userMatrixSender)
{
	m_userMatrixSender = p_userMatrixSender;
}
