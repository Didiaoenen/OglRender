#pragma once

#include <unordered_map>

#include "Window_Window.h"
#include "Window_EKey.h"
#include "Window_EKeyState.h"
#include "Window_EMouseButton.h"
#include "Window_EMouseButtonState.h"

namespace Window
{
	class Window_InputManager
	{
	public:
		Window_InputManager(Window_Window& pWindow);

		~Window_InputManager();

		EKeyState GetKeyState(EKey pKey) const;

		EMouseButtonState GetMouseButtonState(EMouseButton pButton) const;

		bool IsKeyPressed(EKey pKey) const;

		bool IsKeyReleased(EKey pKey) const;

		bool IsMouseButtonPressed(EMouseButton pButton) const;

		bool IsMouseButtonReleased(EMouseButton pButton) const;

		std::pair<double, double> GetMousePosition() const;

		void ClearEvents();

	private:
		void OnKeyPressed(int pKey);
		void OnKeyReleased(int pKey);
		void OnMouseButtonPressed(int pButton);
		void OnMouseButtonReleased(int pButton);

	private:
		Window_Window& mWindow;

		Tools::ListenerID mKeyPressedListener;
		Tools::ListenerID mKeyReleasedListener;
		Tools::ListenerID mMouseButtonPressedListener;
		Tools::ListenerID mMouseButtonReleasedListener;

		std::unordered_map<EKey, EKeyState>					mKeyEvents;
		std::unordered_map<EMouseButton, EMouseButtonState>	mMouseButtonEvents;
	};
}

