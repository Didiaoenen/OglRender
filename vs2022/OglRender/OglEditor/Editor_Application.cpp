#include <OglTools/Tools_Clock.h>

#include "Editor_Application.h"

Editor::Editor_Application::Editor_Application(const std::string& pProjectPath, const std::string& pProjectName) :
	m_context(pProjectPath, pProjectName),
	m_editor(m_context)
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
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool Editor::Editor_Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
