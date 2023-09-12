#pragma once

#include <array>

#include "UI_WidgetContainer.h"

namespace UI
{
	template <size_t _Size>
	class UI_Columns : public UI_AWidget, public UI_WidgetContainer
	{
	public:
		UI_Columns()
		{
			mWidths.fill(-1.f);
		}

	protected:
		virtual void _Draw_Impl() override
		{
			ImGui::Columns(static_cast<int>(_Size), ("##" + mWidgetID).c_str(), false);

			int counter = 0;

			CollectGarbages();

			for (auto it = mWidgets.begin(); it != mWidgets.end();)
			{
				it->first->Draw();

				++it;

				if (it != mWidgets.end())
				{
					if (mWidths[counter] != -1.f)
					{
						ImGui::SetColumnWidth(counter, mWidths[counter]);
					}

					ImGui::NextColumn();
				}

				++counter;

				if (counter == _Size)
				{
					counter = 0;
				}
			}

			ImGui::Columns(1);
		}

	public:
		std::array<float, _Size> mWidths;
	};
}

