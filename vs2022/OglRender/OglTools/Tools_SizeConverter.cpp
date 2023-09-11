#include "Tools_SizeConverter.h"

std::pair<float, Tools::Tools_SizeConverter::ESizeUnit> Tools::Tools_SizeConverter::ConvertToOptimalUnit(float pValue, ESizeUnit pUnit)
{
    if (pValue == 0.0f) return { 0.0f, ESizeUnit::BYTE };
    const float bytes = Convert(pValue, pUnit, ESizeUnit::BYTE);
    const int digits = static_cast<int>(trunc(log10(bytes)));
    const ESizeUnit targetUnit = static_cast<ESizeUnit>(fmin(3.0f * floor(digits / 3.0f), static_cast<float>(ESizeUnit::TERA_BYTE)));

    return { Convert(bytes, ESizeUnit::BYTE, targetUnit), targetUnit };
}

float Tools::Tools_SizeConverter::Convert(float pValue, ESizeUnit pFrom, ESizeUnit pTo)
{
    const float fromValue = powf(1024.0f, static_cast<float>(pFrom) / 3.0f);
    const float toValue = powf(1024.0f, static_cast<float>(pTo) / 3.0f);

    return pValue * (fromValue / toValue);
}

std::string Tools::Tools_SizeConverter::UnitToString(ESizeUnit pUnit)
{
    switch (pUnit)
    {
        case Tools_SizeConverter::ESizeUnit::BYTE: return "B";
        case Tools_SizeConverter::ESizeUnit::KILO_BYTE: return "KB";
        case Tools_SizeConverter::ESizeUnit::MEGA_BYTE: return "MB";
        case Tools_SizeConverter::ESizeUnit::GIGA_BYTE: return "GB";
        case Tools_SizeConverter::ESizeUnit::TERA_BYTE: return "TB";
    }
    
    return "?";
}
