#include "Tools_String.h"

bool Tools::Tools_String::Replace(std::string& pTarget, const std::string& pFrom, const std::string& pTo)
{
	size_t start_pos = pTarget.find(pFrom);

	if (start_pos != std::string::npos)
	{
		pTarget.replace(start_pos, pFrom.length(), pTo);
		return true;
	}

	return false;
}

void Tools::Tools_String::ReplaceAll(std::string& pTarget, const std::string& pFrom, const std::string& pTo)
{
	if (pFrom.empty()) return;

	size_t start_pos = 0;
	while ((start_pos = pTarget.find(pFrom, start_pos)) != std::string::npos)
	{
		pTarget.replace(start_pos, pFrom.length(), pTo);
		start_pos += pTo.length();
	}
}

std::string Tools::Tools_String::GenerateUnique(const std::string& pSource, std::function<bool(std::string)> pIsAvailable)
{
    auto suffixlessSource = pSource;

    auto suffixOpeningParenthesisPos = std::string::npos;
    auto suffixClosingParenthesisPos = std::string::npos;

    auto currentPos = decltype(std::string::npos){pSource.length() - 1};

    for (auto it = pSource.rbegin(); it < pSource.rend(); ++it, --currentPos)
    {
        const auto c = *it;

        if (suffixClosingParenthesisPos == std::string::npos && c == ')') suffixClosingParenthesisPos = currentPos;
        if (suffixClosingParenthesisPos != std::string::npos && c == '(') suffixOpeningParenthesisPos = currentPos;
    }

    auto counter = uint32_t{ 1 };

    if (suffixOpeningParenthesisPos != std::string::npos && suffixClosingParenthesisPos == pSource.length() - 1 && suffixOpeningParenthesisPos > 0 && pSource[suffixOpeningParenthesisPos - 1] == ' ')
    {
        const auto between = pSource.substr(suffixOpeningParenthesisPos + 1, suffixClosingParenthesisPos - suffixOpeningParenthesisPos - 1);

        if (!between.empty() && std::find_if(between.begin(), between.end(), [](unsigned char c) { return !std::isdigit(c); }) == between.end())
        {
            counter = static_cast<uint32_t>(std::atoi(between.c_str()));
            suffixlessSource = pSource.substr(0, suffixOpeningParenthesisPos - 1);
        }
    }

    auto result = suffixlessSource;

    while (!pIsAvailable(result))
    {
        result = suffixlessSource + " (" + std::to_string(counter++) + ")";
    }

    return result;
}
