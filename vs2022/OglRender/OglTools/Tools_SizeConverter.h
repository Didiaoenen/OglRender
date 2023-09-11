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

        static std::pair<float, ESizeUnit> ConvertToOptimalUnit(float pValue, ESizeUnit pUnit);

        static float Convert(float pValue, ESizeUnit pFrom, ESizeUnit pTo);

        static std::string UnitToString(ESizeUnit pUnit);
	};
}

