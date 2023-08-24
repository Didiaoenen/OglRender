#include <iostream>
#include <glad/glad.h>

#include "interface/OGL_IApplication.h"
#include "OGL_OpenGLGraphicsManager.h"

using namespace OGL;

bool OGL_OpenGLGraphicsManager::Initialize()
{
    if (mInitialize)
    {
        return true;
    }

    OGL_OpenGLGraphicsCommonBaseManager::Initialize();

    if (!gladLoadGL())
    {
        std::cerr << "OpenGL load failed!" << std::endl;
        return false;
    }

    std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " loaded" << std::endl;

    auto conf = mApp->GetConfiguration();
    glViewport(0, 0, conf.screenWidth, conf.screenHeight);

    if (conf.msaaSamples) 
    {
        glEnable(GL_MULTISAMPLE);
    }

    return true;
}

void OGL_OpenGLGraphicsManager::Finalize()
{
    OGL_OpenGLGraphicsCommonBaseManager::Finalize();
}

void OGL_OpenGLGraphicsManager::CreateTextureView(Texture2D& textureView, const TextureArrayBase& textureArray, const uint32_t slice, const uint32_t mip)
{
}

void OGL_OpenGLGraphicsManager::BeginPass(Frame& frame)
{
    if (frame.renderToTexture && false)
    {
        GLuint framebuffer;
        glGenFramebuffers(1, &framebuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        if (frame.enableMSAA)
        {
            if (frame.colorTextures[1].handler)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, (GLuint)frame.colorTextures[1].handler, 0);
            }
        }
        else 
        {
            if (frame.colorTextures[0].handler) 
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)frame.colorTextures[0].handler, 0);
            }
        }

        if (frame.depthTexture.handler)
        {
            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frame.depthTexture.handler, 0);
        }

        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) 
        {
            assert(0);
        }

        frame.frameBuffer = framebuffer;
    }
    else
    {
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glViewport(0, 0, mCanvasWidth, mCanvasHeight);

    if (frame.clearRT)
    {
        glClearColor(frame.clearColor[0], frame.clearColor[1], frame.clearColor[2], frame.clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void OGL::OGL_OpenGLGraphicsManager::EndPass(Frame& frame)
{
    if (frame.renderToTexture)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        GLuint framebuffer = (GLuint)frame.frameBuffer;
        glDeleteFramebuffers(1, &framebuffer);

        frame.frameBuffer = 0;
    }
}

void OGL_OpenGLGraphicsManager::GetOpenGLTextureFormat(const PixelFormat pixelFormat, uint32_t& format, uint32_t& internalFormat, uint32_t& type)
{
    switch (pixelFormat) 
    {
    case PixelFormat::R8:
        format = GL_RED;
        internalFormat = GL_R8;
        type = GL_UNSIGNED_BYTE;
        break;
    case PixelFormat::RGB8:
        format = GL_RGB;
        internalFormat = GL_RGB8;
        type = GL_UNSIGNED_BYTE;
        break;
    case PixelFormat::RGBA8:
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
        type = GL_UNSIGNED_BYTE;
        break;
    case PixelFormat::DEPTH:
        format = GL_DEPTH_COMPONENT;
        internalFormat = GL_DEPTH_COMPONENT;
        type = GL_FLOAT;
        break;
    default:
        break;
    }
}

void OGL_OpenGLGraphicsManager::GetOpenGLTextureFormat(const CompressedFormat compressedFormat, uint32_t& format, uint32_t& internalFormat, uint32_t& type)
{
}

void OGL_OpenGLGraphicsManager::BeginFrame(Frame& frame)
{
    OGL_OpenGLGraphicsCommonBaseManager::BeginFrame(frame);
}

void OGL_OpenGLGraphicsManager::EndFrame(Frame& frame)
{
    OGL_OpenGLGraphicsCommonBaseManager::EndFrame(frame);
}