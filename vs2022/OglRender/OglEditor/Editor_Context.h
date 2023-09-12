#pragma once

#include <OglTools/Tools_IniFile.h>

#include <OglRenderer/Render_ShapeDrawer.h>
#include <OglRenderer/Render_UniformBuffer.h>
#include <OglRenderer/Render_ShaderStorageBuffer.h>

#include <OglWindow/Window_Device.h>
#include <OglWindow/Window_Window.h>
#include <OglWindow/Window_InputManager.h>

#include <OglCore/Core_Renderer.h>
#include <OglCore/Core_ModelManager.h>
#include <OglCore/Core_SceneManager.h>
#include <OglCore/Core_ShaderManager.h>
#include <OglCore/Core_TextureManager.h>
#include <OglCore/Core_MaterialManager.h>

#include <OglUI/UI_UIManager.h>

#include "Editor_EditorResources.h"

namespace Editor
{
	class Editor_Context
	{
	public:
		Editor_Context(const std::string& pProjectPath, const std::string& pProjectName);

		~Editor_Context();

		void ResetProjectSettings();

		bool IsProjectSettingsIntegrityVerified();

		void ApplyProjectSettings();

	public:
		const std::string mProjectPath;
		const std::string mProjectName;
		const std::string mProjectFilePath;
		const std::string mEngineAssetsPath;
		const std::string mProjectAssetsPath;
		const std::string mProjectScriptsPath;
		const std::string mEditorAssetsPath;

		std::unique_ptr<Window::Window_Device>			device;
		std::unique_ptr<Window::Window_Window>			window;
		std::unique_ptr<Window::Window_InputManager>	inputManager;
		std::unique_ptr<Render::Render_Driver>			driver;
		std::unique_ptr<Core::Core_Renderer>			renderer;
		std::unique_ptr<Render::Render_ShapeDrawer>		shapeDrawer;
		std::unique_ptr<UI::UI_UIManager>				uiManager;
		std::unique_ptr<Editor_EditorResources>			editorResources;
		std::unique_ptr<Render::Render_UniformBuffer>	engineUBO;

		std::unique_ptr<Render::Render_ShaderStorageBuffer>	lightSSBO;
		std::unique_ptr<Render::Render_ShaderStorageBuffer>	simulatedLightSSBO;

		Core::Core_SceneManager sceneManager;

		Core::Core_ModelManager		modelManager;
		Core::Core_TextureManager	textureManager;
		Core::Core_ShaderManager	shaderManager;
		Core::Core_MaterialManager	materialManager;

		Window::Window_WindowSettings windowSettings;

		Tools::Tools_IniFile projectSettings;
	};
}

