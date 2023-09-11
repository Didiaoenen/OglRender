
#include <Windows.h>

#include <filesystem>

#include "Window_FileDialog.h"

Window::Window_FileDialog::Window_FileDialog(std::function<int(tagOFNA*)> pCallback, const std::string& pDialogTitle) : 
	mCallback(pCallback), mDialogTitle(pDialogTitle), mInitialDirectory("")
{
}

void Window::Window_FileDialog::SetInitialDirectory(const std::string& pInitialDirectory)
{
	mInitialDirectory = pInitialDirectory;
}

void Window::Window_FileDialog::Show(EExplorerFlags pFlags)
{
	OPENFILENAME ofn;

	if (!mInitialDirectory.empty())
	{
		mFilePath = mInitialDirectory;
	}

	mFilePath.resize(MAX_PATH);

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = mFilter.c_str();
	ofn.lpstrFile = mFilePath.data();
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = mDialogTitle.c_str();

	if (!mInitialDirectory.empty())
	{
		ofn.lpstrInitialDir = mInitialDirectory.c_str();
	}

	ofn.Flags = static_cast<DWORD>(pFlags);

	mSucceeded = mCallback(&ofn);

	if (!mSucceeded)
	{
		HandleError();
	}
	else
	{
		mFilePath = mFilePath.c_str();
	}

	mFileName.clear();
	for (auto it = mFilePath.rbegin(); it != mFilePath.rend() && *it != '\\' && *it != '/'; ++it)
	{
		mFileName += *it;
	}
	std::reverse(mFileName.begin(), mFileName.end());
}

bool Window::Window_FileDialog::HasSucceeded() const
{
	return mSucceeded;
}

std::string Window::Window_FileDialog::GetSelectedFileName()
{
	return mFileName;
}

std::string Window::Window_FileDialog::GetSelectedFilePath()
{
	return mFilePath;
}

std::string Window::Window_FileDialog::GetErrorInfo()
{
	return mError;
}

bool Window::Window_FileDialog::IsFileExisting() const
{
	return std::filesystem::exists(mFilePath);
}

void Window::Window_FileDialog::HandleError()
{
	switch (CommDlgExtendedError())
	{
	case CDERR_DIALOGFAILURE:	mError = "CDERR_DIALOGFAILURE";   break;
	case CDERR_FINDRESFAILURE:	mError = "CDERR_FINDRESFAILURE";  break;
	case CDERR_INITIALIZATION:	mError = "CDERR_INITIALIZATION";  break;
	case CDERR_LOADRESFAILURE:	mError = "CDERR_LOADRESFAILURE";  break;
	case CDERR_LOADSTRFAILURE:	mError = "CDERR_LOADSTRFAILURE";  break;
	case CDERR_LOCKRESFAILURE:	mError = "CDERR_LOCKRESFAILURE";  break;
	case CDERR_MEMALLOCFAILURE: mError = "CDERR_MEMALLOCFAILURE"; break;
	case CDERR_MEMLOCKFAILURE:	mError = "CDERR_MEMLOCKFAILURE";  break;
	case CDERR_NOHINSTANCE:		mError = "CDERR_NOHINSTANCE";     break;
	case CDERR_NOHOOK:			mError = "CDERR_NOHOOK";          break;
	case CDERR_NOTEMPLATE:		mError = "CDERR_NOTEMPLATE";      break;
	case CDERR_STRUCTSIZE:		mError = "CDERR_STRUCTSIZE";      break;
	case FNERR_BUFFERTOOSMALL:	mError = "FNERR_BUFFERTOOSMALL";  break;
	case FNERR_INVALIDFILENAME: mError = "FNERR_INVALIDFILENAME"; break;
	case FNERR_SUBCLASSFAILURE: mError = "FNERR_SUBCLASSFAILURE"; break;
	default:					mError = "You cancelled.";
	}
}
