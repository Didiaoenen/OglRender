#pragma once

#include "Core_Material.h"

namespace Core
{
	template<typename T>
	inline void Core_Material::Set(const std::string pKey, const T& pValue)
	{
		if (HasShader())
		{
			if (mUniformsData.find(pKey) != mUniformsData.end())
			{
				mUniformsData[pKey] = std::any(pValue);
			}
		}
		else
		{
			//OVLOG_ERROR("Material Set failed: No attached shader");
		}
	}

	template<typename T>
	inline const T& Core_Material::Get(const std::string pKey)
	{
		if (mUniformsData.find(pKey) != mUniformsData.end())
		{
			return T();
		}
		else
		{
			return std::any_cast<T>(mUniformsData.at(pKey));
		}
	}
}