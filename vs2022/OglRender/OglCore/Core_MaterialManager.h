#pragma once

#include "Core_MaterialLoader.h"
#include "Core_AResourceManager.h"

namespace Core
{
	class Core_MaterialManager : public Core_AResourceManager<Core_Material>
	{
	public:
		virtual Core_Material* CreateResource(const std::string& p_path) override;

		virtual void DestroyResource(Core_Material* p_resource) override;

		virtual void ReloadResource(Core_Material* p_resource, const std::string& p_path) override;
	};
}

