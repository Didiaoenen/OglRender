
#include <filesystem>

#include <OglTools/Tools_String.h>

#include <OglRenderer/Render_Defines.h>

#include "Editor_ProjectHub.h"
#include "Editor_Application.h"

#undef APIENTRY
#include "Windows.h"

FORCE_DEDICATED_GPU

void UpdateWorkingDirectory(const std::string& p_executablePath)
{
	if (!IsDebuggerPresent())
	{
		std::filesystem::current_path(Tools::Tools_PathParser::GetContainingFolder(p_executablePath));
	}
}

int main(int argc, char** argv);
static void TryRun(const std::string& projectPath, const std::string& projectName);

#ifndef _DEBUG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	main(__argc, __argv);
}
#endif

int main(int argc, char** argv)
{
	UpdateWorkingDirectory(argv[0]);

	bool ready = false;
	std::string projectPath;
	std::string projectName;

	{
		Editor::Editor_ProjectHub hub;

		if (argc < 2)
		{
			std::tie(ready, projectPath, projectName) = hub.Run();
		}
		else
		{
			std::string projectFile = argv[1];

			if (Tools::Tools_PathParser::GetExtension(projectFile) == "ovproject")
			{
				ready = true;
				projectPath = Tools::Tools_PathParser::GetContainingFolder(projectFile);
				projectName = Tools::Tools_PathParser::GetElementName(projectFile);
				Tools::Tools_String::Replace(projectName, ".ovproject", "");
			}

			hub.RegisterProject(projectPath);
		}
	}

	if (ready)
	{
		TryRun(projectPath, projectName);
	}

	return EXIT_SUCCESS;
}

static void TryRun(const std::string& projectPath, const std::string& projectName)
{
	auto errorEvent = [](Window::EDeviceError, std::string errMsg)
		{
			errMsg = "Overload requires OpenGL 4.3 or newer.\r\n" + errMsg;
			MessageBox(0, errMsg.c_str(), "Overload", MB_OK | MB_ICONSTOP);
		};

	std::unique_ptr<Editor::Editor_Application> app;

	try
	{
		auto listenerId = Window::Window_Device::mErrorEvent += errorEvent;
		app = std::make_unique<Editor::Editor_Application>(projectPath, projectName);
		Window::Window_Device::mErrorEvent -= listenerId;
	}
	catch (...) {}

	if (app)
	{
		app->Run();
	}
}