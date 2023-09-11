#pragma once

#include <string>

#undef MessageBox
#undef ERROR
#undef IGNORE

namespace Window
{
	class Window_MessageBox
	{
	public:
		enum class EMessageType
		{
			QUESTION = 0x00000020L,
			INFORMATION = 0x00000040L,
			WARNING = 0x00000030L,
			ERROR = 0x00000010L,
		};

		enum class EButtonLayout
		{
			OK = 0x00000000L,
			OK_CANCEL = 0x00000001L,
			YES_NO = 0x00000004L,
			YES_NO_CANCEL = 0x00000003L,
			RETRY_CANCEL = 0x00000005L,
			ABORT_RETRY_IGNORE = 0x00000002L,
			CANCEL_TRYAGAIN_CONTINUE = 0x00000006L,
			HELP = 0x00004000L,
		};

		enum class EUserAction
		{
			OK = 1,
			CANCEL = 2,
			YES = 6,
			NO = 7,
			CONTINUE = 11,
			IGNORE = 5,
			RETRY = 4,
			TRYAGAIN = 10,
		};

		Window_MessageBox(std::string pTitle, std::string pMessage, EMessageType pMessageType = EMessageType::INFORMATION, EButtonLayout pButtonLayout = EButtonLayout::OK, bool pAutoSpawn = true);

		void Spawn();

		const EUserAction& GetUserAction() const;

	private:
		std::string		mTitle;
		std::string		mMessage;
		EButtonLayout	mButtonLayout;
		EMessageType	mMessageType;
		EUserAction		mUserResult;
	};
}

