#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "sceneGraph/OGL_Component.h"
#include "common/OGL_Define.h"

namespace OGL
{
class OGL_Camera : public OGL_Component
{
public:
    OGL_Camera() = default;
    virtual ~OGL_Camera() = default;

    const glm::mat4& GetViewMatrix();
    const glm::mat4& GetProjectionMatrix();
    const glm::mat4& GetViewProjectionMatrix();

public:
    CameraType mCameraType{ CameraType::NONE };

    glm::mat4 mViewMatrix{ glm::identity<glm::mat4>() };
    glm::mat4 mProjectionMatrix{ glm::identity<glm::mat4>() };
    glm::mat4 mViewProjectionMatrix{ glm::identity<glm::mat4>() };

    glm::vec3 mUp{ 0.0f, 1.0f, 0.0f };
    glm::vec3 mLookAt{ 0.0f, 0.0f, -1.0f };
    glm::vec3 mPosition{ 0.0f, 0.0f, 0.0f };

    float mAspect{ 0.0f };
    float mHorizontalFOV{ 60.0f };
    float mOrthographicWidth{ 0.0f };
    float mOrthographicHeight{ 0.0f };
    float mClipPlaneNear{ 0.1f };
    float mClipPlaneFar{ 100.0f };
};
}