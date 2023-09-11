#pragma once

#include <OglRenderer/Render_Texture.h>

#include "Core_AResourceManager.h"

namespace Core
{
	class Core_TextureManager : public Core_AResourceManager<Render::Render_Texture>
	{
	public:
		virtual Render::Render_Texture* CreateResource(const std::string& pPath) override;

		virtual void DestroyResource(Render::Render_Texture* p_resource) override;

		virtual void ReloadResource(Render::Render_Texture* p_resource, const std::string& pPath) override;
	};
}

