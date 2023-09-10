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
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		virtual void _Render_Impl() override;

		void SetResource(ViewableResource p_resource);

		ViewableResource GetResource() const;

	private:
		ViewableResource m_resource;
	};
}

