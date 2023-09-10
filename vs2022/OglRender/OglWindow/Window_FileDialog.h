#pragma once

#include <string>
#include <functional>

struct tagOFNA;

namespace Window 
{
	enum class EExplorerFlags
	{
		READONLY = 0x00000001,
		OVERWRITEPROMPT = 0x00000002,
		HIDEREADONLY = 0x00000004,
		NOCHANGEDIR = 0x00000008,
		SHOWHELP = 0x00000010,
		ENABLEHOOK = 0x00000020,
		ENABLETEMPLATE = 0x00000040,
		ENABLETEMPLATEHANDLE = 0x00000080,
		NOVALIDATE = 0x00000100,
		ALLOWMULTISELECT = 0x00000200,
		EXTENSIONDIFFERENT = 0x00000400,
		PATHMUSTEXIST = 0x00000800,
		FILEMUSTEXIST = 0x00001000,
		CREATEPROMPT = 0x00002000,
		SHAREAWARE = 0x00004000,
		NOREADONLYRETURN = 0x00008000,
		NOTESTFILECREATE = 0x00010000,
		NONETWORKBUTTON = 0x00020000,
		NOLONGNAMES = 0x00040000,
		EXPLORER = 0x00080000,
		NODEREFERENCELINKS = 0x00100000,
		LONGNAMES = 0x00200000,
		ENABLEINCLUDENOTIFY = 0x00400000,
		ENABLESIZING = 0x00800000,
		DONTADDTORECENT = 0x02000000,
		FORCESHOWHIDDEN = 0x10000000,
	};

	inline EExplorerFlags operator~ (EExplorerFlags a) { return (EExplorerFlags)~(int)a; }
	inline EExplorerFlags operator| (EExplorerFlags a, EExplorerFlags b) { return (EExplorerFlags)((int)a | (int)b); }
	inline EExplorerFlags operator& (EExplorerFlags a, EExplorerFlags b) { return (EExplorerFlags)((int)a & (int)b); }
	inline EExplorerFlags operator^ (EExplorerFlags a, EExplorerFlags b) { return (EExplorerFlags)((int)a ^ (int)b); }
	inline EExplorerFlags& operator|= (EExplorerFlags& a, EExplorerFlags b) { return (EExplorerFlags&)((int&)a |= (int)b); }
	inline EExplorerFlags& operator&= (EExplorerFlags& a, EExplorerFlags b) { return (EExplorerFlags&)((int&)a &= (int)b); }
	inline EExplorerFlags& operator^= (EExplorerFlags& a, EExplorerFlags b) { return (EExplorerFlags&)((int&)a ^= (int)b); }

	class Window_FileDialog
	{
	public:
		Window_FileDialog(std::function<int(tagOFNA*)> pCallback, const std::string& pDialogTitle);

		void SetInitialDirectory(const std::string& pInitialDirectory);

		virtual void Show(EExplorerFlags pFlags = EExplorerFlags::DONTADDTORECENT | EExplorerFlags::FILEMUSTEXIST | EExplorerFlags::HIDEREADONLY | EExplorerFlags::NOCHANGEDIR);

		bool HasSucceeded() const;

		std::string GetSelectedFileName();

		std::string GetSelectedFilePath();

		std::string GetErrorInfo();

		bool IsFileExisting() const;

	private:
		void HandleError();

	protected:
		std::function<int(tagOFNA*)> mCallback;
		const std::string mDialogTitle;
		std::string mInitialDirectory;
		std::string mFilter;
		std::string mError;
		std::string mFilename;
		std::string mFilepath;
		bool mSucceeded{ false };
	};
}

