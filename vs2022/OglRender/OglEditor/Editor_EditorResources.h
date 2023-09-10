#pragma once

#include <OglCore/Core_ModelManager.h>
#include <OglCore/Core_ShaderManager.h>
#include <OglCore/Core_TextureManager.h>

namespace Editor
{
	class Editor_EditorResources
	{
	public:
		Editor_EditorResources(const std::string& p_editorAssetsPath);

		~Editor_EditorResources();

		Render::Render_Texture* GetFileIcon(const std::string& p_filename);

		Render::Render_Texture* GetTexture(const std::string& p_id);

		Render::Render_Model* GetModel(const std::string& p_id);

		Render::Render_Shader* GetShader(const std::string& p_id);

	private:
		std::unordered_map<std::string, Render::Render_Texture*> m_textures;
		std::unordered_map<std::string, Render::Render_Model*> m_models;
		std::unordered_map<std::string, Render::Render_Shader*> m_shaders;
	};
}

