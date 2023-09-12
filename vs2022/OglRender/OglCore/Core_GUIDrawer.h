#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <OglUI/UI_WidgetContainer.h>
#include <OglUI/UI_DragSingleScalar.h>
#include <OglUI/UI_DragMultipleScalars.h>
#include <OglUI/UI_InputText.h>
#include <OglUI/UI_Image.h>
#include <OglUI/UI_Color.h>
#include <OglUI/UI_Text.h>

namespace Render
{
	class Render_Model;
	class Render_Shader;
	class Render_Texture;
}

namespace Core
{
	class Core_Material;

	class Core_GUIDrawer
	{
	public:
		static const UI::Color TitleColor;
		static const UI::Color ClearButtonColor;

		static const float _MIN_FLOAT;
		static const float _MAX_FLOAT;

		static void ProvideEmptyTexture(Render::Render_Texture& pEmptyTexture);

		static void CreateTitle(UI::UI_WidgetContainer& pRoot, const std::string& pName);

		template <typename T>
		static void DrawScalar(UI::UI_WidgetContainer& pRoot, const std::string& pName, T& pData, float pStep = 1.f, T pMin = std::numeric_limits<T>::min(), T pMax = std::numeric_limits<T>::max());
		static void DrawBoolean(UI::UI_WidgetContainer& pRoot, const std::string& pName, bool& pData);
		static void DrawVec2(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::vec2& pData, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawVec3(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::vec3& pData, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawVec4(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::vec4& pData, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawQuat(UI::UI_WidgetContainer& pRoot, const std::string& pName, glm::quat& pData, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawString(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::string& pData);
		static void DrawColor(UI::UI_WidgetContainer& pRoot, const std::string& pName, UI::Color& pColor, bool p_hasAlpha = false);
		static UI::UI_Text& DrawMesh(UI::UI_WidgetContainer& pRoot, const std::string& pName, Render::Render_Model*& pData, Tools::Tools_Event<>* pUpdateNotifier = nullptr);
		static UI::UI_Image& DrawTexture(UI::UI_WidgetContainer& pRoot, const std::string& pName, Render::Render_Texture*& pData, Tools::Tools_Event<>* pUpdateNotifier = nullptr);
		static UI::UI_Text& DrawShader(UI::UI_WidgetContainer& pRoot, const std::string& pName, Render::Render_Shader*& pData, Tools::Tools_Event<>* pUpdateNotifier = nullptr);
		static UI::UI_Text& DrawMaterial(UI::UI_WidgetContainer& pRoot, const std::string& pName, Core::Core_Material*& pData, Tools::Tools_Event<>* pUpdateNotifier = nullptr);
		static UI::UI_Text& DrawAsset(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::string& pData, Tools::Tools_Event<>* pUpdateNotifier = nullptr);

		template <typename T>
		static void DrawScalar(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<T(void)> pGatherer, std::function<void(T)> pProvider, float pStep = 1.f, T pMin = std::numeric_limits<T>::min(), T pMax = std::numeric_limits<T>::max());
		static void DrawBoolean(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<bool(void)> pGatherer, std::function<void(bool)> pProvider);
		static void DrawVec2(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::vec2(void)> pGatherer, std::function<void(glm::vec2)> pProvider, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawVec3(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::vec3(void)> pGatherer, std::function<void(glm::vec3)> pProvider, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawVec4(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::vec4(void)> pGatherer, std::function<void(glm::vec4)> pProvider, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawQuat(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<glm::quat(void)> pGatherer, std::function<void(glm::quat)> pProvider, float pStep = 1.f, float pMin = _MIN_FLOAT, float pMax = _MAX_FLOAT);
		static void DrawDDString(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<std::string(void)> pGatherer, std::function<void(std::string)> pProvider, const std::string& pIdentifier);
		static void DrawString(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<std::string(void)> pGatherer, std::function<void(std::string)> pProvider);
		static void DrawColor(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<UI::Color(void)> pGatherer, std::function<void(UI::Color)> pProvider, bool pHasAlpha = false);

		template <typename T>
		static ImGuiDataType_ GetDataType();

		template <typename T>
		static std::string GetFormat();

	private:
		static Render::Render_Texture* __EMPTY_TEXTURE;
	};
}

#include "Core_GUIDrawer.inl"