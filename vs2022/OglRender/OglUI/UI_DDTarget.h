#pragma once

#include <string>

#include <OglTools/Tools_Event.h>

#include "imgui.h"
#include "UI_IPlugin.h"

namespace UI
{
	template<typename T>
	class UI_DDTarget : public UI_IPlugin
	{
	public:
		UI_DDTarget(const std::string& pIdentifier)
			: mIdentifier(pIdentifier)
		{
		}

		virtual void Execute() override
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (!mIsHovered)
				{
					mHoverStartEvent.Invoke();
				}

				mIsHovered = true;

				ImGuiDragDropFlags target_flags = 0;

				if (!mShowYellowRect)
				{
					target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(mIdentifier.c_str(), target_flags))
				{
					T data = *(T*)payload->Data;
					mDataReceivedEvent.Invoke(data);
				}
				ImGui::EndDragDropTarget();
			}
			else
			{
				if (mIsHovered)
				{
					mHoverEndEvent.Invoke();
				}

				mIsHovered = false;
			}
		}

		bool IsHovered() const
		{
			return mIsHovered;
		}

	public:
		std::string mIdentifier;
		Tools::Tools_Event<T> mDataReceivedEvent;
		Tools::Tools_Event<> mHoverStartEvent;
		Tools::Tools_Event<> mHoverEndEvent;

		bool mShowYellowRect{ true };

	private:
		bool mIsHovered{ false };
	};
}

