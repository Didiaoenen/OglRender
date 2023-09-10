#include <OglUI/UI_DDTarget.h>

#include <OglTools/Tools_PathParser.h>

#include "Editor_EditorRenderer.h"
#include "Editor_EditorActions.h"
#include "Editor_AssetView.h"

Editor::Editor_AssetView::Editor_AssetView(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	Editor_AViewControllable(p_title, p_opened, p_windowSettings)
{
}

void Editor::Editor_AssetView::_Render_Impl()
{
}

void Editor::Editor_AssetView::SetResource(ViewableResource p_resource)
{
}

Editor::Editor_AssetView::ViewableResource Editor::Editor_AssetView::GetResource() const
{
	return ViewableResource();
}
