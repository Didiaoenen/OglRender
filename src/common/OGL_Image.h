#pragma once

#include "common/OGL_Define.h"

namespace OGL
{
class OGL_Image
{
public:
    int width{ 0 };
    int height{ 0 };
    void* data{ nullptr };
    PixelFormat pixelFormat{ PixelFormat::UNKNOWN };
    ImageDataType imageDataType{ ImageDataType::UNKNOWN };
    ImageDataFormat imageDataFormat{ ImageDataFormat::UNKNOWN };
};
}

