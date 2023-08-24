#pragma once

#include <list>
#include <memory>

#include "OGL_Base.h"

namespace OGL
{
class OGL_TreeNode
{
public:
	virtual ~OGL_TreeNode() = default;
	virtual void AppendChild(Ref<OGL_TreeNode>&& subNode) 
	{
		subNode->mParent = this;
		mChildren.push_back(std::move(subNode));
	}

protected:
	OGL_TreeNode* mParent{ nullptr };
	std::list<Ref<OGL_TreeNode>> mChildren;
};
}