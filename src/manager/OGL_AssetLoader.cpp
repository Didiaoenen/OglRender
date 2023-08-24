#include "OGL_AssetLoader.h"

using namespace OGL;

std::string OGL_AssetLoader::mTargetPath;
std::vector<std::string> OGL_AssetLoader::mSearchPath;

bool OGL_AssetLoader::Initialize()
{
    AddSearchPath("./../../../");
    return true;
}

bool OGL_AssetLoader::AddSearchPath(const char* path)
{
    auto src = mSearchPath.begin();

    while (src != mSearchPath.end()) 
    {
        if (*src == path) return true;
        src++;
    }
    mSearchPath.emplace_back(path);

    return true;
}

bool OGL_AssetLoader::RemoveSearchPath(const char* path)
{
    auto src = mSearchPath.begin();

    while (src != mSearchPath.end()) 
    {
        if (*src == path) {
            mSearchPath.erase(src);
            return true;
        }
        src++;
    }

    return true;
}

void OGL_AssetLoader::ClearSearchPath()
{
    mSearchPath.clear();
}

std::string OGL_AssetLoader::GetFileRealPath(const char* filePath)
{
    return std::string();
}

bool OGL_AssetLoader::FileExists(const char* filePath)
{
    AssetFilePtr fp = OpenFile(filePath, OPEN_BINARY);
    if (fp != nullptr) 
    {
        CloseFile(fp);
        return true;
    }
    return false;
}

OGL_IAssetLoader::AssetFilePtr OGL_AssetLoader::OpenFile(const char* name, AssetOpenMode mode)
{
    FILE* fp = nullptr;
    std::string upPath(mTargetPath);
    
    std::string fullPath;
    for (int32_t i = 0; i < 10; i++) 
    {
        auto src = mSearchPath.begin();
        bool looping = true;
        while (looping) 
        {
            fullPath.assign(upPath);
            if (src != mSearchPath.end()) 
            {
                fullPath.append(*src);
                //fullPath.append("Asset/");
                src++;
            }
            else 
            {
                //fullPath.append("Asset/");
                looping = false;
            }
            fullPath.append(name);

            switch (mode) 
            {
            case OPEN_TEXT:
                fp = fopen(fullPath.c_str(), "r");
                break;
            case OPEN_BINARY:
                fp = fopen(fullPath.c_str(), "rb");
                break;
            }

            if (fp)
            {
                return (AssetFilePtr)fp;
            }
        }

        upPath.append("../");
    }

    return nullptr;
}

OGL_Buffer OGL_AssetLoader::SyncOpenAndReadText(const char* filePath)
{
    OGL_Buffer buff;
    AssetFilePtr fp = OpenFile(filePath, OPEN_TEXT);

    if (fp) 
    {
        size_t length = GetSize(fp);

        uint8_t* data = new uint8_t[length + 1];
        length = fread(data, 1, length, static_cast<FILE*>(fp));

        data[length] = '\0';
        buff.SetData(data, length + 1);

        CloseFile(fp);
    }

    return buff;
}

OGL_Buffer OGL_AssetLoader::SyncOpenAndReadBinary(const char* filePath)
{
    OGL_Buffer buff;
    AssetFilePtr fp = OpenFile(filePath, OPEN_BINARY);

    if (fp) 
    {
        size_t length = GetSize(fp);

        uint8_t* data = new uint8_t[length];
        fread(data, length, 1, static_cast<FILE*>(fp));

        buff.SetData(data, length);

        CloseFile(fp);
    }

    return buff;
}

size_t OGL_AssetLoader::SyncRead(const AssetFilePtr& fp, OGL_Buffer& buf)
{
    size_t sz;

    if (!fp) 
    {
        fprintf(stderr, "null file discriptor\n");
        return 0;
    }

    sz = fread(buf.GetData(), buf.GetDataSize(), 1, static_cast<FILE*>(fp));

    return sz;
}

void OGL_AssetLoader::CloseFile(AssetFilePtr& fp)
{
    fclose((FILE*)fp);
    fp = nullptr;
}

size_t OGL_AssetLoader::GetSize(const AssetFilePtr& fp)
{
    FILE* _fp = static_cast<FILE*>(fp);

    long pos = ftell(_fp);
    fseek(_fp, 0, SEEK_END);
    size_t length = ftell(_fp);
    fseek(_fp, pos, SEEK_SET);

    return length;
}

int32_t OGL_AssetLoader::Seek(AssetFilePtr fp, long offset, AssetSeekBase where)
{
    return fseek(static_cast<FILE*>(fp), offset, static_cast<int>(where));
}
