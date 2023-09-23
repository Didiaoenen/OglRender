#pragma once

#include <string>

#include "Render_Model.h"
#include "Render_AssimpParser.h"

namespace Render
{
	class Render_ModelLoader
	{
	public:
		Render_ModelLoader() = delete;

		static Render_Model* Create(const std::string& pFilepath, EModelParserFlags pParserFlags = EModelParserFlags::NONE);

		static void Reload(Render_Model& pModel, const std::string& pFilePath, EModelParserFlags pParserFlags = EModelParserFlags::NONE);

		static bool Destroy(Render_Model*& pModelInstance);

	private:
		static Render_AssimpParser __ASSIMP;
	};
}

