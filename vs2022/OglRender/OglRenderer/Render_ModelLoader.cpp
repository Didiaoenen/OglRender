#include "Render_ModelLoader.h"

Render::Render_AssimpParser Render::Render_ModelLoader::__ASSIMP;

Render::Render_Model* Render::Render_ModelLoader::Create(const std::string& p_filepath, EModelParserFlags p_parserFlags)
{
	Render_Model* result = new Render_Model(p_filepath);

	if (__ASSIMP.LoadModel(p_filepath, result->m_meshes, result->m_materialNames, p_parserFlags))
	{
		result->ComputeBoundingSphere();
		return result;
	}

	delete result;

	return nullptr;
}

void Render::Render_ModelLoader::Reload(Render_Model& p_model, const std::string& p_filePath, EModelParserFlags p_parserFlags)
{
	Render_Model* newModel = Create(p_filePath, p_parserFlags);

	if (newModel)
	{
		p_model.m_meshes = newModel->m_meshes;
		p_model.m_materialNames = newModel->m_materialNames;
		p_model.m_boundingSphere = newModel->m_boundingSphere;
		newModel->m_meshes.clear();
		delete newModel;
	}
}

bool Render::Render_ModelLoader::Destroy(Render_Model*& p_modelInstance)
{
	if (p_modelInstance)
	{
		delete p_modelInstance;
		p_modelInstance = nullptr;

		return true;
	}

	return false;
}
