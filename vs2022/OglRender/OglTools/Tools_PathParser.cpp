
#include <algorithm>

#include "Tools_PathParser.h"

std::string Tools::Tools_PathParser::MakeWindowsStyle(const std::string& pPath)
{
    std::string result;
    result.resize(pPath.size());

    for (size_t i = 0; i < pPath.size(); ++i)
    {
        result[i] = pPath[i] == '/' ? '\\' : pPath[i];
    }

    return result;
}

std::string Tools::Tools_PathParser::MakeNonWindowsStyle(const std::string& pPath)
{
    std::string result;
    result.resize(pPath.size());

    for (size_t i = 0; i < pPath.size(); ++i)
    {
        result[i] = pPath[i] == '\\' ? '/' : pPath[i];
    }

    return result;
}

std::string Tools::Tools_PathParser::GetContainingFolder(const std::string& pPath)
{
    std::string result;

    bool extraction = false;

    for (auto it = pPath.rbegin(); it != pPath.rend(); ++it)
    {
        if (extraction)
        {
            result += *it;
        }

        if (!extraction && it != pPath.rbegin() && (*it == '\\' || *it == '/'))
        {
            extraction = true;
        }
    }

    std::reverse(result.begin(), result.end());

    if (!result.empty() && result.back() != '\\')
    {
        result += '\\';
    }

    return result;
}

std::string Tools::Tools_PathParser::GetElementName(const std::string& pPath)
{
    std::string result;

    std::string path = pPath;
    if (!path.empty() && path.back() == '\\')
    {
        path.pop_back();
    }

    for (auto it = path.rbegin(); it != path.rend() && *it != '\\' && *it != '/'; ++it)
    {
        result += *it;
    }

    std::reverse(result.begin(), result.end());

    return result;
}

std::string Tools::Tools_PathParser::GetExtension(const std::string& pPath)
{
    std::string result;

    for (auto it = pPath.rbegin(); it != pPath.rend() && *it != '.'; ++it)
    {
        result += *it;
    }

    std::reverse(result.begin(), result.end());

    return result;
}

std::string Tools::Tools_PathParser::FileTypeToString(EFileType pFileType)
{
    switch (pFileType)
    {
        case Tools_PathParser::EFileType::MODEL:	return "Model";
        case Tools_PathParser::EFileType::TEXTURE:	return "Texture";
        case Tools_PathParser::EFileType::SHADER:	return "Shader";
        case Tools_PathParser::EFileType::MATERIAL:	return "Material";
        case Tools_PathParser::EFileType::SOUND:	return "Sound";
        case Tools_PathParser::EFileType::SCENE:	return "Scene";
        case Tools_PathParser::EFileType::SCRIPT:	return "Script";
        case Tools_PathParser::EFileType::FONT:		return "Font";
    }

    return "Unknown";
}

Tools::Tools_PathParser::EFileType Tools::Tools_PathParser::GetFileType(const std::string& pPath)
{
    std::string ext = GetExtension(pPath);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == "fbx" || ext == "obj")											return EFileType::MODEL;
    else if (ext == "png" || ext == "jpeg" || ext == "jpg" || ext == "tga")		return EFileType::TEXTURE;
    else if (ext == "glsl" || ext == "shader")									return EFileType::SHADER;
    else if (ext == "ovmat")													return EFileType::MATERIAL;
    else if (ext == "wav" || ext == "mp3" || ext == "ogg")						return EFileType::SOUND;
    else if (ext == "ovscene")													return EFileType::SCENE;
    else if (ext == "lua")														return EFileType::SCRIPT;
    else if (ext == "ttf")														return EFileType::FONT;

    return EFileType::UNKNOWN;
}
