#pragma once

#include <string>
#include <functional>

namespace Tools
{
	class Tools_String
	{
	public:
		Tools_String() = delete;

		static bool Replace(std::string& p_target, const std::string& p_from, const std::string& p_to);

		static void ReplaceAll(std::string& p_target, const std::string& p_from, const std::string& p_to);

		static std::string GenerateUnique(const std::string& p_source, std::function<bool(std::string)> p_isAvailable);
	};
}

