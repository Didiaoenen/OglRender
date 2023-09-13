#pragma once

#include <OglUI/UI_Text.h>
#include <OglUI/UI_PanelWindow.h>

#include <OglTools/Tools_IniFile.h>

namespace Editor
{
	class Editor_ProjectSettings : public UI::UI_PanelWindow
	{
	public:
		Editor_ProjectSettings
		(
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		template <typename T>
		std::function<T()> GenerateGatherer(const std::string& p_keyName)
		{
			return std::bind(&Tools::Tools_IniFile::Get<T>, &m_projectFile, p_keyName);
		}

		template <typename T>
		std::function<void(T)> GenerateProvider(const std::string& p_keyName)
		{
			return std::bind(&Tools::Tools_IniFile::Set<T>, &m_projectFile, p_keyName, std::placeholders::_1);
		}

	private:
		Tools::Tools_IniFile& m_projectFile;
	};
}

