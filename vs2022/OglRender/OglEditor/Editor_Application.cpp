#include <OglTools/Tools_Clock.h>

#include "Editor_Application.h"

Editor::Editor_Application::Editor_Application(const std::string& pProjectPath, const std::string& pProjectName) :
	mContext(pProjectPath, pProjectName),
	mEditor(mContext)
{
}

Editor::Editor_Application::~Editor_Application()
{
}

void Editor::Editor_Application::Run()
{
	Tools::Tools_Clock clock;

	while (IsRunning())
	{
		mEditor.PreUpdate();
		mEditor.Update(clock.GetDeltaTime());
		mEditor.PostUpdate();

		clock.Update();
	}
}

bool Editor::Editor_Application::IsRunning() const
{
	return !mContext.mWindow->ShouldClose();
}
