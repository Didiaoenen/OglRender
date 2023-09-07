#include "UI_APanelTransformable.h"
#include "UI_Converter.h"

UI::UI_APanelTransformable::UI_APanelTransformable(const glm::vec2& pDefaultPosition, const glm::vec2& pDefaultSize, EHorizontalAlignment pDefaultHorizontalAlignment, EVerticalAlignment pDefaultVerticalAlignment, bool pIgnoreConfigFile)
	: mDefaultPosition(pDefaultPosition),
	mDefaultSize(pDefaultSize),
	mDefaultHorizontalAlignment(pDefaultHorizontalAlignment),
	mDefaultVerticalAlignment(pDefaultVerticalAlignment),
	mIgnoreConfigFile(pIgnoreConfigFile)
{
}

void UI::UI_APanelTransformable::SetPosition(const glm::vec2& pPosition)
{
	mPosition = pPosition;
	mPositionChanged = true;
}

void UI::UI_APanelTransformable::SetSize(const glm::vec2& pSize)
{
	mSize = pSize;
	mSizeChanged = true;
}

void UI::UI_APanelTransformable::SetAlignment(EHorizontalAlignment pHorizontalAlignment, EVerticalAlignment pVerticalAligment)
{
	mHorizontalAlignment = pHorizontalAlignment;
	mVerticalAlignment = pVerticalAligment;
	mAlignmentChanged = true;
}

const glm::vec2& UI::UI_APanelTransformable::GetPosition() const
{
	return mPosition;
}

const glm::vec2& UI::UI_APanelTransformable::GetSize() const
{
	return mSize;
}

UI::EHorizontalAlignment UI::UI_APanelTransformable::GetHorizontalAlignment() const
{
	return mHorizontalAlignment;
}

UI::EVerticalAlignment UI::UI_APanelTransformable::GetVerticalAlignment() const
{
	return mVerticalAlignment;
}

void UI::UI_APanelTransformable::Update()
{
	if (!mFirstFrame)
	{
		if (!mAutoSize)
		{
			UpdateSize();
		}
		CopyImGuiSize();

		UpdatePosition();
		CopyImGuiPosition();
	}

	mFirstFrame = false;
}

glm::vec2 UI::UI_APanelTransformable::CalculatePositionAlignmentOffset(bool pDefault)
{
	glm::vec2 result(0.0f, 0.0f);

	switch (pDefault ? mDefaultHorizontalAlignment : mHorizontalAlignment)
	{
	case EHorizontalAlignment::CENTER:
		result.x -= mSize.x / 2.0f;
		break;
	case EHorizontalAlignment::RIGHT:
		result.x -= mSize.x;
		break;
	}

	switch (pDefault ? mDefaultVerticalAlignment : mVerticalAlignment)
	{
	case EVerticalAlignment::MIDDLE:
		result.y -= mSize.y / 2.0f;
		break;
	case EVerticalAlignment::BOTTOM:
		result.y -= mSize.y;
		break;
	}

	return result;
}

void UI::UI_APanelTransformable::UpdatePosition()
{
	if (mDefaultPosition.x != -1.f && mDefaultPosition.y != 1.f)
	{
		glm::vec2 offsettedDefaultPos = mPosition + CalculatePositionAlignmentOffset(true);
		ImGui::SetWindowPos(UI_Converter::ToImVec2(offsettedDefaultPos), mIgnoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	}

	if (mPositionChanged || mAlignmentChanged)
	{
		glm::vec2 offset = CalculatePositionAlignmentOffset(false);
		glm::vec2 offsettedPos(mPosition.x + offset.x, mPosition.y + offset.y);
		ImGui::SetWindowPos(UI_Converter::ToImVec2(offsettedPos), ImGuiCond_Always);
		mPositionChanged = false;
		mAlignmentChanged = false;
	}
}

void UI::UI_APanelTransformable::UpdateSize()
{
	if (mSizeChanged)
	{
		ImGui::SetWindowSize(UI_Converter::ToImVec2(mSize), ImGuiCond_Always);
		mSizeChanged = false;
	}
}

void UI::UI_APanelTransformable::CopyImGuiPosition()
{
	mPosition = UI_Converter::ToGlmVec2(ImGui::GetWindowPos());
}

void UI::UI_APanelTransformable::CopyImGuiSize()
{
	mSize = UI_Converter::ToGlmVec2(ImGui::GetWindowSize());
}
