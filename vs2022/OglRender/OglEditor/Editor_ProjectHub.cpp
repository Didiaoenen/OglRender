
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

class Editor_ProjectHubPanel : public UI::UI_PanelWindow
{
public:
	Editor_ProjectHubPanel(bool& p_readyToGo, std::string& p_path, std::string& p_projectName) :
		UI::UI_PanelWindow("Overload - Project Hub", true),
		m_readyToGo(p_readyToGo),
		m_path(p_path),
		m_projectName(p_projectName)
	{
		mResizable = false;
		mMovable = false;
		mTitleBar = false;

		std::filesystem::create_directories(std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\");

		SetSize({ 1000, 580 });
		SetPosition({ 0.f, 0.f });

		auto& openProjectButton = CreateWidget<UI::UI_Button>("Open Project");
		auto& newProjectButton = CreateWidget<UI::UI_Button>("New Project");
		auto& pathField = CreateWidget<UI::UI_InputText>("");
		m_goButton = &CreateWidget<UI::UI_Button>("GO");

		pathField.mContentChangedEvent += [this, &pathField](std::string p_content)
			{
				pathField.mContent = Tools::Tools_PathParser::MakeWindowsStyle(p_content);

				if (pathField.mContent != "" && pathField.mContent.back() != '\\')
				{
					pathField.mContent += '\\';
				}

				UpdateGoButton(pathField.mContent);
			};

		UpdateGoButton("");

		openProjectButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };
		newProjectButton.mIdleBackgroundColor = { 0.f, 0.5f, 0.0f, 1.f };

		openProjectButton.mClickedEvent += [this]
			{
				Window::Window_OpenFileDialog dialog("Open project");
				dialog.AddFileType("Overload Project", "*.ovproject");
				dialog.Show();

				std::string ovProjectPath = dialog.GetSelectedFilePath();
				std::string rootFolderPath = Tools::Tools_PathParser::GetContainingFolder(ovProjectPath);

				if (dialog.HasSucceeded())
				{
					RegisterProject(rootFolderPath);
					OpenProject(rootFolderPath);
				}
			};

		newProjectButton.mClickedEvent += [this, &pathField]
			{
				Window::Window_SaveFileDialog dialog("New project location");
				dialog.DefineExtension("Overload Project", "..");
				dialog.Show();
				if (dialog.HasSucceeded())
				{
					std::string result = dialog.GetSelectedFilePath();
					pathField.mContent = std::string(result.data(), result.data() + result.size() - std::string("..").size());
					pathField.mContent += "\\";
					UpdateGoButton(pathField.mContent);
				}
			};

		m_goButton->mClickedEvent += [this, &pathField]
			{
				CreateProject(pathField.mContent);
				RegisterProject(pathField.mContent);
				OpenProject(pathField.mContent);
			};

		openProjectButton.mLineBreak = false;
		newProjectButton.mLineBreak = false;
		pathField.mLineBreak = false;

		for (uint8_t i = 0; i < 4; ++i)
			CreateWidget<UI::UI_Spacing>();

		CreateWidget<UI::UI_Separator>();

		for (uint8_t i = 0; i < 4; ++i)
			CreateWidget<UI::UI_Spacing>();

		auto& columns = CreateWidget<UI::UI_Columns<2>>();

		columns.mWidths = { 750, 500 };

		std::string line;
		std::ifstream myfile(PROJECTS_FILE);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				if (std::filesystem::exists(line))
				{
					auto& text = columns.CreateWidget<UI::UI_Text>(line);
					auto& actions = columns.CreateWidget<UI::UI_Group>();
					auto& openButton = actions.CreateWidget<UI::UI_Button>("Open");
					auto& deleteButton = actions.CreateWidget<UI::UI_Button>("Delete");

					openButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };
					deleteButton.mIdleBackgroundColor = { 0.5f, 0.f, 0.f, 1.f };

					openButton.mClickedEvent += [this, line]
						{
							OpenProject(line);
						};

					std::string toErase = line;
					deleteButton.mClickedEvent += [this, &text, &actions, toErase]
						{
							text.Destroy();
							actions.Destroy();

							std::string line;
							std::ifstream fin(PROJECTS_FILE);
							std::ofstream temp("temp");

							while (getline(fin, line))
							{
								if (line != toErase)
								{
									temp << line << std::endl;
								}
							}


							temp.close();
							fin.close();

							std::filesystem::remove(PROJECTS_FILE);
							std::filesystem::rename("temp", PROJECTS_FILE);
						};

					openButton.mLineBreak = false;
					deleteButton.mLineBreak;
				}
			}
			myfile.close();
		}
	}

	void UpdateGoButton(const std::string& p_path)
	{
		bool validPath = p_path != "";
		m_goButton->mIdleBackgroundColor = validPath ? UI::Color{ 0.f, 0.5f, 0.0f, 1.f } : UI::Color{ 0.1f, 0.1f, 0.1f, 1.f };
		m_goButton->mDisabled = !validPath;
	}

	void CreateProject(const std::string& p_path)
	{
		if (!std::filesystem::exists(p_path))
		{
			std::filesystem::create_directory(p_path);
			std::filesystem::create_directory(p_path + "Assets\\");
			std::filesystem::create_directory(p_path + "Scripts\\");
			std::ofstream projectFile(p_path + '\\' + Tools::Tools_PathParser::GetElementName(std::string(p_path.data(), p_path.data() + p_path.size() - 1)) + ".ovproject");
		}
	}

	void RegisterProject(const std::string& p_path)
	{
		bool pathAlreadyRegistered = false;

		{
			std::string line;
			std::ifstream myfile(PROJECTS_FILE);
			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					if (line == p_path)
					{
						pathAlreadyRegistered = true;
						break;
					}
				}
				myfile.close();
			}
		}

		if (!pathAlreadyRegistered)
		{
			std::ofstream projectsFile(PROJECTS_FILE, std::ios::app);
			projectsFile << p_path << std::endl;
		}
	}

	void OpenProject(const std::string& p_path)
	{
		m_readyToGo = std::filesystem::exists(p_path);
		if (!m_readyToGo)
		{
			Window::Window_MessageBox errorMessage("Project not found", "The selected project does not exists", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK);
		}
		else
		{
			m_path = p_path;
			m_projectName = Tools::Tools_PathParser::GetElementName(m_path);
			Close();
		}
	}

	void Draw() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 50.f, 50.f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

		UI::UI_PanelWindow::Draw();

		ImGui::PopStyleVar(2);
	}

private:
	bool& m_readyToGo;
	std::string& m_path;
	std::string& m_projectName;
	UI::UI_Button* m_goButton = nullptr;
};

Editor::Editor_ProjectHub::Editor_ProjectHub()
{
	SetupContext();
	m_mainPanel = std::make_unique<Editor_ProjectHubPanel>(m_readyToGo, m_projectPath, m_projectName);

	m_uiManager->SetCanvas(m_canvas);
	m_canvas.AddPanel(*m_mainPanel);
}

std::tuple<bool, std::string, std::string> Editor::Editor_ProjectHub::Run()
{
	m_renderer->SetClearColor(0.f, 0.f, 0.f, 1.f);

	while (!m_window->ShouldClose())
	{
		m_renderer->Clear();
		m_device->PollEvents();
		m_uiManager->Render();
		m_window->SwapBuffers();

		if (!m_mainPanel->IsOpened())
		{
			m_window->SetShouldClose(true);
		}
	}

	return { m_readyToGo, m_projectPath, m_projectName };
}

void Editor::Editor_ProjectHub::SetupContext()
{
	Window::Window_DeviceSettings deviceSettings;
	Window::Window_WindowSettings windowSettings;
	windowSettings.title = "Overload - Project Hub";
	windowSettings.width = 1000;
	windowSettings.height = 580;
	windowSettings.maximized = false;
	windowSettings.resizable = false;
	windowSettings.decorated = true;

	m_device = std::make_unique<Window::Window_Device>(deviceSettings);
	m_window = std::make_unique<Window::Window_Window>(*m_device, windowSettings);
	m_window->MakeCurrentContext();

	auto [monWidth, monHeight] = m_device->GetMonitorSize();
	auto [winWidth, winHeight] = m_window->GetSize();
	m_window->SetPosition(monWidth / 2 - winWidth / 2, monHeight / 2 - winHeight / 2);

	m_driver = std::make_unique<Render::Render_Driver>(Render::Render_DriverSettings{ false });
	m_renderer = std::make_unique<Render::Render_Renderer>(*m_driver);
	m_renderer->SetCapability(Render::ERenderingCapability::MULTISAMPLE, true);

	m_uiManager = std::make_unique<UI::UI_UIManager>(m_window->GetGlfwWindow(),UI::EStyle::ALTERNATIVE_DARK);
	m_uiManager->LoadFont("Ruda_Big", "Data\\Editor\\Fonts\\Ruda-Bold.ttf", 18);
	m_uiManager->UseFont("Ruda_Big");
	m_uiManager->EnableEditorLayoutSave(false);
	m_uiManager->EnableDocking(false);
}

void Editor::Editor_ProjectHub::RegisterProject(const std::string& pPath)
{
	static_cast<Editor_ProjectHubPanel*>(m_mainPanel.get())->RegisterProject(pPath);
}
