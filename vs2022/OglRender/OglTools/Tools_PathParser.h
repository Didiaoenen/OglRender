#pragma once

#include <string>

namespace Tools
{
	class Tools_PathParser
	{
	public:
		enum class EFileType
		{
			UNKNOWN,
			MODEL,
			TEXTURE,
			SHADER,
			MATERIAL,
			SOUND,
			SCENE,
			SCRIPT,
			FONT
		};

		Tools_PathParser() = delete;

		static std::string MakeWindowsStyle(const std::string& pPath);

		static std::string MakeNonWindowsStyle(const std::string& pPath);

		static std::string GetContainingFolder(const std::string& pPath);

		static std::string GetElementName(const std::string& pPath);

		static std::string GetExtension(const std::string& pPath);

		static std::string FileTypeToString(EFileType pFileType);

		static EFileType GetFileType(const std::string& pPath);
	};
}

