#pragma once

#include <string>
#include <vector>

#include "common/OGL_Buffer.h"
#include "interface/OGL_IAssetLoader.h"

namespace OGL
{
class OGL_AssetLoader : public OGL_IAssetLoader
{
public:
	OGL_AssetLoader() = default;
	~OGL_AssetLoader() override = default;

    bool Initialize() override;
    void Finalize() override {}
    void Tick(double dt) override {}

    bool AddSearchPath(const char* path) override;

    bool RemoveSearchPath(const char* path) override;

    void ClearSearchPath() override;

    std::string GetFileRealPath(const char* filePath);

    bool FileExists(const char* filePath) override;

    AssetFilePtr OpenFile(const char* name, AssetOpenMode mode) override;

    OGL_Buffer SyncOpenAndReadText(const char* filePath) override;

    OGL_Buffer SyncOpenAndReadBinary(const char* filePath) override;

    size_t SyncRead(const AssetFilePtr& fp, OGL_Buffer& buf) override;

    void CloseFile(AssetFilePtr& fp) override;

    size_t GetSize(const AssetFilePtr& fp) override;

    int32_t Seek(AssetFilePtr fp, long offset, AssetSeekBase where) override;

    inline std::string SyncOpenAndReadTextFileToString(const char* fileName) override 
    {
        std::string result;
        OGL_Buffer buffer = SyncOpenAndReadText(fileName);
        if (buffer.GetDataSize()) 
        {
            char* content = reinterpret_cast<char*>(buffer.GetData());

            if (content) 
            {
                result = std::string(content);
            }
        }

        return result;
    }

protected:
    static std::string mTargetPath;

private:
    static std::vector<std::string> mSearchPath;
};
}

