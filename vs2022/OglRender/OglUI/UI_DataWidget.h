#pragma once

#include "UI_AWidget.h"

namespace UI
{

	template<typename T>
	class UI_DataWidget : public UI_AWidget
	{
	public:
		UI_DataWidget(T& pDataHolder)
			: mData(pDataHolder)
		{

		}

		virtual void Draw() override;

		void NotifyChange();

	private:
		T& mData;
	};

	template<typename T>
	inline void UI_DataWidget<T>::Draw()
	{
		if (mEnabled)
		{
			TRY_GATHER(T, mData);
			UI_AWidget::Draw();
			TRY_PROVIDE(T, mData);
		}
	}

	template<typename T>
	inline void UI_DataWidget<T>::NotifyChange()
	{
		TRY_NOTIFY_CHANGE(T);
	}
}