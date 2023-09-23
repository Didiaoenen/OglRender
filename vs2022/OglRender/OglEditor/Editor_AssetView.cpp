#include <OglUI/UI_DDTarget.h>

#include <OglTools/Tools_PathParser.h>

#include "Editor_EditorRenderer.h"
#include "Editor_EditorActions.h"
#include "Editor_AssetView.h"

Editor::Editor_AssetView::Editor_AssetView(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	Editor_AViewControllable(pTitle, pOpened, pWindowSettings)
{
	mCamera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	mCamera.SetFar(5000.0f);

	m_resource = static_cast<Render::Render_Model*>(nullptr);
	mImage->AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [this](auto pData)
		{
			std::string path = pData.first;

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

	auto& baseRenderer = *EDITOR_CONTEXT(mRenderer).get();

	mFbo.Bind();

	baseRenderer.SetStencilMask(0xFF);
	baseRenderer.Clear(mCamera);
	baseRenderer.SetStencilMask(0x00);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	mEditorRenderer.RenderGrid(mCameraPosition, mGridColor);

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

	mFbo.Unbind();
}

void Editor::Editor_AssetView::SetResource(ViewableResource pResource)
{
	m_resource = pResource;
}

Editor::Editor_AssetView::ViewableResource Editor::Editor_AssetView::GetResource() const
{
	return m_resource;
}
