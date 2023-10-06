#include <OglRenderer/Render_Texture.h>
#include <OglRenderer/Render_UniformBuffer.h>

#include "Core_ServiceLocator.h"
#include "Core_TextureManager.h"
#include "Core_Material.h"

void Core::Core_Material::SetShader(Render::Render_Shader* pShader)
{
	mShader = pShader;
	if (mShader)
	{
		Render::Render_UniformBuffer::BindBlockToShader(*mShader, "EngineUBO");
		FillUniform();
	}
	else
	{
		mUniformsData.clear();
	}
}

void Core::Core_Material::FillUniform()
{
	mUniformsData.clear();

	for (auto it = mShader->mUniforms.begin(); it != mShader->mUniforms.end(); it++)
	{
		auto& [name, program] = *it;
		mUniformsData.emplace(name, program);
	}
}

void Core::Core_Material::Bind(Render::Render_Texture* pEmptyTexture, const std::string& pProgramName)
{
	if (HasShader())
	{
		Render::Render_Program* program = mShader->GetRenderProgram(pProgramName);
		if (program)
		{
			program->Bind();

			int textureSlot = 0;

			for (auto& [name, value] : mUniformsData)
			{
				auto uniformData = mShader->GetUniformInfo(name);

				if (uniformData)
				{
					switch (uniformData->type)
					{
					case Render::EUniformType::UNIFORM_BOOL:		if (value.type() == typeid(bool))		program->SetUniformInt(name, std::any_cast<bool>(value));			break;
					case Render::EUniformType::UNIFORM_INT:			if (value.type() == typeid(int))		program->SetUniformInt(name, std::any_cast<int>(value));			break;
					case Render::EUniformType::UNIFORM_FLOAT:		if (value.type() == typeid(float))		program->SetUniformFloat(name, std::any_cast<float>(value));		break;
					case Render::EUniformType::UNIFORM_FLOAT_VEC2:	if (value.type() == typeid(glm::vec2))	program->SetUniformVec2(name, std::any_cast<glm::vec2>(value));		break;
					case Render::EUniformType::UNIFORM_FLOAT_VEC3:	if (value.type() == typeid(glm::vec3))	program->SetUniformVec3(name, std::any_cast<glm::vec3>(value));		break;
					case Render::EUniformType::UNIFORM_FLOAT_VEC4:	if (value.type() == typeid(glm::vec4))	program->SetUniformVec4(name, std::any_cast<glm::vec4>(value));		break;
					case Render::EUniformType::UNIFORM_SAMPLER_2D:
					{
						if (value.type() == typeid(Render::Render_Texture*))
						{
							if (auto tex = std::any_cast<Render::Render_Texture*>(value); tex)
							{
								tex->Bind(textureSlot);
								program->SetUniformInt(uniformData->name, textureSlot++);
							}
							else if (pEmptyTexture)
							{
								pEmptyTexture->Bind(textureSlot);
								program->SetUniformInt(uniformData->name, textureSlot++);
							}
						}
					}
					}
				}
			}
		}
	}
}

void Core::Core_Material::UnBind(const std::string& pProgramName)
{
	if (HasShader())
	{
		Render::Render_Program* program = mShader->GetRenderProgram(pProgramName);
		if (program)
		{
			program->Unbind();
		}
	}
}

Render::Render_Shader*& Core::Core_Material::GetShader()
{
	return mShader;
}

bool Core::Core_Material::HasShader() const
{
	return mShader;
}

void Core::Core_Material::SetBlendable(bool pBlendable)
{
	mBlendable = pBlendable;
}

void Core::Core_Material::SetBackfaceCulling(bool pBackfaceCulling)
{
	mBackfaceCulling = pBackfaceCulling;
}

void Core::Core_Material::SetFrontfaceCulling(bool pFrontfaceCulling)
{
	mFrontfaceCulling = pFrontfaceCulling;
}

void Core::Core_Material::SetDepthTest(bool pDepthTest)
{
	mDepthTest = pDepthTest;
}

void Core::Core_Material::SetDepthWriting(bool pDepthWriting)
{
	mDepthWriting = pDepthWriting;
}

void Core::Core_Material::SetColorWriting(bool pColorWriting)
{
	mColorWriting = pColorWriting;
}

void Core::Core_Material::SetGPUInstances(int pInstances)
{
	mGpuInstances = pInstances;
}

bool Core::Core_Material::IsBlendable() const
{
	return mBlendable;
}

bool Core::Core_Material::HasBackfaceCulling() const
{
	return mBackfaceCulling;
}

bool Core::Core_Material::HasFrontfaceCulling() const
{
	return mFrontfaceCulling;
}

bool Core::Core_Material::HasDepthTest() const
{
	return mDepthTest;
}

bool Core::Core_Material::HasDepthWriting() const
{
	return mDepthWriting;
}

bool Core::Core_Material::HasColorWriting() const
{
	return mColorWriting;
}

int Core::Core_Material::GetGPUInstances() const
{
	return mGpuInstances;
}

uint8_t Core::Core_Material::GenerateStateMask() const
{
	uint8_t result = 0;

	if (mDepthWriting)							result |= 0b0000'0001;
	if (mColorWriting)							result |= 0b0000'0010;
	if (mBlendable)								result |= 0b0000'0100;
	if (mBackfaceCulling || mFrontfaceCulling)	result |= 0b0000'1000;
	if (mDepthTest)								result |= 0b0001'0000;
	if (mBackfaceCulling)						result |= 0b0010'0000;
	if (mFrontfaceCulling)						result |= 0b0100'0000;

	return result;
}

std::map<std::string, std::any>& Core::Core_Material::GetUniformsData()
{
	return mUniformsData;
}

void Core::Core_Material::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::SerializeShader(pDoc, pNode, "shader", mShader);

	tinyxml2::XMLNode* settingsNode = pDoc.NewElement("settings");
	pNode->InsertEndChild(settingsNode);

	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "blendable", mBlendable);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "backface_culling", mBackfaceCulling);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "frontface_culling", mFrontfaceCulling);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "depth_test", mDepthTest);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "depth_writing", mDepthWriting);
	Core_Serializer::SerializeBoolean(pDoc, settingsNode, "color_writing", mColorWriting);
	Core_Serializer::SerializeInt(pDoc, settingsNode, "gpu_instances", mGpuInstances);

	tinyxml2::XMLNode* uniformsNode = pDoc.NewElement("uniforms");
	pNode->InsertEndChild(uniformsNode);

	for (const auto& [uniformName, uniformValue] : mUniformsData)
	{
		tinyxml2::XMLNode* uniform = pDoc.NewElement("uniform");
		uniformsNode->InsertEndChild(uniform);

		const Render::Render_UniformInfo* uniformInfo = mShader->GetUniformInfo(uniformName);

		Core_Serializer::SerializeString(pDoc, uniform, "name", uniformName);

		if (uniformInfo && uniformValue.has_value())
		{
			switch (uniformInfo->type)
			{
			case Render::EUniformType::UNIFORM_BOOL:
				if (uniformValue.type() == typeid(bool))
				{
					Core_Serializer::SerializeInt(pDoc, uniform, "value", std::any_cast<bool>(uniformValue));
				}
				break;

			case Render::EUniformType::UNIFORM_INT:
				if (uniformValue.type() == typeid(int))
				{
					Core_Serializer::SerializeInt(pDoc, uniform, "value", std::any_cast<int>(uniformValue));
				}
				break;

			case Render::EUniformType::UNIFORM_FLOAT:
				if (uniformValue.type() == typeid(float))
				{
					Core_Serializer::SerializeFloat(pDoc, uniform, "value", std::any_cast<float>(uniformValue));
				}
				break;

			case Render::EUniformType::UNIFORM_FLOAT_VEC2:
				if (uniformValue.type() == typeid(glm::vec2))
				{
					Core_Serializer::SerializeVec2(pDoc, uniform, "value", std::any_cast<glm::vec2>(uniformValue));
				}
				break;

			case Render::EUniformType::UNIFORM_FLOAT_VEC3:
				if (uniformValue.type() == typeid(glm::vec3))
				{
					Core_Serializer::SerializeVec3(pDoc, uniform, "value", std::any_cast<glm::vec3>(uniformValue));
				}
				break;

			case Render::EUniformType::UNIFORM_FLOAT_VEC4:
				if (uniformValue.type() == typeid(glm::vec4))
				{
					Core_Serializer::SerializeVec4(pDoc, uniform, "value", std::any_cast<glm::vec4>(uniformValue));
				}
				break;

			case Render::EUniformType::UNIFORM_SAMPLER_2D:
				if (uniformValue.type() == typeid(Render::Render_Texture*))
				{
					Core_Serializer::SerializeTexture(pDoc, uniform, "value", std::any_cast<Render::Render_Texture*>(uniformValue));
				}
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
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "blendable", mBlendable);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "backface_culling", mBackfaceCulling);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "frontface_culling", mFrontfaceCulling);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "depth_test", mDepthTest);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "depth_writing", mDepthWriting);
		Core_Serializer::DeserializeBoolean(pDoc, settingsNode, "color_writing", mColorWriting);
		Core_Serializer::DeserializeInt(pDoc, settingsNode, "gpu_instances", mGpuInstances);
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

					const Render::Render_UniformInfo* uniformInfo = mShader->GetUniformInfo(uniformName);

					if (uniformInfo)
					{
						switch (uniformInfo->type)
						{
						case Render::EUniformType::UNIFORM_BOOL:
							mUniformsData[uniformInfo->name] = Core_Serializer::DeserializeBoolean(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_INT:
							mUniformsData[uniformInfo->name] = Core_Serializer::DeserializeInt(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT:
							mUniformsData[uniformInfo->name] = Core_Serializer::DeserializeFloat(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT_VEC2:
							mUniformsData[uniformInfo->name] = Core_Serializer::DeserializeVec2(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT_VEC3:
							mUniformsData[uniformInfo->name] = Core_Serializer::DeserializeVec3(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_FLOAT_VEC4:
							mUniformsData[uniformInfo->name] = Core_Serializer::DeserializeVec4(pDoc, uniform, "value");
							break;

						case Render::EUniformType::UNIFORM_SAMPLER_2D:
							mUniformsData[uniformInfo->name] = Core_Serializer::DeserializeTexture(pDoc, uniform, "value");
							break;
						}
					}
				}
			}
		}
	}
}
