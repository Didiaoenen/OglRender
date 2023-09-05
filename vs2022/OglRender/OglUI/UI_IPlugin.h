#pragma once

namespace UI
{
class UI_IPlugin
{
public:
	virtual void Execute() = 0;

	void* userData{ nullptr };
};
}