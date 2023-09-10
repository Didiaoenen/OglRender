#pragma once

#include "Window_FileDialog.h"

namespace Window
{
	class Window_SaveFileDialog : public Window_FileDialog
	{
	public:
		Window_SaveFileDialog(const std::string& pDialogTitle);

		virtual void Show(EExplorerFlags pFlags = EExplorerFlags::DONTADDTORECENT | EExplorerFlags::FILEMUSTEXIST | EExplorerFlags::HIDEREADONLY | EExplorerFlags::NOCHANGEDIR) override;

		void DefineExtension(const std::string& pLabel, const std::string& pExtension);

	private:
		void AddExtensionToFilePathAndName();

	private:
		std::string mExtension;
	};
}

