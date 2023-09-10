#include <OglCore/Core_GUIDrawer.h>

#include <OglRenderer/Render_ETextureFilteringMode.h>

#include <OglTools/Tools_PathParser.h>

#include "Editor_RawShaders.h"
#include "Editor_RawTextures.h"
#include "Editor_EditorResources.h"

Editor::Editor_EditorResources::Editor_EditorResources(const std::string& p_editorAssetsPath)
{
}

Editor::Editor_EditorResources::~Editor_EditorResources()
{
}

Render::Render_Texture* Editor::Editor_EditorResources::GetFileIcon(const std::string& p_filename)
{
	return nullptr;
}

Render::Render_Texture* Editor::Editor_EditorResources::GetTexture(const std::string& p_id)
{
	return nullptr;
}

Render::Render_Model* Editor::Editor_EditorResources::GetModel(const std::string& p_id)
{
	return nullptr;
}

Render::Render_Shader* Editor::Editor_EditorResources::GetShader(const std::string& p_id)
{
	return nullptr;
}
