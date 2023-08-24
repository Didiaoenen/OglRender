#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "base/OGL_Base.h"

namespace OGL
{
class OGL_Texture;

template <typename T>
struct OGL_ParameterValueMap 
{
    T Value = (T)0;
    Ref<OGL_Texture> ValueMap;

    OGL_ParameterValueMap() = default;
    ~OGL_ParameterValueMap() = default;

    explicit OGL_ParameterValueMap(const T value)
        : Value(value), ValueMap(nullptr) 
    {
    }

    explicit OGL_ParameterValueMap(Ref<OGL_Texture> value)
        : ValueMap(std::move(value)) 
    {
    }

    OGL_ParameterValueMap& operator=(const Ref<OGL_Texture>& rhs)
    {
        ValueMap = rhs;
        return *this;
    }

    OGL_ParameterValueMap& operator=(const T& value)
    {
        Value = value;
        return *this;
    }

    OGL_ParameterValueMap& operator=(Ref<OGL_Texture>&& rhs)
    {
        ValueMap.swap(rhs);
        return *this;
    }

    OGL_ParameterValueMap& operator=(T&& value)
    {
        Value.swap(value);
        return *this;
    }

};

using Color = OGL_ParameterValueMap<glm::vec4>;
using Normal = OGL_ParameterValueMap<glm::vec3>;
using Parameter = OGL_ParameterValueMap<float>;
}