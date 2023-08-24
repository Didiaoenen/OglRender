#pragma once

#include <string>

#include "base/OGL_Base.h"
#include "interface/OGL_IRuntimeModule.h"

namespace OGL
{

class OGL_Scene;

class OGL_ISceneManager : public OGL_IRuntimeModule
{
public:
    OGL_ISceneManager() = default;
    virtual ~OGL_ISceneManager() = default;

    virtual bool LoadScene(const std::string& sceneName) = 0;

    virtual uint64_t GetSceneRevision() const = 0;

    virtual const Ref<OGL_Scene> GetSceneForRendering() const = 0;

    virtual void ResetScene() = 0;
};
}