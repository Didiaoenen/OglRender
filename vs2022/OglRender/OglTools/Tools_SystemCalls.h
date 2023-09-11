#pragma once

#include <string>

namespace Tools
{
	class Tools_SystemCalls
	{
	public:
		Tools_SystemCalls() = delete;

		static void ShowInExplorer(const std::string& pPath);

		static void OpenFile(const std::string& pFile, const std::string& pWorkingDir = "");

		static void EditFile(const std::string& pFile);

		static void OpenURL(const std::string& pUrl);
	};
}

