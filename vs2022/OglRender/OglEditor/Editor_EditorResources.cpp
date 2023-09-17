#include <OglCore/Core_GUIDrawer.h>

#include <OglRenderer/Render_ETextureFilteringMode.h>

#include <OglTools/Tools_PathParser.h>

#include "Editor_RawShaders.h"
#include "Editor_RawTextures.h"
#include "Editor_EditorResources.h"

Editor::Editor_EditorResources::Editor_EditorResources(const std::string& p_editorAssetsPath)
{
	std::string buttonsFolder = p_editorAssetsPath + "Textures\\Buttons\\";
	std::string iconsFolder = p_editorAssetsPath + "Textures\\Icons\\";
	std::string modelsFolder = p_editorAssetsPath + "Models\\";
	std::string shadersFolder = p_editorAssetsPath + "Shaders\\";

	Render::EModelParserFlags modelParserFlags = Render::EModelParserFlags::NONE;

	modelParserFlags |= Render::EModelParserFlags::TRIANGULATE;
	modelParserFlags |= Render::EModelParserFlags::GEN_SMOOTH_NORMALS;
	modelParserFlags |= Render::EModelParserFlags::OPTIMIZE_MESHES;
	modelParserFlags |= Render::EModelParserFlags::OPTIMIZE_GRAPH;
	modelParserFlags |= Render::EModelParserFlags::FIND_INSTANCES;
	modelParserFlags |= Render::EModelParserFlags::CALC_TANGENT_SPACE;
	modelParserFlags |= Render::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	modelParserFlags |= Render::EModelParserFlags::DEBONE;
	modelParserFlags |= Render::EModelParserFlags::FIND_INVALID_DATA;
	modelParserFlags |= Render::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	modelParserFlags |= Render::EModelParserFlags::GEN_UV_COORDS;
	//modelParserFlags |= Render::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	modelParserFlags |= Render::EModelParserFlags::GLOBAL_SCALE;

	Render::ETextureFilteringMode firstFilterEditor = Render::ETextureFilteringMode::LINEAR;
	Render::ETextureFilteringMode secondFilterEditor = Render::ETextureFilteringMode::LINEAR;

	Render::ETextureFilteringMode firstFilterBillboard = Render::ETextureFilteringMode::NEAREST;
	Render::ETextureFilteringMode secondFilterBillboard = Render::ETextureFilteringMode::NEAREST;

	{
		std::vector<uint64_t> raw = BUTTON_PLAY;
		m_textures["Button_Play"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_PAUSE;
		m_textures["Button_Pause"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_STOP;
		m_textures["Button_Stop"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_NEXT;
		m_textures["Button_Next"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_REFRESH;
		m_textures["Button_Refresh"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FILE;
		m_textures["Icon_Unknown"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FOLDER;
		m_textures["Icon_Folder"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_TEXTURE;
		m_textures["Icon_Texture"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_MODEL;
		m_textures["Icon_Model"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SHADER;
		m_textures["Icon_Shader"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_MATERIAL;
		m_textures["Icon_Material"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SCENE;
		m_textures["Icon_Scene"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SOUND;
		m_textures["Icon_Sound"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SCRIPT;
		m_textures["Icon_Script"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FONT;
		m_textures["Icon_Font"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BILL_PLIGHT;
		m_textures["Bill_Point_Light"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_SLIGHT;
		m_textures["Bill_Spot_Light"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_DLIGHT;
		m_textures["Bill_Directional_Light"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_ABLIGHT;
		m_textures["Bill_Ambient_Box_Light"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_ASLIGHT;
		m_textures["Bill_Ambient_Sphere_Light"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	m_models["Cube"] = Render::Render_ModelLoader::Create(modelsFolder + "Cube.fbx", modelParserFlags);
	m_models["Cylinder"] = Render::Render_ModelLoader::Create(modelsFolder + "Cylinder.fbx", modelParserFlags);
	m_models["Plane"] = Render::Render_ModelLoader::Create(modelsFolder + "Plane.fbx", modelParserFlags);
	m_models["Vertical_Plane"] = Render::Render_ModelLoader::Create(modelsFolder + "Vertical_Plane.fbx", modelParserFlags);
	m_models["Roll"] = Render::Render_ModelLoader::Create(modelsFolder + "Roll.fbx", modelParserFlags);
	m_models["Sphere"] = Render::Render_ModelLoader::Create(modelsFolder + "Sphere.fbx", modelParserFlags);
	m_models["Arrow_Translate"] = Render::Render_ModelLoader::Create(modelsFolder + "Arrow_Translate.fbx", modelParserFlags);
	m_models["Arrow_Rotate"] = Render::Render_ModelLoader::Create(modelsFolder + "Arrow_Rotate.fbx", modelParserFlags);
	m_models["Arrow_Scale"] = Render::Render_ModelLoader::Create(modelsFolder + "Arrow_Scale.fbx", modelParserFlags);
	m_models["Arrow_Picking"] = Render::Render_ModelLoader::Create(modelsFolder + "Arrow_Picking.fbx", modelParserFlags);
	m_models["Camera"] = Render::Render_ModelLoader::Create(modelsFolder + "Camera.fbx", modelParserFlags);

	auto gridSource = Editor_RawShaders::GetGrid();
	auto gizmoSource = Editor_RawShaders::GetGizmo();
	auto billboardSource = Editor_RawShaders::GetBillboard();
	m_shaders["Grid"] = Render::Render_ShaderLoader::CreateFromSource(gridSource.first, gridSource.second);
	m_shaders["Gizmo"] = Render::Render_ShaderLoader::CreateFromSource(gizmoSource.first, gizmoSource.second);
	m_shaders["Billboard"] = Render::Render_ShaderLoader::CreateFromSource(billboardSource.first, billboardSource.second);

	{
		std::vector<uint64_t> raw = EMPTY_TEXTURE;
		m_textures["Empty_Texture"] = Render::Render_TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
		Core::Core_GUIDrawer::ProvideEmptyTexture(*m_textures["Empty_Texture"]);
	}
}

Editor::Editor_EditorResources::~Editor_EditorResources()
{
	for (auto [id, texture] : m_textures)
	{
		Render::Render_TextureLoader::Destroy(texture);
	}

	for (auto [id, mesh] : m_models)
	{
		Render::Render_ModelLoader::Destroy(mesh);
	}

	for (auto [id, shader] : m_shaders)
	{
		Render::Render_ShaderLoader::Destroy(shader);
	}
}

Render::Render_Texture* Editor::Editor_EditorResources::GetFileIcon(const std::string& p_filename)
{
	return GetTexture("Icon_" + Tools::Tools_PathParser::FileTypeToString(Tools::Tools_PathParser::GetFileType(p_filename)));
}

Render::Render_Texture* Editor::Editor_EditorResources::GetTexture(const std::string& pId)
{
	if (m_textures.find(pId) != m_textures.end())
	{
		return m_textures.at(pId);
	}

	return nullptr;
}

Render::Render_Model* Editor::Editor_EditorResources::GetModel(const std::string& pId)
{
	if (m_models.find(pId) != m_models.end())
	{
		return m_models.at(pId);
	}

	return nullptr;
}

Render::Render_Shader* Editor::Editor_EditorResources::GetShader(const std::string& pId)
{
	if (m_shaders.find(pId) != m_shaders.end())
	{
		return m_shaders.at(pId);
	}

	return nullptr;
}
