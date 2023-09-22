#include <OglRenderer/Render_Texture.h>
#include <OglRenderer/Render_UniformBuffer.h>

#include "Core_ServiceLocator.h"
#include "Core_TextureManager.h"
#include "Core_Material.h"

void Core::Core_Material::SetShader(Render::Render_Shader* pShader)
{
	m_shader = pShader;
	if (m_shader)
	{
		Render::Render_UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
		FillUniform();
	}
	else
	{
		m_uniformsData.clear();
	}
}

void Core::Core_Material::FillUniform()
{
	m_uniformsData.clear();

	for (const Render::Render_UniformInfo& element : m_shader->mUniforms)
	{
		m_uniformsData.emplace(element.name, element.defaultValue);
	}
}

void Core::Core_Material::Bind(Render::Render_Texture* p_emptyTexture)
{
	if (HasShader())
	{
		m_shader->Bind();

		int textureSlot = 0;

		for (auto& [name, value] : m_uniformsData)
		{
			auto uniformData = m_shader->GetUniformInfo(name);

			if (uniformData)
			{
				switch (uniformData->type)
				{
					case Render::EUniformType::UNIFORM_BOOL:			if (value.type() == typeid(bool))		m_shader->SetUniformInt(name, std::any_cast<bool>(value));			break;
					case Render::EUniformType::UNIFORM_INT:			if (value.type() == typeid(int))		m_shader->SetUniformInt(name, std::any_cast<int>(value));			break;
					case Render::EUniformType::UNIFORM_FLOAT:		if (value.type() == typeid(float))		m_shader->SetUniformFloat(name, std::any_cast<float>(value));		break;
					case Render::EUniformType::UNIFORM_FLOAT_VEC2:	if (value.type() == typeid(glm::vec2))	m_shader->SetUniformVec2(name, std::any_cast<glm::vec2>(value));		break;
					case Render::EUniformType::UNIFORM_FLOAT_VEC3:	if (value.type() == typeid(glm::vec3))	m_shader->SetUniformVec3(name, std::any_cast<glm::vec3>(value));		break;
					case Render::EUniformType::UNIFORM_FLOAT_VEC4:	if (value.type() == typeid(glm::vec4))	m_shader->SetUniformVec4(name, std::any_cast<glm::vec4>(value));		break;
					case Render::EUniformType::UNIFORM_SAMPLER_2D:
					{
						if (value.type() == typeid(Render::Render_Texture*))
						{
							if (auto tex = std::any_cast<Render::Render_Texture*>(value); tex)
							{
								tex->Bind(textureSlot);
								m_shader->SetUniformInt(uniformData->name, textureSlot++);
							}
							else if (p_emptyTexture)
							{
								p_emptyTexture->Bind(textureSlot);
								m_shader->SetUniformInt(uniformData->name, textureSlot++);
							}
						}
					}
				}
			}
		}
	}
}

void Core::Core_Material::UnBind()
{
	if (HasShader())
	{
		m_shader->Unbind();
	}
}

Render::Render_Shader*& Core::Core_Material::GetShader()
{
	return m_shader;
}

bool Core::Core_Material::HasShader() const
{
	return m_shader;
}

void Core::Core_Material::SetBlendable(bool p_blendable)
{
	m_blendable = p_blendable;
}

void Core::Core_Material::SetBackfaceCulling(bool p_backfaceCulling)
{
	m_backfaceCulling = p_backfaceCulling;
}

void Core::Core_Material::SetFrontfaceCulling(bool p_frontfaceCulling)
{
	m_frontfaceCulling = p_frontfaceCulling;
}

void Core::Core_Material::SetDepthTest(bool p_depthTest)
{
	m_depthTest = p_depthTest;
}

void Core::Core_Material::SetDepthWriting(bool p_depthWriting)
{
	m_depthWriting = p_depthWriting;
}

void Core::Core_Material::SetColorWriting(bool p_colorWriting)
{
	m_colorWriting = p_colorWriting;
}

void Core::Core_Material::SetGPUInstances(int pInstances)
{
	m_gpuInstances = pInstances;
}

bool Core::Core_Material::IsBlendable() const
{
	return m_blendable;
}

bool Core::Core_Material::HasBackfaceCulling() const
{
	return m_backfaceCulling;
}

bool Core::Core_Material::HasFrontfaceCulling() const
{
	return m_frontfaceCulling;
}

bool Core::Core_Material::HasDepthTest() const
{
	return m_depthTest;
}

bool Core::Core_Material::HasDepthWriting() const
{
	return m_depthWriting;
}

bool Core::Core_Material::HasColorWriting() const
{
	return m_colorWriting;
}

int Core::Core_Material::GetGPUInstances() const
{
	return m_gpuInstances;
}

uint8_t Core::Core_Material::GenerateStateMask() const
{
	uint8_t result = 0;

	if (m_depthWriting)								result |= 0b0000'0001;
	if (m_colorWriting)								result |= 0b0000'0010;
	if (m_blendable)								result |= 0b0000'0100;
	if (m_backfaceCulling || m_frontfaceCulling)	result |= 0b0000'1000;
	if (m_depthTest)								result |= 0b0001'0000;
	if (m_backfaceCulling)							result |= 0b0010'0000;
	if (m_frontfaceCulling)							result |= 0b0100'0000;

	return result;
}

std::map<std::string, std::any>& Core::Core_Material::GetUniformsData()
{
	return m_uniformsData;
}

void Core::Core_Material::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::SerializeShader(pDoc, pNode, "shader", m_shader);

	tinyxml2::XMLNode* settingsNode = pDoc.NewElement("settings");
	pNode->InsertEndChild(settingsNode);

	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "blendable", m_blendable);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "backface_culling", m_backfaceCulling);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "frontface_culling", m_frontfaceCulling);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "depth_test", m_depthTest);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "depth_writing", m_depthWriting);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "color_writing", m_colorWriting);
	Core_Serializer::SerializeInt(pDoc, settingsNode, "gpu_instances", m_gpuInstances);

	tinyxml2::XMLNode* uniformsNode = pDoc.NewElement("uniforms");
	pNode->InsertEndChild(uniformsNode);

	for (const auto& [uniformName, uniformValue] : m_uniformsData)
	{
		tinyxml2::XMLNode* uniform = pDoc.NewElement("uniform");
		uniformsNode->InsertEndChild(uniform);

		const Render::Render_UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

		Core_Serializer::SerializeString(pDoc, uniform, "name", uniformName);

		if (uniformInfo && uniformValue.has_value())
		{
			switch (uniformInfo->type)
			{
			case Render::EUniformType::UNIFORM_BOOL:
				if (uniformValue.type() == typeid(bool)) Core_Serializer::SerializeInt(pDoc, uniform, "value", std::any_cast<bool>(uniformValue));
				break;

			case Render::EUniformType::UNIFORM_INT:
				if (uniformValue.type() == typeid(int)) Core_Serializer::SerializeInt(pDoc, uniform, "value", std::any_cast<int>(uniformValue));
				break;

			case Render::EUniformType::UNIFORM_FLOAT:
				if (uniformValue.type() == typeid(float)) Core_Serializer::SerializeFloat(pDoc, uniform, "value", std::any_cast<float>(uniformValue));
				break;

			case Render::EUniformType::UNIFORM_FLOAT_VEC2:
				if (uniformValue.type() == typeid(glm::vec2)) Core_Serializer::SerializeVec2(pDoc, uniform, "value", std::any_cast<glm::vec2>(uniformValue));
				break;

			case Render::EUniformType::UNIFORM_FLOAT_VEC3:
				if (uniformValue.type() == typeid(glm::vec3)) Core_Serializer::SerializeVec3(pDoc, uniform, "value", std::any_cast<glm::vec3>(uniformValue));
				break;

			case Render::EUniformType::UNIFORM_FLOAT_VEC4:
				if (uniformValue.type() == typeid(glm::vec4)) Core_Serializer::SerializeVec4(pDoc, uniform, "value", std::any_cast<glm::vec4>(uniformValue));
				break;

			case Render::EUniformType::UNIFORM_SAMPLER_2D:
				if (uniformValue.type() == typeid(Render::Render_Texture*)) Core_Serializer::SerializeTexture(pDoc, uniform, "value", std::any_cast<Render::Render_Texture*>(uniformValue));
				break;
			}
		}
	}
}

void Core::Core_Material::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	tinyxml2::XMLNode* settingsNode = pNode->FirstChildElement("settings");

	if (settingsNode)
	{
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "blendable", m_blendable);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "backface_culling", m_backfaceCulling);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "frontface_culling", m_frontfaceCulling);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "depth_test", m_depthTest);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "depth_writing", m_depthWriting);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "color_writing", m_colorWriting);
		Core_Serializer::DeserializeInt(pDoc, settingsNode, "gpu_instances", m_gpuInstances);
	}

	Render::Render_Shader* deserializedShader = Core_Serializer::DeserializeShader(pDoc, pNode, "shader");

	if (deserializedShader)
	{
		SetShader(deserializedShader);

		tinyxml2::XMLNode* uniformsNode = pNode->FirstChildElement("uniforms");

		if (uniformsNode)
		{
			for (auto uniform = uniformsNode->FirstChildElement("uniform"); uniform; uniform = uniform->NextSiblingElement("uniform"))
			{
				if (auto uniformNameElement = uniform->FirstChildElement("name"); uniformNameElement)
				{
					const std::string uniformName = uniformNameElement->GetText();

					const Render::Render_UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

					if (uniformInfo)
					{
						switch (uniformInfo->type)
						{
						case Render::EUniformType::UNIFORM_BOOL:
							m_uniformsData[uniformInfo->name] = Core_Serializer::DeserializeBoolean(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_INT:
							m_uniformsData[uniformInfo->name] = Core_Serializer::DeserializeInt(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT:
							m_uniformsData[uniformInfo->name] = Core_Serializer::DeserializeFloat(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT_VEC2:
							m_uniformsData[uniformInfo->name] = Core_Serializer::DeserializeVec2(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT_VEC3:
							m_uniformsData[uniformInfo->name] = Core_Serializer::DeserializeVec3(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT_VEC4:
							m_uniformsData[uniformInfo->name] = Core_Serializer::DeserializeVec4(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_SAMPLER_2D:
							m_uniformsData[uniformInfo->name] = Core_Serializer::DeserializeTexture(pDoc, uniform, "value");
							break;
						}
					}
				}
			}
		}
	}
}
