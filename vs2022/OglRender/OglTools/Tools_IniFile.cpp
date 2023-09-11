
#include <fstream>
#include <filesystem>

#include "Tools_IniFile.h"

Tools::Tools_IniFile::Tools_IniFile(const std::string& pFilePath)
{
	Load();
}

void Tools::Tools_IniFile::Reload()
{
	RemoveAll();
	Load();
}

void Tools::Tools_IniFile::Rewrite() const
{
	std::ofstream outfile;
	outfile.open(mFilePath, std::ios_base::trunc);

	if (outfile.is_open())
	{
		for (const auto& [key, value] : mData)
		{
			outfile << key << "=" << value << std::endl;
		}
	}

	outfile.close();
}

bool Tools::Tools_IniFile::Remove(const std::string& pKey)
{
	if (IsKeyExisting(pKey))
	{
		mData.erase(pKey);
		return true;
	}

	return false;
}

void Tools::Tools_IniFile::RemoveAll()
{
	mData.clear();
}

bool Tools::Tools_IniFile::IsKeyExisting(const std::string& pKey) const
{
	return mData.find(pKey) != mData.end();
}

std::vector<std::string> Tools::Tools_IniFile::GetFormattedContent() const
{
	std::vector<std::string> result;

	for (const auto& [key, value] : mData)
	{
		result.push_back(key + "=" + value);
	}

	return result;
}

void Tools::Tools_IniFile::RegisterPair(const std::string& pKey, const std::string& pValue)
{
	RegisterPair(std::make_pair(pKey, pValue));
}

void Tools::Tools_IniFile::RegisterPair(const AttributePair& pPair)
{
	mData.insert(pPair);
}

void Tools::Tools_IniFile::Load()
{
	std::fstream iniFile;
	iniFile.open(mFilePath);

	if (iniFile.is_open())
	{
		std::string currentLine;

		while (std::getline(iniFile, currentLine))
		{
			if (IsValidLine(currentLine))
			{
				currentLine.erase(std::remove_if(currentLine.begin(), currentLine.end(), isspace), currentLine.end());
				RegisterPair(ExtractKeyAndValue(currentLine));
			}
		}

		iniFile.close();
	}
}

Tools::Tools_IniFile::AttributePair Tools::Tools_IniFile::ExtractKeyAndValue(const std::string& pAttributeLine) const
{
	std::string key;
	std::string value;

	std::string* currentBuffer = &key;

	for (auto& c : pAttributeLine)
	{
		if (c == '=')
		{
			currentBuffer = &value;
		}
		else
		{
			currentBuffer->push_back(c);
		}
	}

	return std::make_pair(key, value);
}

bool Tools::Tools_IniFile::IsValidLine(const std::string& pAttributeLine) const
{
	if (pAttributeLine.size() == 0)
	{
		return false;
	}

	if (pAttributeLine[0] == '#' || pAttributeLine[0] == ';' || pAttributeLine[0] == '[')
	{
		return false;
	}

	if (std::count(pAttributeLine.begin(), pAttributeLine.end(), '=') != 1)
	{
		return false;
	}

	return true;
}

bool Tools::Tools_IniFile::StringToBoolean(const std::string& pValue) const
{
	return (pValue == "1" || pValue == "T" || pValue == "t" || pValue == "True" || pValue == "true");
}
