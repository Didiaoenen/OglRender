#include "Render_ModelLoader.h"

Render::Render_AssimpParser Render::Render_ModelLoader::__ASSIMP;

Render::Render_Model* Render::Render_ModelLoader::Create(const std::string& pFilepath, EModelParserFlags pParserFlags)
{
	Render_Model* result = new Render_Model(pFilepath);

	if (__ASSIMP.LoadModel(pFilepath, result->mMeshes, result->mMaterialNames, pParserFlags))
	{
		result->ComputeBoundingSphere();
		return result;
	}

	delete result;
	 
	return nullptr;
}

void Render::Render_ModelLoader::Reload(Render_Model& pModel, const std::string& pFilePath, EModelParserFlags pParserFlags)
{
	Render_Model* newModel = Create(pFilePath, pParserFlags);

	if (newModel)
	{
		pModel.mMeshes = newModel->mMeshes;
		pModel.mMaterialNames = newModel->mMaterialNames;
		pModel.mBoundingSphere = newModel->mBoundingSphere;
		newModel->mMeshes.clear();
		delete newModel;
	}
}

bool Render::Render_ModelLoader::Destroy(Render_Model* &pModelInstance)
{
	if (pModelInstance)
	{
		delete pModelInstance;
		pModelInstance = nullptr;

		return true;
	}

	return false;
}
