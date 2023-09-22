#pragma once

#include <string>
#include <functional>

namespace Tools
{
	class Tools_String
	{
	public:
		Tools_String() = delete;

		static bool Replace(std::string& pTarget, const std::string& pFrom, const std::string& pTo);

		static void ReplaceAll(std::string& pTarget, const std::string& pFrom, const std::string& pTo);

		static std::string GenerateUnique(const std::string& pSource, std::function<bool(std::string)> pIsAvailable);
	};
}

