#include <array>

#include <OglTools/Tools_PathParser.h>

#include <OglUI/UI_TextColored.h>
#include <OglUI/UI_DragSingleScalar.h>
#include <OglUI/UI_DragMultipleScalars.h>
#include <OglUI/UI_InputText.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_ButtonSmall.h>
#include <OglUI/UI_DDTarget.h>

#include "Core_ServiceLocator.h"
#include "Core_ModelManager.h"
#include "Core_TextureManager.h"
#include "Core_ShaderManager.h"
#include "Core_MaterialManager.h"

#include "Core_GUIDrawer.h"

const UI::Color Core::Core_GUIDrawer::TitleColor = { 0.85f, 0.65f, 0.0f, 1.f };
const UI::Color Core::Core_GUIDrawer::ClearButtonColor = { 0.5f, 0.0f, 0.0f, 1.f };
const float Core::Core_GUIDrawer::_MIN_FLOAT = -999999999.f;
const float Core::Core_GUIDrawer::_MAX_FLOAT = +999999999.f;
Render::Render_Texture* Core::Core_GUIDrawer::__EMPTY_TEXTURE = nullptr;

void Core::Core_GUIDrawer::ProvideEmptyTexture(Render::Render_Texture& pEmptyTexture)
{
	__EMPTY_TEXTURE = &pEmptyTexture;
}

void Core::Core_GUIDrawer::CreateTitle(UI::UI_WidgetContainer& pRoot, const std::string& pName)
{
	pRoot.CreateWidget<UI::UI_TextColored>(pName, TitleColor);
}

void Core::Core_GUIDrawer::DrawBoolean(UI::UI_WidgetContainer& pRoot, const std::string& pName, bool& pData)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_CheckBox>();
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<bool>>();
	dispatcher.RegisterReference(reinterpret_cast<bool&>(pData));
}

void Core::Core_GUIDrawer::DrawVec2(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::vec2& pData, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 2>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 2>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 2>&>(pData));
}

void Core::Core_GUIDrawer::DrawVec3(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::vec3& pData, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 3>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 3>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(pData));
}

void Core::Core_GUIDrawer::DrawVec4(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::vec4& pData, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 4>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(pData));
}

void Core::Core_GUIDrawer::DrawQuat(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::quat& pData, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 4>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(pData));
}

void Core::Core_GUIDrawer::DrawString(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::string& pData)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_InputText>("");
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::string>>();
	dispatcher.RegisterReference(pData);
}

void Core::Core_GUIDrawer::DrawColor(UI::UI_WidgetContainer& pRoot, const std::string& pName, UI::Color& p_color, bool p_hasAlpha)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<UI::Color>>();
	dispatcher.RegisterReference(p_color);
}

UI::UI_Text& Core::Core_GUIDrawer::DrawMesh(UI::UI_WidgetContainer& pRoot, const std::string& pName, Render::Render_Model*& pData, Tools::Tools_Event<>* pUpdateNotifier)
{
	CreateTitle(pRoot, pName);

	std::string displayedText = (pData ? pData->path : std::string("Empty"));
	auto& rightSide = pRoot.CreateWidget<UI::UI_Group>();

	auto& widget = rightSide.CreateWidget<UI::UI_Text>(displayedText);

	widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [&widget, &pData, pUpdateNotifier](auto p_receivedData)
		{
			if (Tools::Tools_PathParser::GetFileType(p_receivedData.first) == Tools::Tools_PathParser::EFileType::MODEL)
			{
				if (auto resource = OVSERVICE(Core_ModelManager).GetResource(p_receivedData.first); resource)
				{
					pData = resource;
					widget.mContent = p_receivedData.first;
					if (pUpdateNotifier)
						pUpdateNotifier->Invoke();
				}
			}
		};

	widget.mLineBreak = false;

	auto& resetButton = rightSide.CreateWidget<UI::UI_ButtonSmall>("Clear");
	resetButton.mIdleBackgroundColor = ClearButtonColor;
	resetButton.mClickedEvent += [&widget, &pData, pUpdateNotifier]
		{
			pData = nullptr;
			widget.mContent = "Empty";
			if (pUpdateNotifier)
				pUpdateNotifier->Invoke();
		};

	return widget;
}

UI::UI_Image& Core::Core_GUIDrawer::DrawTexture(UI::UI_WidgetContainer& pRoot, const std::string& pName, Render::Render_Texture*& pData, Tools::Tools_Event<>* pUpdateNotifier)
{
	CreateTitle(pRoot, pName);

	std::string displayedText = (pData ? pData->path : std::string("Empty"));
	auto& rightSide = pRoot.CreateWidget<UI::UI_Group>();

	auto& widget = rightSide.CreateWidget<UI::UI_Image>(pData ? pData->id : (__EMPTY_TEXTURE ? __EMPTY_TEXTURE->id : 0), glm::vec2{ 75, 75 });

	widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [&widget, &pData, pUpdateNotifier](auto p_receivedData)
		{
			if (Tools::Tools_PathParser::GetFileType(p_receivedData.first) == Tools::Tools_PathParser::EFileType::TEXTURE)
			{
				if (auto resource = OVSERVICE(Core_TextureManager).GetResource(p_receivedData.first); resource)
				{
					pData = resource;
					widget.mTextureID.mId = resource->id;
					if (pUpdateNotifier)
						pUpdateNotifier->Invoke();
				}
			}
		};

	widget.mLineBreak = false;

	auto& resetButton = rightSide.CreateWidget<UI::UI_Button>("Clear");
	resetButton.mIdleBackgroundColor = ClearButtonColor;
	resetButton.mClickedEvent += [&widget, &pData, pUpdateNotifier]
		{
			pData = nullptr;
			widget.mTextureID.mId = (__EMPTY_TEXTURE ? __EMPTY_TEXTURE->id : 0);
			if (pUpdateNotifier)
				pUpdateNotifier->Invoke();
		};

	return widget;
}

UI::UI_Text& Core::Core_GUIDrawer::DrawShader(UI::UI_WidgetContainer& pRoot, const std::string& pName, Render::Render_Shader*& pData, Tools::Tools_Event<>* pUpdateNotifier)
{
	CreateTitle(pRoot, pName);

	std::string displayedText = (pData ? pData->path : std::string("Empty"));
	auto& rightSide = pRoot.CreateWidget<UI::UI_Group>();

	auto& widget = rightSide.CreateWidget<UI::UI_Text>(displayedText);

	widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [&widget, &pData, pUpdateNotifier](auto p_receivedData)
		{
			if (Tools::Tools_PathParser::GetFileType(p_receivedData.first) == Tools::Tools_PathParser::EFileType::SHADER)
			{
				if (auto resource = OVSERVICE(Core_ShaderManager).GetResource(p_receivedData.first); resource)
				{
					pData = resource;
					widget.mContent = p_receivedData.first;
					if (pUpdateNotifier)
						pUpdateNotifier->Invoke();
				}
			}
		};

	widget.mLineBreak = false;

	auto& resetButton = rightSide.CreateWidget<UI::UI_ButtonSmall>("Clear");
	resetButton.mIdleBackgroundColor = ClearButtonColor;
	resetButton.mClickedEvent += [&widget, &pData, pUpdateNotifier]
		{
			pData = nullptr;
			widget.mContent = "Empty";
			if (pUpdateNotifier)
				pUpdateNotifier->Invoke();
		};

	return widget;
}

UI::UI_Text& Core::Core_GUIDrawer::DrawMaterial(UI::UI_WidgetContainer& pRoot, const std::string& pName, Core::Core_Material*& pData, Tools::Tools_Event<>* pUpdateNotifier)
{
	CreateTitle(pRoot, pName);

	std::string displayedText = (pData ? pData->path : std::string("Empty"));
	auto& rightSide = pRoot.CreateWidget<UI::UI_Group>();

	auto& widget = rightSide.CreateWidget<UI::UI_Text>(displayedText);

	widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [&widget, &pData, pUpdateNotifier](auto p_receivedData)
		{
			if (Tools::Tools_PathParser::GetFileType(p_receivedData.first) == Tools::Tools_PathParser::EFileType::MATERIAL)
			{
				if (auto resource = OVSERVICE(Core_MaterialManager).GetResource(p_receivedData.first); resource)
				{
					pData = resource;
					widget.mContent = p_receivedData.first;
					if (pUpdateNotifier)
						pUpdateNotifier->Invoke();
				}
			}
		};

	widget.mLineBreak = false;

	auto& resetButton = rightSide.CreateWidget<UI::UI_ButtonSmall>("Clear");
	resetButton.mIdleBackgroundColor = ClearButtonColor;
	resetButton.mClickedEvent += [&widget, &pData, pUpdateNotifier]
		{
			pData = nullptr;
			widget.mContent = "Empty";
			if (pUpdateNotifier)
				pUpdateNotifier->Invoke();
		};

	return widget;
}

UI::UI_Text& Core::Core_GUIDrawer::DrawAsset(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::string& pData, Tools::Tools_Event<>* pUpdateNotifier)
{
	CreateTitle(pRoot, pName);

	const std::string displayedText = (pData.empty() ? std::string("Empty") : pData);
	auto& rightSide = pRoot.CreateWidget<UI::UI_Group>();

	auto& widget = rightSide.CreateWidget<UI::UI_Text>(displayedText);

	widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [&widget, &pData, pUpdateNotifier](auto p_receivedData)
		{
			pData = p_receivedData.first;
			widget.mContent = p_receivedData.first;
			if (pUpdateNotifier)
				pUpdateNotifier->Invoke();
		};

	widget.mLineBreak = false;

	auto& resetButton = rightSide.CreateWidget<UI::UI_ButtonSmall>("Clear");
	resetButton.mIdleBackgroundColor = ClearButtonColor;
	resetButton.mClickedEvent += [&widget, &pData, pUpdateNotifier]
		{
			pData = "";
			widget.mContent = "Empty";
			if (pUpdateNotifier)
				pUpdateNotifier->Invoke();
		};

	return widget;
}

void Core::Core_GUIDrawer::DrawBoolean(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<bool(void)> pGatherer, std::function<void(bool)> pProvider)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_CheckBox>();
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<bool>>();

	dispatcher.RegisterGatherer([pGatherer]()
		{
			bool value = pGatherer();
			return reinterpret_cast<bool&>(value);
		});

	dispatcher.RegisterProvider([pProvider](bool p_value)
		{
			pProvider(reinterpret_cast<bool&>(p_value));
		});
}

void Core::Core_GUIDrawer::DrawVec2(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::vec2(void)> pGatherer, std::function<void(glm::vec2)> pProvider, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 2>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 2>>>();

	dispatcher.RegisterGatherer([pGatherer]()
		{
			glm::vec2 value = pGatherer();
			return reinterpret_cast<const std::array<float, 2>&>(value);
		});

	dispatcher.RegisterProvider([pProvider](std::array<float, 2> p_value)
		{
			pProvider(reinterpret_cast<glm::vec2&>(p_value));
		});
}

void Core::Core_GUIDrawer::DrawVec3(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::vec3(void)> pGatherer, std::function<void(glm::vec3)> pProvider, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 3>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 3>>>();

	dispatcher.RegisterGatherer([pGatherer]()
		{
			glm::vec3 value = pGatherer();
			return reinterpret_cast<const std::array<float, 3>&>(value);
		});

	dispatcher.RegisterProvider([pProvider](std::array<float, 3> p_value)
		{
			pProvider(reinterpret_cast<glm::vec3&>(p_value));
		});
}

void Core::Core_GUIDrawer::DrawVec4(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::vec4(void)> pGatherer, std::function<void(glm::vec4)> pProvider, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 4>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 4>>>();

	dispatcher.RegisterGatherer([pGatherer]()
		{
			glm::vec4 value = pGatherer();
			return reinterpret_cast<const std::array<float, 4>&>(value);
		});

	dispatcher.RegisterProvider([pProvider](std::array<float, 4> p_value)
		{
			pProvider(reinterpret_cast<glm::vec4&>(p_value));
		});
}

void Core::Core_GUIDrawer::DrawQuat(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::quat(void)> pGatherer, std::function<void(glm::quat)> pProvider, float pStep, float pMin, float pMax)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 4>>(GetDataType<float>(), pMin, pMax, 0.f, pStep, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 4>>>();

	dispatcher.RegisterGatherer([pGatherer]()
		{
			glm::quat value = pGatherer();
			return reinterpret_cast<const std::array<float, 4>&>(value);
		});

	dispatcher.RegisterProvider([&dispatcher, pProvider](std::array<float, 4> p_value)
		{
			pProvider(glm::normalize(reinterpret_cast<glm::quat&>(p_value)));
		});
}

void Core::Core_GUIDrawer::DrawDDString(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<std::string(void)> pGatherer, std::function<void(std::string)> pProvider, const std::string& pIdentifier)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_InputText>("");
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(pGatherer);
	dispatcher.RegisterProvider(pProvider);

	auto& ddTarget = widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>(pIdentifier);
	ddTarget.mDataReceivedEvent += [&widget, &dispatcher](std::pair<std::string, UI::UI_Group*> pData)
		{
			widget.mContent = pData.first;
			dispatcher.NotifyChange();
		};
}

void Core::Core_GUIDrawer::DrawString(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<std::string(void)> pGatherer, std::function<void(std::string)> pProvider)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_InputText>("");
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(pGatherer);
	dispatcher.RegisterProvider(pProvider);
}

void Core::Core_GUIDrawer::DrawColor(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<UI::Color(void)> pGatherer, std::function<void(UI::Color)> pProvider, bool pHasAlpha)
{
	CreateTitle(pRoot, pName);
	auto& widget = pRoot.CreateWidget<UI::UI_ColorEdit>(pHasAlpha);
	auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<UI::Color>>();
	dispatcher.RegisterGatherer(pGatherer);
	dispatcher.RegisterProvider(pProvider);
}
