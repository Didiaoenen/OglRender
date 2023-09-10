#include "Tools_String.h"

bool Tools::Tools_String::Replace(std::string& p_target, const std::string& p_from, const std::string& p_to)
{
    return false;
}

void Tools::Tools_String::ReplaceAll(std::string& p_target, const std::string& p_from, const std::string& p_to)
{
}

std::string Tools::Tools_String::GenerateUnique(const std::string& p_source, std::function<bool(std::string)> p_isAvailable)
{
    return std::string();
}
