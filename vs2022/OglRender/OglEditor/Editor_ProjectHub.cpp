
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
	Editor_ProjectHubPanel(bool& pReadyToGo, std::string& pPath, std::string& pProjectName) :
		UI::UI_PanelWindow("Overload - Project Hub", true),
		mReadyToGo(pReadyToGo),
		mPath(pPath),
		mProjectName(pProjectName)
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
		mGoButton = &CreateWidget<UI::UI_Button>("GO");

		pathField.mContentChangedEvent += [this, &pathField](std::string pContent)
			{
				pathField.mContent = Tools::Tools_PathParser::MakeWindowsStyle(pContent);

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

		mGoButton->mClickedEvent += [this, &pathField]
			{
				CreateProject(pathField.mContent);
				RegisterProject(pathField.mContent);
				OpenProject(pathField.mContent);
			};

		openProjectButton.mLineBreak = false;
		newProjectButton.mLineBreak = false;
		pathField.mLineBreak = false;

		for (uint8_t i = 0; i < 4; ++i)
		{
			CreateWidget<UI::UI_Spacing>();
		}

		CreateWidget<UI::UI_Separator>();

		for (uint8_t i = 0; i < 4; ++i)
		{
			CreateWidget<UI::UI_Spacing>();
		}

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

	void UpdateGoButton(const std::string& pPath)
	{
		bool validPath = pPath != "";
		mGoButton->mIdleBackgroundColor = validPath ? UI::Color{ 0.f, 0.5f, 0.0f, 1.f } : UI::Color{ 0.1f, 0.1f, 0.1f, 1.f };
		mGoButton->mDisabled = !validPath;
	}

	void CreateProject(const std::string& pPath)
	{
		if (!std::filesystem::exists(pPath))
		{
			std::filesystem::create_directory(pPath);
			std::filesystem::create_directory(pPath + "Assets\\");
			std::filesystem::create_directory(pPath + "Scripts\\");
			std::ofstream projectFile(pPath + '\\' + Tools::Tools_PathParser::GetElementName(std::string(pPath.data(), pPath.data() + pPath.size() - 1)) + ".ovproject");
		}
	}

	void RegisterProject(const std::string& pPath)
	{
		bool pathAlreadyRegistered = false;

		{
			std::string line;
			std::ifstream myfile(PROJECTS_FILE);
			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					if (line == pPath)
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
			projectsFile << pPath << std::endl;
		}
	}

	void OpenProject(const std::string& pPath)
	{
		mReadyToGo = std::filesystem::exists(pPath);
		if (!mReadyToGo)
		{
			Window::Window_MessageBox errorMessage("Project not found", "The selected project does not exists", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK);
		}
		else
		{
			mPath = pPath;
			mProjectName = Tools::Tools_PathParser::GetElementName(mPath);
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
	bool& mReadyToGo;
	std::string& mPath;
	std::string& mProjectName;
	UI::UI_Button* mGoButton{ nullptr };
};

Editor::Editor_ProjectHub::Editor_ProjectHub()
{
	SetupContext();
	mMainPanel = std::make_unique<Editor_ProjectHubPanel>(mReadyToGo, mProjectPath, mProjectName);

	mUIManager->SetCanvas(mCanvas);
	mCanvas.AddPanel(*mMainPanel);
}

std::tuple<bool, std::string, std::string> Editor::Editor_ProjectHub::Run()
{
	mRenderer->SetClearColor(0.f, 0.f, 0.f, 1.f);

	while (!mWindow->ShouldClose())
	{
		mRenderer->Clear();
		mDevice->PollEvents();
		mUIManager->Render();
		mWindow->SwapBuffers();

		if (!mMainPanel->IsOpened())
		{
			mWindow->SetShouldClose(true);
		}
	}

	return { mReadyToGo, mProjectPath, mProjectName };
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

	mDevice = std::make_unique<Window::Window_Device>(deviceSettings);
	mWindow = std::make_unique<Window::Window_Window>(*mDevice, windowSettings);
	mWindow->MakeCurrentContext();

	auto [monWidth, monHeight] = mDevice->GetMonitorSize();
	auto [winWidth, winHeight] = mWindow->GetSize();
	mWindow->SetPosition(monWidth / 2 - winWidth / 2, monHeight / 2 - winHeight / 2);

	mDriver = std::make_unique<Render::Render_Driver>(Render::Render_DriverSettings{ false });
	mRenderer = std::make_unique<Render::Render_Renderer>(*mDriver);
	mRenderer->SetCapability(Render::ERenderingCapability::MULTISAMPLE, true);

	mUIManager = std::make_unique<UI::UI_UIManager>(mWindow->GetGlfwWindow(), UI::EStyle::UNITY_DARK);
	mUIManager->LoadFont("Ruda_Big", "Data\\Editor\\Fonts\\Ruda-Bold.ttf", 18);
	mUIManager->UseFont("Ruda_Big");
	mUIManager->EnableEditorLayoutSave(false);
	mUIManager->EnableDocking(false);
}

void Editor::Editor_ProjectHub::RegisterProject(const std::string& pPath)
{
	static_cast<Editor_ProjectHubPanel*>(mMainPanel.get())->RegisterProject(pPath);
}
