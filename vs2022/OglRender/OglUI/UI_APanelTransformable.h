#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "OglTools/Tools_Event.h"

#include "UI_APanel.h"
#include "UI_Alignment.h"

namespace UI
{
	class UI_APanelTransformable : public UI_APanel
	{
	public:
		UI_APanelTransformable
		(
			const glm::vec2& pDefaultPosition = glm::vec2(-1.f, -1.f),
			const glm::vec2& pDefaultSize = glm::vec2(-1.f, -1.f),
			EHorizontalAlignment pDefaultHorizontalAlignment = EHorizontalAlignment::LEFT,
			EVerticalAlignment pDefaultVerticalAlignment = EVerticalAlignment::TOP,
			bool pIgnoreConfigFile = false
		);

		void SetPosition(const glm::vec2& pPosition);

		void SetSize(const glm::vec2& pSize);

		void SetAlignment(EHorizontalAlignment pHorizontalAlignment, EVerticalAlignment pVerticalAligment);

		const glm::vec2& GetPosition() const;

		const glm::vec2& GetSize() const;

		EHorizontalAlignment GetHorizontalAlignment() const;

		EVerticalAlignment GetVerticalAlignment() const;

	protected:
		void Update();
		virtual void _Draw_Impl() = 0;

	private:
		glm::vec2 CalculatePositionAlignmentOffset(bool pDefault = false);

		void UpdatePosition();
		void UpdateSize();
		void CopyImGuiPosition();
		void CopyImGuiSize();

	public:
		bool mAutoSize{ true };

	protected:
		glm::vec2 mDefaultPosition;
		glm::vec2 mDefaultSize;
		EHorizontalAlignment mDefaultHorizontalAlignment;
		EVerticalAlignment mDefaultVerticalAlignment;
		bool mIgnoreConfigFile;

		glm::vec2 mPosition{ 0.0f, 0.0f };
		glm::vec2 mSize{ 0.0f, 0.0f };

		bool mPositionChanged{ false };
		bool mSizeChanged{ false };

		EHorizontalAlignment mHorizontalAlignment = EHorizontalAlignment::LEFT;
		EVerticalAlignment mVerticalAlignment = EVerticalAlignment::TOP;

		bool mAlignmentChanged{ false };
		bool mFirstFrame{ true };
	};
}

