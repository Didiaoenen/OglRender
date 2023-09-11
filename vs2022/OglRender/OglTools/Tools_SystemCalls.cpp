
#include <Windows.h>

#include "Tools_PathParser.h"
#include "Tools_SystemCalls.h"

void Tools::Tools_SystemCalls::ShowInExplorer(const std::string& pPath)
{
	ShellExecuteA(NULL, "open", Tools_PathParser::MakeWindowsStyle(pPath).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void Tools::Tools_SystemCalls::OpenFile(const std::string& pFile, const std::string& pWorkingDir)
{
	ShellExecuteA(NULL, "open", Tools_PathParser::MakeWindowsStyle(pFile).c_str(), NULL,
		pWorkingDir.empty() ? NULL : Tools_PathParser::MakeWindowsStyle(pWorkingDir).c_str(), SW_SHOWNORMAL);
}

void Tools::Tools_SystemCalls::EditFile(const std::string& pFile)
{
	ShellExecuteW(NULL, NULL, std::wstring(pFile.begin(), pFile.end()).c_str(), NULL, NULL, SW_NORMAL);
}

void Tools::Tools_SystemCalls::OpenURL(const std::string& pUrl)
{
	ShellExecute(0, 0, pUrl.c_str(), 0, 0, SW_SHOW);
}
