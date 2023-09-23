#pragma once

#include "Core_MaterialLoader.h"
#include "Core_AResourceManager.h"

namespace Core
{
	class Core_MaterialManager : public Core_AResourceManager<Core_Material>
	{
	public:
		virtual Core_Material* CreateResource(const std::string& pPath) override;

		virtual void DestroyResource(Core_Material* pResource) override;

		virtual void ReloadResource(Core_Material* pResource, const std::string& pPath) override;
	};
}

