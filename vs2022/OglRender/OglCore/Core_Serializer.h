#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <OglUI/UI_Color.h>
#include <OglTools/tinyxml2.h>

namespace Render
{
	class Render_Model;
	class Render_Shader;
	class Render_Texture;
}

namespace Core
{
	class Core_Material;

	class Core_Serializer
	{
	public:
#pragma region SERIALIZATION_HELPERS
		static void SerializeBoolean(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, bool pValue);
		static void SerializeString(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const std::string& pValue);
		static void SerializeFloat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, float pValue);
		static void SerializeDouble(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, double pValue);
		static void SerializeInt(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int pValue);
		static void SerializeUint(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, unsigned pValue);
		static void SerializeInt64(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int64_t pValue);
		static void SerializeVec2(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::vec2& pValue);
		static void SerializeVec3(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::vec3& pValue);
		static void SerializeVec4(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::vec4& pValue);
		static void SerializeQuat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::quat& pValue);
		static void SerializeColor(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const UI::Color& pValue);
		static void SerializeModel(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Model* pValue);
		static void SerializeShader(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Shader* pValue);
		static void SerializeTexture(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Texture* pValue);
		static void SerializeMaterial(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Core_Material* pValue);
#pragma endregion

#pragma region DESERIALIZATION_HELPERS
		static void DeserializeBoolean(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, bool& pOut);
		static void DeserializeString(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, std::string& pOut);
		static void DeserializeFloat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, float& pOut);
		static void DeserializeDouble(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, double& pOut);
		static void DeserializeInt(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int& pOut);
		static void DeserializeUint(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, unsigned& pOut);
		static void DeserializeInt64(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int64_t& pOut);
		static void DeserializeVec2(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::vec2& pOut);
		static void DeserializeVec3(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::vec3& pOut);
		static void DeserializeVec4(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::vec4& pOut);
		static void DeserializeQuat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::quat& pOut);
		static void DeserializeColor(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, UI::Color& pOut);
		static void DeserializeModel(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Model*& pOut);
		static void DeserializeShader(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Shader*& pOut);
		static void DeserializeTexture(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Texture*& pOut);
		static void DeserializeMaterial(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Core_Material*& pOut);

		static bool DeserializeBoolean(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static std::string DeserializeString(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static float DeserializeFloat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static double DeserializeDouble(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static int DeserializeInt(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static unsigned DeserializeUint(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static int64_t DeserializeInt64(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static glm::vec2 DeserializeVec2(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static glm::vec3 DeserializeVec3(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static glm::vec4 DeserializeVec4(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static glm::quat DeserializeQuat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static UI::Color DeserializeColor(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static Render::Render_Model* DeserializeModel(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static Render::Render_Shader* DeserializeShader(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static Render::Render_Texture* DeserializeTexture(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
		static Core_Material* DeserializeMaterial(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName);
#pragma endregion
	};
}

