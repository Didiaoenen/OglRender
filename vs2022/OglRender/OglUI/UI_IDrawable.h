#pragma once

namespace UI
{
	class UI_IDrawable
	{
	public:
		virtual void Draw() = 0;

	protected:
		virtual ~UI_IDrawable() = default;
	};
}