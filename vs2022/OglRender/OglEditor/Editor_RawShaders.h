#pragma once

#include <string>

namespace Editor
{
	class Editor_RawShaders
	{
	public:
		static std::pair<std::string, std::string> GetGrid();

		static std::pair<std::string, std::string> GetGizmo();

		static std::pair<std::string, std::string> GetBillboard();
	};
}

