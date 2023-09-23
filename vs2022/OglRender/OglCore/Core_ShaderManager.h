#pragma once

#include <OglRenderer/Render_ShaderLoader.h>

#include "Core_AResourceManager.h"

namespace Core
{
	class Core_ShaderManager : public Core_AResourceManager<Render::Render_Shader>
	{
	public:
		virtual Render::Render_Shader* CreateResource(const std::string& pPath) override;

		virtual void DestroyResource(Render::Render_Shader* pResource) override;

		virtual void ReloadResource(Render::Render_Shader* pResource, const std::string& pPath) override;
	};
}

