#pragma once

#include "Core_Material.h"

namespace Core
{
	template<typename T>
	inline void Core_Material::Set(const std::string pKey, const T& pValue)
	{
		if (HasShader())
		{
			if (m_uniformsData.find(pKey) != m_uniformsData.end())
				m_uniformsData[pKey] = std::any(pValue);
		}
		else
		{
			OVLOG_ERROR("Material Set failed: No attached shader");
		}
	}

	template<typename T>
	inline const T& Core_Material::Get(const std::string pKey)
	{
		if (m_uniformsData.find(pKey) != m_uniformsData.end())
			return T();
		else
			return std::any_cast<T>(m_uniformsData.at(pKey));
	}
}