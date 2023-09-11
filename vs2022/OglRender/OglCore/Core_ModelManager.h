#pragma once

#include <OglRenderer/Render_ModelLoader.h>

#include "Core_AResourceManager.h"

namespace Core
{
	class Core_ModelManager : public Core_AResourceManager<Render::Render_Model>
	{
	public:
		virtual Render::Render_Model* CreateResource(const std::string& pPath) override;

		virtual void DestroyResource(Render::Render_Model* p_resource) override;

		virtual void ReloadResource(Render::Render_Model* p_resource, const std::string& pPath) override;
	};
}

