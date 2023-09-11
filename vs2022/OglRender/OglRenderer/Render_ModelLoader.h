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

		static Render_Model* Create(const std::string& p_filepath, EModelParserFlags p_parserFlags = EModelParserFlags::NONE);

		static void Reload(Render_Model& p_model, const std::string& pFilePath, EModelParserFlags p_parserFlags = EModelParserFlags::NONE);

		static bool Destroy(Render_Model*& p_modelInstance);

	private:
		static Render_AssimpParser __ASSIMP;
	};
}

