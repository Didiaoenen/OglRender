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

		std::unique_ptr<Window::Window_Device>			mDevice;
		std::unique_ptr<Window::Window_Window>			mWindow;
		std::unique_ptr<Window::Window_InputManager>	mInputManager;
		std::unique_ptr<Render::Render_Driver>			mDriver;
		std::unique_ptr<Core::Core_Renderer>			mRenderer;
		std::unique_ptr<Render::Render_ShapeDrawer>		mShapeDrawer;
		std::unique_ptr<UI::UI_UIManager>				mUIManager;
		std::unique_ptr<Editor_EditorResources>			mEditorResources;
		std::unique_ptr<Render::Render_UniformBuffer>	mEngineUBO;

		std::unique_ptr<Render::Render_ShaderStorageBuffer>	mLightSSBO;
		std::unique_ptr<Render::Render_ShaderStorageBuffer>	mSimulatedLightSSBO;

		Core::Core_SceneManager mSceneManager;

		Core::Core_ModelManager		mModelManager;
		Core::Core_TextureManager	mTextureManager;
		Core::Core_ShaderManager	mShaderManager;
		Core::Core_MaterialManager	mMaterialManager;

		Window::Window_WindowSettings mWindowSettings;

		Tools::Tools_IniFile mProjectSettings;
	};
}

