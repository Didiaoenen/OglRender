#pragma once

#include <string>
#include <unordered_map>

#include "UI_Canvas.h"
#include "UI_EStyle.h"

namespace UI
{
	class UI_Manager
	{
	public:
		UI_Manager(GLFWwindow* pGlfwWindow, EStyle pStyle = EStyle::IM_DARK_STYLE, const std::string& pGlslVersion = "#version 150");

		~UI_Manager();

		void ApplyStyle(EStyle pStyle);

		bool LoadFont(const std::string& pId, const std::string& pPath, float pFontSize);

		bool UnloadFont(const std::string& pId);

		bool UseFont(const std::string& pId);

		void UseDefaultFont();

		void EnableEditorLayoutSave(bool pValue);

		bool IsEditorLayoutSaveEnabled() const;

		void SetEditorLayoutSaveFilename(const std::string& pFilename);

		void SetEditorLayoutAutosaveFrequency(float pFrequency);

		float GetEditorLayoutAutosaveFrequency(float pFrequeny);

		void EnableDocking(bool pValue);

		void ResetLayout(const std::string& pConfig) const;

		bool IsDockingEnabled() const;

		void SetCanvas(UI_Canvas& pCanvas);

		void RemoveCanvas();

		void Render();

	private:
		void PushCurrentFont();
		void PopCurrentFont();

	private:
		bool mDockingState;
		UI_Canvas* mCurrentCanvas = nullptr;
		std::unordered_map<std::string, ImFont*> mFonts;
		std::string mLayoutSaveFilename = "imgui.ini";
	};
}