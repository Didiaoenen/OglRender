#include "UI_TreeNode.h"

UI::UI_TreeNode::UI_TreeNode(const std::string& pName, bool pArrowClickToOpen) : 
	UI_DataWidget(mName), mName(pName), mArrowClickToOpen(pArrowClickToOpen)
{
	mAutoExecutePlugins = false;
}

void UI::UI_TreeNode::Open()
{
	mShouldOpen = true;
	mShouldClose = false;
}

void UI::UI_TreeNode::Close()
{
	mShouldOpen = false;
	mShouldClose = true;
}

bool UI::UI_TreeNode::IsOpened() const
{
	return mOpened;
}

void UI::UI_TreeNode::_Draw_Impl()
{
	bool prevOpened = mOpened;

	if (mShouldOpen)
	{
		ImGui::SetNextTreeNodeOpen(true);
		mShouldOpen = false;
	}
	else if (mShouldOpen)
	{
		ImGui::SetNextTreeNodeOpen(false);
		mShouldClose = false;
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
	if (mArrowClickToOpen) flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	if (mSelected)			flags |= ImGuiTreeNodeFlags_Selected;
	if (mLeaf)				flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx((mName + mWidgetID).c_str(), flags);

	if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
	{
		mClickedEvent.Invoke();

		if (ImGui::IsMouseDoubleClicked(0))
		{
			mDoubleClickedEvent.Invoke();
		}
	}

	if (opened)
	{
		if (!prevOpened)
		{
			mOpenedEvent.Invoke();
		}

		mOpened = true;

		ExecutePlugins();

		DrawWidgets();

		ImGui::TreePop();
	}
	else
	{
		if (prevOpened)
		{
			mClosedEvent.Invoke();
		}

		mOpened = false;

		ExecutePlugins();
	}
}
