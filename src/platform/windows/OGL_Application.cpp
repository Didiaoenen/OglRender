#include "rhi/opengl/OGL_OpenGLGraphicsManager.h"
#include "rhi/opengl/OGL_OpenGLPipelineStateManager.h"
#include "manager/OGL_AssetLoader.h"
#include "manager/OGL_SceneManager.h"
#include "sceneGraph/OGL_EditorCamera.h"
#include "OGL_Application.h"

using namespace OGL;

OGL_Application* OGL_Application::mApp = nullptr;

OGL_Application::OGL_Application()
{
	info.title = "Default Window";
	info.majorVersion = 3;
	info.minorVersion = 3;
	info.windowWidth = 960;
	info.windowHeight = 720;

	if (!WindowHint())
	{
		std::cout << "Faild to create GLFW window" << std::endl;
		return;
	}

	if (!InitGLAD())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	mGraphicManager = new OGL_OpenGLGraphicsManager();
	mPipelineStateManager = new OGL_OpenGLPipelineStateManager();

	mSceneManager = new OGL_SceneManager();
	mAssetLoader = new OGL_AssetLoader();

	RegisterRuntimeModule(mGraphicManager);
	RegisterRuntimeModule(mPipelineStateManager);
	RegisterRuntimeModule(mSceneManager);
	RegisterRuntimeModule(mAssetLoader);

	mEditorCamera = new OGL_EditorCamera();
	mEditorCamera->mCameraInfo.fov = 60.0f;
	mEditorCamera->mCameraInfo.width = info.windowWidth;
	mEditorCamera->mCameraInfo.height = info.windowHeight;
	mEditorCamera->mCameraInfo.zFar = 100.0f;
	mEditorCamera->mCameraInfo.zNear = 0.1f;
	mEditorCamera->mZoom = mEditorCamera->mCameraInfo.fov;
}

bool OGL_Application::Initialize()
{
	OGL_BaseApplication::Initialize();
    return true;
}

void OGL_Application::Finalize()
{
}

void OGL_Application::Setup()
{
	mApp = this;

	//
	mSceneManager->LoadScene("blender/AnisiaRoon_v1.0_2.79.gltf");
}

void OGL_Application::Update(double dt)
{
	OGL_BaseApplication::Tick();
}

void OGL_Application::Input()
{
}

void OGL_Application::Run()
{
	Setup();

	bool running = true;

	do
	{
		double time = glfwGetTime();
		mDT = time - mLastTime;
		mLastTime = time;

		ProcessInput(mWindow);

		Update(time);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();

		running = !glfwWindowShouldClose(mWindow);
	} while (running);

	Finalize();

	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void OGL_Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OGL_Application::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (mFirstMouse)
	{
		mLastX = xpos;
		mLastY = ypos;
		mFirstMouse = false;
	}

	double xoffset = xpos - mLastX;
	double yoffset = ypos - mLastY;

	mLastX = xpos;
	mLastY = ypos;

	mEditorCamera->ProcessMouseMovement(xoffset, yoffset);
}

void OGL_Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	mEditorCamera->ProcessMouseScroll(yoffset);
}

void* OGL_Application::GetMainWindowHandler()
{
	return mWindow;
}

void OGL_Application::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mEditorCamera->ProcessKeyboard(CameraMovement::Forward, mDT);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mEditorCamera->ProcessKeyboard(CameraMovement::Backward, mDT);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mEditorCamera->ProcessKeyboard(CameraMovement::Left, mDT);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mEditorCamera->ProcessKeyboard(CameraMovement::Right, mDT);
	}

	Input();
}

void OGL_Application::GLFWWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	if (mApp)
	{
		mApp->WindowSizeCallback(window, width, height);
	}
}

void OGL_Application::GLFWWindowScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (mApp)
	{
		mApp->ScrollCallback(window, xoffset, yoffset);
	}
}

void OGL_Application::GLFWWindowMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (mApp)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			mApp->MouseCallback(window, xpos, ypos);
		}
		else
		{
			mApp->mFirstMouse = true;
		}
	}
}

bool OGL_Application::InitGLAD()
{
	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool OGL_Application::WindowHint()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		return false;
	}
	glfwMakeContextCurrent(mWindow);

	glfwSetWindowSizeCallback(mWindow, OGL_Application::GLFWWindowSizeCallback);

	glfwSetScrollCallback(mWindow, OGL_Application::GLFWWindowScrollCallback);
	glfwSetCursorPosCallback(mWindow, OGL_Application::GLFWWindowMouseCallback);

	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}