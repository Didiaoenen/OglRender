#include <OglUI/UI_DDTarget.h>

#include <OglTools/Tools_PathParser.h>

#include "Editor_EditorRenderer.h"
#include "Editor_EditorActions.h"
#include "Editor_AssetView.h"

Editor::Editor_AssetView::Editor_AssetView(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	Editor_AViewControllable(p_title, p_opened, p_windowSettings)
{
	mCamera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	mCamera.SetFar(5000.0f);

	m_resource = static_cast<Render::Render_Model*>(nullptr);
	m_image->AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [this](auto p_data)
		{
			std::string path = p_data.first;

			switch (Tools::Tools_PathParser::GetFileType(path))
			{
			case Tools::Tools_PathParser::EFileType::MODEL:
				if (auto resource = Core::Core_ServiceLocator::Get<Core::Core_ModelManager>().GetResource(path); resource)
				{
					m_resource = resource;
				}
				break;
			case Tools::Tools_PathParser::EFileType::TEXTURE:
				if (auto resource = Core::Core_ServiceLocator::Get<Core::Core_TextureManager>().GetResource(path); resource)
				{
					m_resource = resource;
				}
				break;

			case Tools::Tools_PathParser::EFileType::MATERIAL:
				if (auto resource = Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>().GetResource(path); resource)
				{
					m_resource = resource;
				}
				break;
			}
		};
}

void Editor::Editor_AssetView::_Render_Impl()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	m_fbo.Bind();

	baseRenderer.SetStencilMask(0xFF);
	baseRenderer.Clear(mCamera);
	baseRenderer.SetStencilMask(0x00);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	mEditorRenderer.RenderGrid(m_cameraPosition, m_gridColor);

	if (auto pval = std::get_if<Render::Render_Model*>(&m_resource); pval && *pval)
	{
		mEditorRenderer.RenderModelAsset(**pval);
	}

	if (auto pval = std::get_if<Render::Render_Texture*>(&m_resource); pval && *pval)
	{
		mEditorRenderer.RenderTextureAsset(**pval);
	}

	if (auto pval = std::get_if<Core::Core_Material*>(&m_resource); pval && *pval)
	{
		mEditorRenderer.RenderMaterialAsset(**pval);
	}

	baseRenderer.ApplyStateMask(glState);

	m_fbo.Unbind();
}

void Editor::Editor_AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

Editor::Editor_AssetView::ViewableResource Editor::Editor_AssetView::GetResource() const
{
	return m_resource;
}
