#pragma once

#include <string>

#include <OglTools/Tools_Event.h>

#include "imgui.h"
#include "UI_IPlugin.h"

namespace UI
{
	template<typename T>
	class UI_DDSource : public UI_IPlugin
	{
	public:
		UI_DDSource
		(
			const std::string& pIdentifier,
			const std::string& pTooltip,
			T pData
		) : mIdentifier(pIdentifier), mTooltip(pTooltip), mData(pData)
		{
		}

		virtual void Execute() override
		{
			ImGuiDragDropFlags src_flags = 0;
			src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
			src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

			if (!mHasTooltip)
				src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip;

			if (ImGui::BeginDragDropSource(src_flags))
			{
				if (!mIsDragged)
				{
					mDragStartEvent.Invoke();
				}

				mIsDragged = true;

				if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
				{
					ImGui::Text(mTooltip.c_str());
				}
				ImGui::SetDragDropPayload(mIdentifier.c_str(), &mData, sizeof(mData));
				ImGui::EndDragDropSource();
			}
			else
			{
				if (mIsDragged)
				{
					mDragStopEvent.Invoke();
				}

				mIsDragged = false;
			}
		}

		bool IsDragged() const
		{
			return mIsDragged;
		}

	public:
		std::string mIdentifier;
		std::string mTooltip;
		T mData;
		Tools::Tools_Event<> mDragStartEvent;
		Tools::Tools_Event<> mDragStopEvent;

		bool mHasTooltip = true;

	private:
		bool mIsDragged;
	};
}

