
#include <Windows.h>

#include "Window_SaveFileDialog.h"

Window::Window_SaveFileDialog::Window_SaveFileDialog(const std::string& pDialogTitle) : 
	Window_FileDialog(GetSaveFileNameA, pDialogTitle)
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
	if (mFileName.size() >= mExtension.size())
	{
		std::string fileEnd(mFileName.data() + mFileName.size() - mExtension.size(), mFileName.data() + mFileName.size());

		if (fileEnd != mExtension)
		{
			mFilePath += mExtension;
			mFileName += mExtension;
		}
	}
	else
	{
		mFilePath += mExtension;
		mFileName += mExtension;
	}
}
