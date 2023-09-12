#include "Core_MaterialLoader.h"

Core::Core_Material* Core::Core_MaterialLoader::Create(const std::string& pPath)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(pPath.c_str());
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();

		Core_Material* material = new Core_Material();

		material->OnDeserialize(doc, root);

		return material;
	}
	else
	{
		return nullptr;
	}
}

void Core::Core_MaterialLoader::Reload(Core_Material& pMaterial, const std::string& pPath)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(pPath.c_str());
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();

		pMaterial.OnDeserialize(doc, root);
	}
}

void Core::Core_MaterialLoader::Save(Core_Material& pMaterial, const std::string& pPath)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* node = doc.NewElement("root");
	doc.InsertFirstChild(node);

	pMaterial.OnSerialize(doc, node);

	//if (doc.SaveFile(pPath.c_str()) == tinyxml2::XML_SUCCESS)
	//	OVLOG_INFO("[MATERIAL] \"" + pPath + "\": Saved");
	//else
	//	OVLOG_ERROR("[MATERIAL] \"" + pPath + "\": Failed to save");
}

bool Core::Core_MaterialLoader::Destroy(Core_Material*& pMaterial)
{
	if (pMaterial)
	{
		delete pMaterial;
		pMaterial = nullptr;

		return true;
	}

	return false;
}
