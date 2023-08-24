#pragma once

#include <string>

#include "interface/OGL_IRuntimeModule.h"

namespace OGL
{

class OGL_Buffer;

class OGL_IAssetLoader : public OGL_IRuntimeModule
{
public:
    OGL_IAssetLoader() = default;
    virtual ~OGL_IAssetLoader() = default;
    using AssetFilePtr = void*;

    enum AssetOpenMode 
    {
        OPEN_TEXT = 0,    
        OPEN_BINARY = 1,  
    };

    enum AssetSeekBase 
    {
        SET = 0,
        CUR = 1,
        END = 2,
    };

    virtual bool AddSearchPath(const char* path) = 0;

    virtual bool RemoveSearchPath(const char* path) = 0;

    virtual void ClearSearchPath() = 0;

    virtual bool FileExists(const char* filePath) = 0;

    virtual AssetFilePtr OpenFile(const char* name, AssetOpenMode mode) = 0;

    virtual OGL_Buffer SyncOpenAndReadText(const char* filePath) = 0;

    virtual OGL_Buffer SyncOpenAndReadBinary(const char* filePath) = 0;

    virtual std::string SyncOpenAndReadTextFileToString(const char* fileName) = 0;

    virtual size_t SyncRead(const AssetFilePtr& fp, OGL_Buffer& buf) = 0;

    virtual void CloseFile(AssetFilePtr& fp) = 0;

    virtual size_t GetSize(const AssetFilePtr& fp) = 0;

    virtual int32_t Seek(AssetFilePtr fp, long offset, AssetSeekBase where) = 0;
};
}
