#pragma once

#include <string>

namespace Editor
{
class EditorAssetBrowser
{
public:
	EditorAssetBrowser
	(
		const std::string& pTitle,
		bool pOpened,
		const std::string& pEngineAssetFolder = "",
		const std::string& pProjectAssetFolder = "",
		const std::string& pProjectScriprFolder = ""
	);

	void Fill();

	void Clear();

	void Refresh();

private:
	void ParseFolder();
	void ConsiderItem();

public:
	static const std::string __FILENAMES_CHARS;

private:
	std::string mEngineAssetFolder;
	std::string mProjectAssetFolder;
	std::string mProjectScriptFolder;


};
}

