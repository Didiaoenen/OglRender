#pragma once

#include <variant>

#include <OglCore/Core_Material.h>

#include "Editor_AViewControllable.h"

namespace Editor
{
	class Editor_AssetView : public Editor_AViewControllable
	{
	public:
		using ViewableResource = std::variant<Render::Render_Model*, Render::Render_Texture*, Core::Core_Material*>;

		Editor_AssetView
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		virtual void _Render_Impl() override;

		void SetResource(ViewableResource pResource);

		ViewableResource GetResource() const;

	private:
		ViewableResource mResource;
	};
}

