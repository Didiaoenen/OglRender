#pragma once

#include <string>

namespace Editor
{
class EditorContext
{
public:
	EditorContext(const std::string& pProjectPath, const std::string& pProjectName);

	~EditorContext();

	void ResetProjectSettings();

	bool IsProjectSettingIntegrityVerified();

	void ApplyProjectSettings();

public:
	const std::string mProjectPath;
	const std::string mProjectName;
	const std::string mProjectFilePath;
	const std::string mEngineAssetsPath;
	const std::string mProjectAssetsPath;
	const std::string mProjectScpritsPath;
	const std::string mEditorAssetsPath;
};
}

