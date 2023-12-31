#pragma once

#include "Editor_Editor.h"
#include "Editor_Context.h"

namespace Editor
{
	class Editor_Application
	{
	public:
		Editor_Application(const std::string& pProjectPath, const std::string& pProjectName);
	
		~Editor_Application();

		void Run();

		bool IsRunning() const;

	private:
		Editor_Context mContext;
		Editor_Editor mEditor;
	};
}

