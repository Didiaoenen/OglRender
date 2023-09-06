#pragma once

#include <array>

#include "UI_WidgetContainer.h"

namespace UI
{
	template <size_t _Size>
	class UI_Columns : public UI_WidgetContainer
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

			for (auto it = m_widgets.begin(); it != m_widgets.end();)
			{
				it->first->Draw();

				++it;

				if (it != m_widgets.end())
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

