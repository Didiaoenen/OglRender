#pragma once

#include <string>

namespace Editor
{
	class EditorApplication
	{
	public:
		EditorApplication(const std::string& pProjectPath, const std::string& pProjectName);
	
		~EditorApplication();

		void Run();

		bool IsRunning() const;

	private:

	};
}

