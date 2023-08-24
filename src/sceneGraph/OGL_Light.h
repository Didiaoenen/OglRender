#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "sceneGraph/OGL_Component.h"

namespace OGL
{
class OGL_Light : public OGL_Component
{
public:
    OGL_Light() = default;
    virtual ~OGL_Light() = default;

public:
    glm::vec3 mUp{ 0.0f, 1.0f, 0.0f };
    glm::vec2 mSize{ 0.0f, 0.0f };
    glm::vec3 mPosition{ 0.0f, 0.0f, 0.0f };
    glm::vec3 mDirection{ 0.0f, 0.0f, 1.0f };
    glm::vec3 mColorDiffuse{ 1.0f, 1.0f, 1.0f };
    glm::vec3 mColorSpecular{ 1.0f, 1.0f, 1.0f };
    glm::vec3 mColorAmbient{ 1.0f, 1.0f, 1.0f };

    //LightType mType{ UNDEFINED };
    float mAngleInnerCone{ 360.0f };
    float mAngleOuterCone{ 360.0f };
    float mAttenuationLinear{ 1.0f };
    float mAttenuationConstant{ 0.0f };
    float mAttenuationQuadratic{ 0.0f };
};
}