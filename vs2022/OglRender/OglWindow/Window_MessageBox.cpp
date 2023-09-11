
#include <Windows.h>

#include "Window_MessageBox.h"

Window::Window_MessageBox::Window_MessageBox(std::string pTitle, std::string pMessage, EMessageType pMessageType, EButtonLayout pButtonLayout, bool pAutoSpawn) : 
	mTitle(pTitle), mMessage(pMessage), mButtonLayout(pButtonLayout), mMessageType(pMessageType)
{
	if (pAutoSpawn)
	{
		Spawn();
	}
}

void Window::Window_MessageBox::Spawn()
{
	int msgboxID = MessageBoxA
	(
		nullptr,
		static_cast<LPCSTR>(mMessage.c_str()),
		static_cast<LPCSTR>(mTitle.c_str()),
		static_cast<UINT>(mMessageType) | static_cast<UINT>(mButtonLayout) | MB_DEFBUTTON2
	);

	mUserResult = static_cast<EUserAction>(msgboxID);
}

const Window::Window_MessageBox::EUserAction& Window::Window_MessageBox::GetUserAction() const
{
	return mUserResult;
}
