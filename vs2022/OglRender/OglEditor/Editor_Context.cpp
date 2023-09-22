#include <filesystem>

#include <OglRenderer/Render_Light.h>
#include <OglCore/Core_ServiceLocator.h>

#include "Editor_Context.h"

Editor::Editor_Context::Editor_Context(const std::string& pProjectPath, const std::string& pProjectName) :
	mProjectPath(pProjectPath),
	mProjectName(pProjectName),
	mProjectFilePath(pProjectPath + pProjectName + ".ovproject"),
	mEngineAssetsPath(std::filesystem::canonical("Data\\Engine").string() + "\\"),
	mProjectAssetsPath(pProjectPath + "Assets\\"),
	mProjectScriptsPath(pProjectPath + "Scripts\\"),
	mEditorAssetsPath("Data\\Editor\\"),
	mSceneManager(mProjectAssetsPath),
	mProjectSettings(mProjectFilePath)
{
	if (!IsProjectSettingsIntegrityVerified())
	{
		ResetProjectSettings();
		mProjectSettings.Rewrite();
	}

	Core::Core_ModelManager::ProvideAssetPaths(mProjectAssetsPath, mEngineAssetsPath);
	Core::Core_TextureManager::ProvideAssetPaths(mProjectAssetsPath, mEngineAssetsPath);
	Core::Core_ShaderManager::ProvideAssetPaths(mProjectAssetsPath, mEngineAssetsPath);
	Core::Core_MaterialManager::ProvideAssetPaths(mProjectAssetsPath, mEngineAssetsPath);

	Window::Window_DeviceSettings deviceSettings;
	deviceSettings.mContextMajorVersion = 4;
	deviceSettings.mContextMinorVersion = 3;
	mWindowSettings.title = "Overload Editor";
	mWindowSettings.width = 1280;
	mWindowSettings.height = 720;
	mWindowSettings.maximized = true;

	mDevice = std::make_unique<Window::Window_Device>(deviceSettings);
	mWindow = std::make_unique<Window::Window_Window>(*mDevice, mWindowSettings);
	std::vector<uint64_t> iconRaw = { 0,0,144115188614240000,7500771567664627712,7860776967494637312,0,0,0,0,7212820467466371072,11247766461832697600,14274185407633888512,12905091124788992000,5626708973701824512,514575842263176960,0,0,6564302121125019648,18381468271671515136,18381468271654737920,18237353083595659264,18165295488836311040,6708138037527189504,0,4186681893338480640,7932834557741046016,17876782538917681152,11319824055216379904,15210934132358518784,18381468271520454400,1085667680982603520,0,18093237891929479168,18309410677600032768,11391881649237530624,7932834561381570304,17300321784231761408,15210934132375296000,8293405106311272448,2961143145139082752,16507969723533236736,17516777143216379904,10671305705855129600,7356091234422036224,16580027318695106560,2240567205413984000,18381468271470188544,10959253511276599296,4330520004484136960,10815138323200743424,11607771853338181632,8364614976649238272,17444719546862998784,2669156352,18381468269893064448,6419342512197474304,11103650170688640000,6492244531366860800,14346241902646925312,13841557270159628032,7428148827772098304,3464698581331941120,18381468268953606144,1645680384,18381468271554008832,7140201027266418688,5987558797656659712,17588834734687262208,7284033640602212096,14273902834169157632,18381468269087692288,6852253225049397248,17732667349600245504,16291515470083266560,10022503688432981760,11968059825861367552,9733991836700645376,14850363587428816640,18381468271168132864,16147400282007410688,656430432014827520,18381468270950094848,15715054717226194944,72057596690306560,11823944635485519872,15859169905251653376,17084149004500473856,8581352906816952064,2527949855582584832,18381468271419856896,8581352907253225472,252776704,1376441223417430016,14994761349590357760,10527190521537370112,0,9806614576878321664,18381468271671515136,17156206598538401792,6059619689256392448,10166619973990488064,18381468271403079424,17444719549178451968,420746240,870625192710242304,4906133035823863552,18381468269289150464,18381468271671515136,18381468271671515136,9950729769032620032,14778305994951169792,269422336,0,0,18381468268785833984,8941923452686178304,18381468270950094848,3440842496,1233456333565402880,0,0,0,11823944636091210240,2383877888,16724143605745719296,2316834816,0,0 };
	mWindow->SetIconFromMemory(reinterpret_cast<uint8_t*>(iconRaw.data()), 16, 16);
	mInputManager = std::make_unique<Window::Window_InputManager>(*mWindow);
	mWindow->MakeCurrentContext();

	mDevice->SetVsync(true);

	mDriver = std::make_unique<Render::Render_Driver>(Render::Render_DriverSettings{ true });
	mRenderer = std::make_unique<Core::Core_Renderer>(*mDriver);
	mRenderer->SetCapability(Render::ERenderingCapability::MULTISAMPLE, true);
	mShapeDrawer = std::make_unique<Render::Render_ShapeDrawer>(*mRenderer);

	std::filesystem::create_directories(std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\");

	mUIManager = std::make_unique<UI::UI_UIManager>(mWindow->GetGlfwWindow(), UI::EStyle::UNITY_DARK);
	mUIManager->LoadFont("Ruda_Big", mEditorAssetsPath + "\\Fonts\\jetbrains-mono\\JetBrainsMono-Bold.ttf", 16);
	mUIManager->LoadFont("Ruda_Small", mEditorAssetsPath + "\\Fonts\\jetbrains-mono\\JetBrainsMono-Bold.ttf", 12);
	mUIManager->LoadFont("Ruda_Medium", mEditorAssetsPath + "\\Fonts\\jetbrains-mono\\JetBrainsMono-Bold.ttf", 14);
	mUIManager->UseFont("Ruda_Medium");
	mUIManager->SetEditorLayoutSaveFilename(std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\layout.ini");
	mUIManager->SetEditorLayoutAutosaveFrequency(60.0f);
	mUIManager->EnableEditorLayoutSave(true);
	mUIManager->EnableDocking(true);

	if (!std::filesystem::exists(std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\layout.ini"))
	{
		mUIManager->ResetLayout("Config\\layout.ini");
	}

	mEditorResources = std::make_unique<Editor::Editor_EditorResources>(mEditorAssetsPath);

	Core::Core_ServiceLocator::Provide<Core::Core_ModelManager>(mModelManager);
	Core::Core_ServiceLocator::Provide<Core::Core_TextureManager>(mTextureManager);
	Core::Core_ServiceLocator::Provide<Core::Core_ShaderManager>(mShaderManager);
	Core::Core_ServiceLocator::Provide<Core::Core_MaterialManager>(mMaterialManager);
	Core::Core_ServiceLocator::Provide<Window::Window_InputManager>(*mInputManager);
	Core::Core_ServiceLocator::Provide<Window::Window_Window>(*mWindow);
	Core::Core_ServiceLocator::Provide<Core::Core_SceneManager>(mSceneManager);

	mEngineUBO = std::make_unique<Render::Render_UniformBuffer>
		(
			sizeof(glm::mat4) +
			sizeof(glm::mat4) +
			sizeof(glm::mat4) +
			sizeof(glm::vec3) +
			sizeof(float) +
			sizeof(glm::mat4),
			0, 0,
			Render::EAccessSpecifier::STREAM_DRAW
		);

	mLightSSBO = std::make_unique<Render::Render_ShaderStorageBuffer>(Render::EAccessSpecifier::STREAM_DRAW);
	mSimulatedLightSSBO = std::make_unique<Render::Render_ShaderStorageBuffer>(Render::EAccessSpecifier::STREAM_DRAW);

	std::vector<glm::mat4> simulatedLights;

	Maths::Maths_FTransform simulatedLightTransform;
	simulatedLightTransform.SetLocalRotation(glm::quat({ 45.f, 180.f, 10.f }));

	Render::Render_Light simulatedDirectionalLight(simulatedLightTransform, Render::Render_Light::ELightType::DIRECTIONAL);
	simulatedDirectionalLight.mColor = { 1.f, 1.f, 1.f };
	simulatedDirectionalLight.mIntensity = 1.f;

	Render::Render_Light simulatedAmbientLight(simulatedLightTransform, Render::Render_Light::ELightType::AMBIENT_SPHERE);
	simulatedAmbientLight.mColor = { 0.07f, 0.07f, 0.07f };
	simulatedAmbientLight.mIntensity = 1.f;
	simulatedAmbientLight.mConstant = 1000.0f;

	simulatedLights.push_back(simulatedDirectionalLight.GenerateMatrix());
	simulatedLights.push_back(simulatedAmbientLight.GenerateMatrix());

	mSimulatedLightSSBO->SendBlocks<glm::mat4>(simulatedLights.data(), simulatedLights.size() * sizeof(glm::mat4));

	ApplyProjectSettings();
}

Editor::Editor_Context::~Editor_Context()
{
	mModelManager.UnloadResources();
	mTextureManager.UnloadResources();
	mShaderManager.UnloadResources();
	mMaterialManager.UnloadResources();
}

void Editor::Editor_Context::ResetProjectSettings()
{
	mProjectSettings.RemoveAll();
	mProjectSettings.Add<float>("gravity", -9.81f);
	mProjectSettings.Add<int>("x_resolution", 1280);
	mProjectSettings.Add<int>("y_resolution", 720);
	mProjectSettings.Add<bool>("fullscreen", false);
	mProjectSettings.Add<std::string>("executable_name", "Game");
	mProjectSettings.Add<std::string>("start_scene", "Scene.ovscene");
	mProjectSettings.Add<bool>("vsync", true);
	mProjectSettings.Add<bool>("multisampling", true);
	mProjectSettings.Add<int>("samples", 4);
	mProjectSettings.Add<int>("opengl_major", 4);
	mProjectSettings.Add<int>("opengl_minor", 3);
	mProjectSettings.Add<bool>("dev_build", true);
}

bool Editor::Editor_Context::IsProjectSettingsIntegrityVerified()
{
	return
		mProjectSettings.IsKeyExisting("gravity") &&
		mProjectSettings.IsKeyExisting("x_resolution") &&
		mProjectSettings.IsKeyExisting("y_resolution") &&
		mProjectSettings.IsKeyExisting("fullscreen") &&
		mProjectSettings.IsKeyExisting("executable_name") &&
		mProjectSettings.IsKeyExisting("start_scene") &&
		mProjectSettings.IsKeyExisting("vsync") &&
		mProjectSettings.IsKeyExisting("multisampling") &&
		mProjectSettings.IsKeyExisting("samples") &&
		mProjectSettings.IsKeyExisting("opengl_major") &&
		mProjectSettings.IsKeyExisting("opengl_minor") &&
		mProjectSettings.IsKeyExisting("dev_build");
}

void Editor::Editor_Context::ApplyProjectSettings()
{
}
