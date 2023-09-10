#pragma once

#include <OglRenderer/Render_ShaderLoader.h>

#include "Core_AResourceManager.h"

namespace Core
{
	class Core_ShaderManager : public Core_AResourceManager<Render::Render_Shader>
	{
	public:
		virtual Render::Render_Shader* CreateResource(const std::string& p_path) override;

		virtual void DestroyResource(Render::Render_Shader* p_resource) override;

		virtual void ReloadResource(Render::Render_Shader* p_resource, const std::string& p_path) override;
	};
}

