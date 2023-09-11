
#include <Windows.h>

#include "Window_OpenFileDialog.h"

Window::Window_OpenFileDialog::Window_OpenFileDialog(const std::string& pDialogTitle) : 
	Window_FileDialog(GetOpenFileNameA, pDialogTitle)
{
}

void Window::Window_OpenFileDialog::AddFileType(const std::string& pLabel, const std::string& pFilter)
{
	mFilter += pLabel + '\0' + pFilter + '\0';
}
