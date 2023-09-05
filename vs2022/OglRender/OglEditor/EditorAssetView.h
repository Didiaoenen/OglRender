#pragma once

#include <string>

namespace Editor
{
class EditorAssetView
{
public:
	EditorAssetView
	(
		const std::string& pTitle,
		bool pOpened
	);

	virtual void _Render_Impl() override;

	void SetResource();

private:

};
}

