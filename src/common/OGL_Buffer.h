#pragma once

#include <stdint.h>

namespace OGL 
{
class OGL_Buffer 
{
public:
    OGL_Buffer() = default;

    explicit OGL_Buffer(size_t size, size_t alignment = 4) 
        : mSize(size) 
    {
        mData = reinterpret_cast<uint8_t*>(new uint8_t[size]);
    }

    OGL_Buffer(const OGL_Buffer& rhs) = delete;

    OGL_Buffer(OGL_Buffer&& rhs) noexcept 
    {
        mData = rhs.mData;
        mSize = rhs.mSize;
        rhs.mData = nullptr;
        rhs.mSize = 0;
    }

    OGL_Buffer& operator=(const OGL_Buffer& rhs) = delete;

    OGL_Buffer& operator=(OGL_Buffer&& rhs) noexcept 
    {
        delete[] mData;
        mData = rhs.mData;
        mSize = rhs.mSize;
        rhs.mData = nullptr;
        rhs.mSize = 0;
        return *this;
    }

    ~OGL_Buffer() 
    {
        if (mData) 
        {
            delete[] mData;
            mData = nullptr;
        }
    }

    [[nodiscard]] uint8_t* GetData() { return mData; };
    [[nodiscard]] const uint8_t* GetData() const { return mData; };
    [[nodiscard]] size_t GetDataSize() const { return mSize; };
        
    uint8_t* MoveData() 
    {
        uint8_t* tmp = mData;
        mData = nullptr;
        mSize = 0;
        return tmp;
    }

    void SetData(uint8_t* data, size_t size) 
    {
        if (mData != nullptr) 
        {
            delete[] mData;
        }
        mData = data;
        mSize = size;
    }

protected:
    size_t mSize{ 0 };
    uint8_t* mData{ nullptr };
};
}
