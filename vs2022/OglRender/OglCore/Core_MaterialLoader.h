#pragma once

#include "Core_Material.h"

namespace Core
{
	class Core_MaterialLoader
	{
	public:
		Core_MaterialLoader() = delete;

		static Core_Material* Create(const std::string& pPath);

		static void Reload(Core_Material& p_material, const std::string& pPath);

		static void Save(Core_Material& p_material, const std::string& pPath);

		static bool Destroy(Core_Material*& p_material);
	};
}

