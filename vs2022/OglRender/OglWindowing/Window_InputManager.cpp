#include "Window_InputManager.h"

Window::Window_InputManager::Window_InputManager(Window_Window& pWindow)
	: mWindow(pWindow)
{
	//mKeyPressedListener = mWindow.KeyPressedEvent.AddListener(std::bind(&InputManager::OnKeyPressed, this, std::placeholders::_1));
	//mKeyReleasedListener = mWindow.KeyReleasedEvent.AddListener(std::bind(&InputManager::OnKeyReleased, this, std::placeholders::_1));
	//mMouseButtonPressedListener = mWindow.MouseButtonPressedEvent.AddListener(std::bind(&InputManager::OnMouseButtonPressed, this, std::placeholders::_1));
	//mMouseButtonReleasedListener = mWindow.MouseButtonReleasedEvent.AddListener(std::bind(&InputManager::OnMouseButtonReleased, this, std::placeholders::_1));
}

Window::Window_InputManager::~Window_InputManager()
{
	//mWindow.KeyPressedEvent.RemoveListener(mKeyPressedListener);
	//mWindow.KeyReleasedEvent.RemoveListener(mKeyReleasedListener);
	//mWindow.MouseButtonPressedEvent.RemoveListener(mMouseButtonPressedListener);
	//mWindow.MouseButtonReleasedEvent.RemoveListener(mMouseButtonReleasedListener);
}

Window::EKeyState Window::Window_InputManager::GetKeyState(EKey pKey) const
{
	//switch (glfwGetKey(mWindow.GetGlfwWindow(), static_cast<int>(pKey)))
	//{
	//case GLFW_PRESS:	return EKeyState::KEY_DOWN;
	//case GLFW_RELEASE:	return EKeyState::KEY_UP;
	//}

	return EKeyState::KEY_UP;
}

Window::EMouseButtonState Window::Window_InputManager::GetMouseButtonState(EMouseButton pButton) const
{
	//switch (glfwGetMouseButton(mWindow.GetGlfwWindow(), static_cast<int>(pButton)))
	//{
	//case GLFW_PRESS:	return EMouseButtonState::MOUSE_DOWN;
	//case GLFW_RELEASE:	return EMouseButtonState::MOUSE_UP;
	//}

	return EMouseButtonState::MOUSE_UP;
}

bool Window::Window_InputManager::IsKeyPressed(EKey pKey) const
{
	return mKeyEvents.find(pKey) != mKeyEvents.end() && mKeyEvents.at(pKey) == EKeyState::KEY_DOWN;
}

bool Window::Window_InputManager::IsKeyReleased(EKey pKey) const
{
	return mKeyEvents.find(pKey) != mKeyEvents.end() && mKeyEvents.at(pKey) == EKeyState::KEY_UP;
}

bool Window::Window_InputManager::IsMouseButtonPressed(EMouseButton pButton) const
{
	return mMouseButtonEvents.find(pButton) != mMouseButtonEvents.end() && mMouseButtonEvents.at(pButton) == EMouseButtonState::MOUSE_DOWN;
}

bool Window::Window_InputManager::IsMouseButtonReleased(EMouseButton pButton) const
{
	return mMouseButtonEvents.find(pButton) != mMouseButtonEvents.end() && mMouseButtonEvents.at(pButton) == EMouseButtonState::MOUSE_UP;
}

std::pair<double, double> Window::Window_InputManager::GetMousePosition() const
{
	std::pair<double, double> result;
	//glfwGetCursorPos(mWindow.GetGlfwWindow(), &result.first, &result.second);
	return result;
}

void Window::Window_InputManager::ClearEvents()
{
	mKeyEvents.clear();
	mMouseButtonEvents.clear();
}

void Window::Window_InputManager::OnKeyPressed(int pKey)
{
	mKeyEvents[static_cast<EKey>(pKey)] = EKeyState::KEY_DOWN;
}

void Window::Window_InputManager::OnKeyReleased(int pKey)
{
	mKeyEvents[static_cast<EKey>(pKey)] = EKeyState::KEY_UP;
}

void Window::Window_InputManager::OnMouseButtonPressed(int pButton)
{
	mMouseButtonEvents[static_cast<EMouseButton>(pButton)] = EMouseButtonState::MOUSE_DOWN;
}

void Window::Window_InputManager::OnMouseButtonReleased(int pButton)
{
	mMouseButtonEvents[static_cast<EMouseButton>(pButton)] = EMouseButtonState::MOUSE_UP;
}
