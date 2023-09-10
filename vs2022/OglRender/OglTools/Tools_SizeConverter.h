#pragma once

#include <tuple>
#include <string>
#include <cstdint>

namespace Tools
{
	class Tools_SizeConverter
	{
	public:
        enum class ESizeUnit
        {
            BYTE = 0,
            KILO_BYTE = 3,
            MEGA_BYTE = 6,
            GIGA_BYTE = 9,
            TERA_BYTE = 12
        };

        Tools_SizeConverter() = delete;

        static std::pair<float, ESizeUnit> ConvertToOptimalUnit(float p_value, ESizeUnit p_unit);

        static float Convert(float p_value, ESizeUnit p_from, ESizeUnit p_to);

        static std::string UnitToString(ESizeUnit p_unit);
	};
}

