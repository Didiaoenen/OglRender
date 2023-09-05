#pragma once

#include <string>

namespace Editor
{
class EditorAssetProperties
{
public:
	EditorAssetProperties
	(
		const std::string& pTitle,
		bool pOpened
	);

	void SetTarget(const std::string& pPath);

	void Refresh();

	void Preview();

private:
	std::string mResources;


};
}

