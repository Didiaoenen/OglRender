#include <Windows.h>
#include "Window_SaveFileDialog.h"

Window::Window_SaveFileDialog::Window_SaveFileDialog(const std::string& pDialogTitle)
	: Window_FileDialog(GetSaveFileNameA, pDialogTitle)
{
}

void Window::Window_SaveFileDialog::Show(EExplorerFlags pFlags)
{
	Window_FileDialog::Show(pFlags);

	if (mSucceeded)
	{
		AddExtensionToFilePathAndName();
	}
}

void Window::Window_SaveFileDialog::DefineExtension(const std::string& pLabel, const std::string& pExtension)
{
	mFilter = pLabel + '\0' + '*' + pExtension + '\0';
	mExtension = pExtension;
}

void Window::Window_SaveFileDialog::AddExtensionToFilePathAndName()
{
	if (mFilename.size() >= mExtension.size())
	{
		std::string fileEnd(mFilename.data() + mFilename.size() - mExtension.size(), mFilename.data() + mFilename.size());

		if (fileEnd != mExtension)
		{
			mFilepath += mExtension;
			mFilename += mExtension;
		}
	}
	else
	{
		mFilepath += mExtension;
		mFilename += mExtension;
	}
}
