#pragma once

#include <string>	

#include "Render_Mesh.h"
#include "Render_EModelParserFlags.h"

namespace Render
{
	class Render_IModelParser
	{
	public:
		virtual bool LoadModel
		(
			const std::string& p_fileName,
			std::vector<Render_Mesh*>& p_meshes,
			std::vector<std::string>& pMaterials,
			EModelParserFlags pParserFlags
		) = 0;
	};

}