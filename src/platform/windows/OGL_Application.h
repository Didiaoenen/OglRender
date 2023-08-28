#pragma once

#include <iostream>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "manager/OGL_BaseApplication.h"

namespace OGL
{

class OGL_GraphicsManager;
class OGL_PipelineStateManager;
class OGL_SceneManager;
class OGL_AnimationManager;
class OGL_AssetLoader;
class OGL_EditorCamera;

struct AppInfo
{
	std::string title;
	float windowWidth;
	float windowHeight;
	int majorVersion;
	int minorVersion;
};

class OGL_Application : public OGL_BaseApplication
{
public:
	OGL_Application();
	virtual ~OGL_Application() override = default;

	virtual bool Initialize();
	virtual void Finalize();
		
	virtual void Setup();

	virtual void Update(double dt);

	virtual void Input();

	virtual void WindowSizeCallback(GLFWwindow* window, int width, int height);
	virtual void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	virtual void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	virtual void* GetMainWindowHandler() override;

	void Run();

	bool InitGLAD();

	bool WindowHint();

	void ProcessInput(GLFWwindow* window);

protected:
	AppInfo info;

	static OGL_Application* mApp;

	GLFWwindow* mWindow{ nullptr };

	double mDT{ 0 };
	double mLastTime{ 0 };

	static void GLFWWindowSizeCallback(GLFWwindow* window, int width, int height);

	static void GLFWWindowMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void GLFWWindowScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
	bool mFirstMouse{ true };
	double mLastX{ 0 };
	double mLastY{ 0 };

	OGL_GraphicsManager* mGraphicManager;
	OGL_PipelineStateManager* mPipelineStateManager;
	OGL_SceneManager* mSceneManager;
	OGL_AnimationManager* mAnimationManager;
	OGL_AssetLoader* mAssetLoader;

public:
	OGL_EditorCamera* mEditorCamera;

};

#define DECLARE_MAIN(App)			\
int main()							\
{									\
	App* app = new App();			\
	if (app->Initialize())			\
	{								\
		app->Run();					\
	}								\
	return 0;						\
}
}