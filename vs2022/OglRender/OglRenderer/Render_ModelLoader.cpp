#include "Render_ModelLoader.h"

Render::Render_AssimpParser Render::Render_ModelLoader::__ASSIMP;

Render::Render_Model* Render::Render_ModelLoader::Create(const std::string& p_filepath, EModelParserFlags p_parserFlags)
{
	Render_Model* result = new Render_Model(p_filepath);

	if (__ASSIMP.LoadModel(p_filepath, result->mMeshes, result->mMaterialNames, p_parserFlags))
	{
		result->ComputeBoundingSphere();
		return result;
	}

	delete result;

	return nullptr;
}

void Render::Render_ModelLoader::Reload(Render_Model& pModel, const std::string& pFilePath, EModelParserFlags p_parserFlags)
{
	Render_Model* newModel = Create(pFilePath, p_parserFlags);

	if (newModel)
	{
		pModel.mMeshes = newModel->mMeshes;
		pModel.mMaterialNames = newModel->mMaterialNames;
		pModel.mBoundingSphere = newModel->mBoundingSphere;
		newModel->mMeshes.clear();
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
