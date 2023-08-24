#include "OGL_Camera.h"

using namespace OGL;

const glm::mat4& OGL_Camera::GetViewMatrix()
{
    return glm::identity<glm::mat4>();
}

const glm::mat4& OGL_Camera::GetProjectionMatrix()
{
    return glm::identity<glm::mat4>();
}

const glm::mat4& OGL_Camera::GetViewProjectionMatrix()
{
    return glm::identity<glm::mat4>();
}
