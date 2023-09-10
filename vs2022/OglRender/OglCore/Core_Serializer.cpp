#include "Core_ModelManager.h"
#include "Core_ShaderManager.h"
#include "Core_TextureManager.h"
#include "Core_MaterialManager.h"
#include "Core_ServiceLocator.h"
#include "Core_Serializer.h"

void Core::Core_Serializer::SerializeBoolean(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, bool pValue)
{
	tinyxml2::XMLElement* element = pDoc.NewElement(pName.c_str());
	element->SetText(pValue);
	pNode->InsertEndChild(element);
}

void Core::Core_Serializer::SerializeString(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const std::string& pValue)
{
	tinyxml2::XMLElement* element = pDoc.NewElement(pName.c_str());
	element->SetText(pValue.c_str());
	pNode->InsertEndChild(element);
}

void Core::Core_Serializer::SerializeFloat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, float pValue)
{
	tinyxml2::XMLElement* element = pDoc.NewElement(pName.c_str());
	element->SetText(pValue);
	pNode->InsertEndChild(element);
}

void Core::Core_Serializer::SerializeDouble(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, double pValue)
{
	tinyxml2::XMLElement* element = pDoc.NewElement(pName.c_str());
	element->SetText(pValue);
	pNode->InsertEndChild(element);
}

void Core::Core_Serializer::SerializeInt(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int pValue)
{
	tinyxml2::XMLElement* element = pDoc.NewElement(pName.c_str());
	element->SetText(pValue);
	pNode->InsertEndChild(element);
}

void Core::Core_Serializer::SerializeUint(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, unsigned pValue)
{
	tinyxml2::XMLElement* element = pDoc.NewElement(pName.c_str());
	element->SetText(pValue);
	pNode->InsertEndChild(element);
}

void Core::Core_Serializer::SerializeInt64(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int64_t pValue)
{
	tinyxml2::XMLElement* element = pDoc.NewElement(pName.c_str());
	element->SetText(pValue);
	pNode->InsertEndChild(element);
}

void Core::Core_Serializer::SerializeVec2(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::vec2& pValue)
{
	tinyxml2::XMLNode* element = pDoc.NewElement(pName.c_str());
	pNode->InsertEndChild(element);

	tinyxml2::XMLElement* x = pDoc.NewElement("x");
	x->SetText(pValue.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = pDoc.NewElement("y");
	y->SetText(pValue.y);
	element->InsertEndChild(y);
}

void Core::Core_Serializer::SerializeVec3(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::vec3& pValue)
{
	tinyxml2::XMLNode* element = pDoc.NewElement(pName.c_str());
	pNode->InsertEndChild(element);

	tinyxml2::XMLElement* x = pDoc.NewElement("x");
	x->SetText(pValue.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = pDoc.NewElement("y");
	y->SetText(pValue.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = pDoc.NewElement("z");
	z->SetText(pValue.z);
	element->InsertEndChild(z);
}

void Core::Core_Serializer::SerializeVec4(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::vec4& pValue)
{
	tinyxml2::XMLNode* element = pDoc.NewElement(pName.c_str());
	pNode->InsertEndChild(element);

	tinyxml2::XMLElement* x = pDoc.NewElement("x");
	x->SetText(pValue.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = pDoc.NewElement("y");
	y->SetText(pValue.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = pDoc.NewElement("z");
	z->SetText(pValue.z);
	element->InsertEndChild(z);

	tinyxml2::XMLElement* w = pDoc.NewElement("w");
	w->SetText(pValue.w);
	element->InsertEndChild(w);
}

void Core::Core_Serializer::SerializeQuat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const glm::quat& pValue)
{
	tinyxml2::XMLNode* element = pDoc.NewElement(pName.c_str());
	pNode->InsertEndChild(element);

	tinyxml2::XMLElement* x = pDoc.NewElement("x");
	x->SetText(pValue.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = pDoc.NewElement("y");
	y->SetText(pValue.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = pDoc.NewElement("z");
	z->SetText(pValue.z);
	element->InsertEndChild(z);

	tinyxml2::XMLElement* w = pDoc.NewElement("w");
	w->SetText(pValue.w);
	element->InsertEndChild(w);
}

void Core::Core_Serializer::SerializeColor(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, const UI::Color& pValue)
{
	tinyxml2::XMLNode* element = pDoc.NewElement(pName.c_str());
	pNode->InsertEndChild(element);

	tinyxml2::XMLElement* r = pDoc.NewElement("r");
	r->SetText(pValue.r);
	element->InsertEndChild(r);

	tinyxml2::XMLElement* g = pDoc.NewElement("g");
	g->SetText(pValue.g);
	element->InsertEndChild(g);

	tinyxml2::XMLElement* b = pDoc.NewElement("b");
	b->SetText(pValue.b);
	element->InsertEndChild(b);

	tinyxml2::XMLElement* a = pDoc.NewElement("a");
	a->SetText(pValue.a);
	element->InsertEndChild(a);
}

void Core::Core_Serializer::SerializeModel(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Model* pValue)
{
	SerializeString(pDoc, pNode, pName.c_str(), pValue ? pValue->path : "?");
}

void Core::Core_Serializer::SerializeShader(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Shader* pValue)
{
	SerializeString(pDoc, pNode, pName.c_str(), pValue ? pValue->path : "?");
}

void Core::Core_Serializer::SerializeTexture(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Texture* pValue)
{
	SerializeString(pDoc, pNode, pName.c_str(), pValue ? pValue->path : "?");
}

void Core::Core_Serializer::SerializeMaterial(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Core_Material* pValue)
{
	SerializeString(pDoc, pNode, pName.c_str(), pValue ? pValue->path : "?");
}

void Core::Core_Serializer::DeserializeBoolean(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, bool& pOut)
{
	if (auto element = pNode->FirstChildElement(pName.c_str()); element)
		element->QueryBoolText(&pOut);
}

void Core::Core_Serializer::DeserializeString(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, std::string& pOut)
{
	if (auto element = pNode->FirstChildElement(pName.c_str()); element)
	{
		const char* result = element->GetText();
		pOut = result ? result : "";
	}
}

void Core::Core_Serializer::DeserializeFloat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, float& pOut)
{
	if (auto element = pNode->FirstChildElement(pName.c_str()); element)
		element->QueryFloatText(&pOut);
}

void Core::Core_Serializer::DeserializeDouble(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, double& pOut)
{
	if (auto element = pNode->FirstChildElement(pName.c_str()); element)
		element->QueryDoubleText(&pOut);
}

void Core::Core_Serializer::DeserializeInt(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int& pOut)
{
	if (auto element = pNode->FirstChildElement(pName.c_str()); element)
		element->QueryIntText(&pOut);
}

void Core::Core_Serializer::DeserializeUint(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, unsigned& pOut)
{
	if (auto element = pNode->FirstChildElement(pName.c_str()); element)
		element->QueryUnsignedText(&pOut);
}

void Core::Core_Serializer::DeserializeInt64(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, int64_t& pOut)
{
	if (auto element = pNode->FirstChildElement(pName.c_str()); element)
		element->QueryInt64Text(&pOut);
}

void Core::Core_Serializer::DeserializeVec2(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::vec2& pOut)
{
	if (auto node = pNode->FirstChildElement(pName.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&pOut.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&pOut.y);
	}
}

void Core::Core_Serializer::DeserializeVec3(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::vec3& pOut)
{
	if (auto node = pNode->FirstChildElement(pName.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&pOut.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&pOut.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&pOut.z);
	}
}

void Core::Core_Serializer::DeserializeVec4(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::vec4& pOut)
{
	if (auto node = pNode->FirstChildElement(pName.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&pOut.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&pOut.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&pOut.z);

		if (auto element = node->FirstChildElement("w"); element)
			element->QueryFloatText(&pOut.w);
	}
}

void Core::Core_Serializer::DeserializeQuat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, glm::quat& pOut)
{
	if (auto node = pNode->FirstChildElement(pName.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&pOut.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&pOut.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&pOut.z);

		if (auto element = node->FirstChildElement("w"); element)
			element->QueryFloatText(&pOut.w);
	}
}

void Core::Core_Serializer::DeserializeColor(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, UI::Color& pOut)
{
	if (auto node = pNode->FirstChildElement(pName.c_str()); node)
	{
		if (auto element = node->FirstChildElement("r"); element)
			element->QueryFloatText(&pOut.r);

		if (auto element = node->FirstChildElement("g"); element)
			element->QueryFloatText(&pOut.g);

		if (auto element = node->FirstChildElement("b"); element)
			element->QueryFloatText(&pOut.b);

		if (auto element = node->FirstChildElement("q"); element)
			element->QueryFloatText(&pOut.a);
	}
}

void Core::Core_Serializer::DeserializeModel(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Model*& pOut)
{
	if (std::string path = DeserializeString(pDoc, pNode, pName.c_str()); path != "?" && path != "")
		pOut = Core::Core_ServiceLocator::Get<Core::Core_ModelManager>().GetResource(path);
	else
		pOut = nullptr;
}

void Core::Core_Serializer::DeserializeShader(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Shader*& pOut)
{
	if (std::string path = DeserializeString(pDoc, pNode, pName.c_str()); path != "?" && path != "")
		pOut = Core::Core_ServiceLocator::Get<Core::Core_ShaderManager>().GetResource(path);
	else
		pOut = nullptr;
}

void Core::Core_Serializer::DeserializeTexture(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Render::Render_Texture*& pOut)
{
	if (std::string path = DeserializeString(pDoc, pNode, pName.c_str()); path != "?" && path != "")
		pOut = Core::Core_ServiceLocator::Get<Core::Core_TextureManager>().GetResource(path);
	else
		pOut = nullptr;
}

void Core::Core_Serializer::DeserializeMaterial(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName, Core_Material*& pOut)
{
	if (std::string path = DeserializeString(pDoc, pNode, pName.c_str()); path != "?" && path != "")
		pOut = Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>().GetResource(path);
	else
		pOut = nullptr;
}

bool Core::Core_Serializer::DeserializeBoolean(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	bool result;
	DeserializeBoolean(pDoc, pNode, pName, result);
	return result;
}

std::string Core::Core_Serializer::DeserializeString(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	std::string result;
	DeserializeString(pDoc, pNode, pName, result);
	return result;
}

float Core::Core_Serializer::DeserializeFloat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	float result;
	DeserializeFloat(pDoc, pNode, pName, result);
	return result;
}

double Core::Core_Serializer::DeserializeDouble(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	double result;
	DeserializeDouble(pDoc, pNode, pName, result);
	return result;
}

int Core::Core_Serializer::DeserializeInt(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	int result;
	DeserializeInt(pDoc, pNode, pName, result);
	return result;
}

unsigned Core::Core_Serializer::DeserializeUint(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	unsigned result;
	DeserializeUint(pDoc, pNode, pName, result);
	return result;
}

int64_t Core::Core_Serializer::DeserializeInt64(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	int64_t result;
	DeserializeInt64(pDoc, pNode, pName, result);
	return result;
}

glm::vec2 Core::Core_Serializer::DeserializeVec2(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	glm::vec2 result;
	DeserializeVec2(pDoc, pNode, pName, result);
	return result;
}

glm::vec3 Core::Core_Serializer::DeserializeVec3(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	glm::vec3 result;
	DeserializeVec3(pDoc, pNode, pName, result);
	return result;
}

glm::vec4 Core::Core_Serializer::DeserializeVec4(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	glm::vec4 result;
	DeserializeVec4(pDoc, pNode, pName, result);
	return result;
}

glm::quat Core::Core_Serializer::DeserializeQuat(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	glm::quat result;
	DeserializeQuat(pDoc, pNode, pName, result);
	return result;
}

UI::Color Core::Core_Serializer::DeserializeColor(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	UI::Color result;
	DeserializeColor(pDoc, pNode, pName, result);
	return result;
}

Render::Render_Model* Core::Core_Serializer::DeserializeModel(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	Render::Render_Model* result;
	DeserializeModel(pDoc, pNode, pName, result);
	return result;
}

Render::Render_Shader* Core::Core_Serializer::DeserializeShader(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	Render::Render_Shader* result;
	DeserializeShader(pDoc, pNode, pName, result);
	return result;
}

Render::Render_Texture* Core::Core_Serializer::DeserializeTexture(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	Render::Render_Texture* result;
	DeserializeTexture(pDoc, pNode, pName, result);
	return result;
}

Core::Core_Material* Core::Core_Serializer::DeserializeMaterial(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode, const std::string& pName)
{
	Core::Core_Material* result;
	DeserializeMaterial(pDoc, pNode, pName, result);
	return result;
}
