#pragma once

#include "Window_FileDialog.h"

namespace Window
{
	class Window_OpenFileDialog : public Window_FileDialog
	{
	public:
		Window_OpenFileDialog(const std::string& pDialogTitle);

		void AddFileType(const std::string& pLabel, const std::string& pFilter);
	};
}

