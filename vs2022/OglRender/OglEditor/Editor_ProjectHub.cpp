
#include <fstream>
#include <filesystem>

#include <OglUI/UI_Text.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_Spacing.h>
#include <OglUI/UI_InputText.h>
#include <OglUI/UI_Separator.h>

#include <OglTools/Tools_PathParser.h>

#include <OglWindow/Window_SaveFileDialog.h>
#include <OglWindow/Window_OpenFileDialog.h>
#include <OglWindow/Window_MessageBox.h>

#include "Editor_ProjectHub.h"

#define PROJECTS_FILE std::string(std::string(getenv("APPDATA")) + "")

Editor::Editor_ProjectHub::Editor_ProjectHub()
{
}

std::tuple<bool, std::string, std::string> Editor::Editor_ProjectHub::Run()
{
	return std::tuple<bool, std::string, std::string>();
}

void Editor::Editor_ProjectHub::SetupContext()
{
}

void Editor::Editor_ProjectHub::RegisterProject(const std::string& p_path)
{
}
