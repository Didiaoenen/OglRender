#pragma once

#include "Core_GUIDrawer.h"

namespace Core
{
	template<typename T>
	inline ImGuiDataType_ Core_GUIDrawer::GetDataType()
	{
		if constexpr (std::is_same<T, float>::value)			return ImGuiDataType_Float;
		else if constexpr (std::is_same<T, double>::value)		return ImGuiDataType_Double;
		else if constexpr (std::is_same<T, uint8_t>::value)		return ImGuiDataType_U8;
		else if constexpr (std::is_same<T, uint16_t>::value)	return ImGuiDataType_U16;
		else if constexpr (std::is_same<T, uint32_t>::value)	return ImGuiDataType_U32;
		else if constexpr (std::is_same<T, uint64_t>::value)	return ImGuiDataType_U64;
		else if constexpr (std::is_same<T, int8_t>::value)		return ImGuiDataType_S8;
		else if constexpr (std::is_same<T, int16_t>::value)		return ImGuiDataType_S16;
		else if constexpr (std::is_same<T, int32_t>::value)		return ImGuiDataType_S32;
		else if constexpr (std::is_same<T, int64_t>::value)		return ImGuiDataType_S64;
	}

	template<typename T>
	inline std::string Core_GUIDrawer::GetFormat()
	{
		if constexpr (std::is_same<T, double>::value)
		{
			return "%.5f";
		}
		else if constexpr (std::is_same<T, float>::value)
		{
			return "%.3f";
		}
		else
		{
			return "%d";
		}
	}

	template<typename T>
	inline void Core_GUIDrawer::DrawScalar(UI::UI_WidgetContainer& pRoot, const std::string& pName, T& pData, float pStep, T pMin, T pMax)
	{
		static_assert(std::is_scalar<T>::value, "T must be a scalar");

		CreateTitle(pRoot, pName);
		auto& widget = pRoot.CreateWidget<UI::UI_DragSingleScalar<T>>(GetDataType<T>(), pMin, pMax, pData, pStep, "", GetFormat<T>());
		auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<T>>();
		dispatcher.RegisterReference(pData);
	}

	template<typename T>
	inline void Core_GUIDrawer::DrawScalar(UI::UI_WidgetContainer& pRoot, const std::string& pName, std::function<T(void)> pGatherer, std::function<void(T)> pProvider, float pStep, T pMin, T pMax)
	{
		static_assert(std::is_scalar<T>::value, "T must be a scalar");

		CreateTitle(pRoot, pName);
		auto& widget = pRoot.CreateWidget<UI::UI_DragSingleScalar<T>>(GetDataType<T>(), pMin, pMax, static_cast<T>(0), pStep, "", GetFormat<T>());
		auto& dispatcher = widget.AddPlugin<UI::UI_DataDispatcher<T>>();
		dispatcher.RegisterGatherer(pGatherer);
		dispatcher.RegisterProvider(pProvider);
	}
}