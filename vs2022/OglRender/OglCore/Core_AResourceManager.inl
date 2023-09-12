#pragma once

#include <algorithm>

#include "Core_AResourceManager.h"

namespace Core
{
	template<typename T>
	inline T* Core_AResourceManager<T>::LoadResource(const std::string& pPath)
	{
		if (auto resource = GetResource(pPath, false); resource)
		{
			return resource;
		}
		else
		{
			auto newResource = CreateResource(pPath);
			if (newResource)
			{
				return RegisterResource(pPath, newResource);
			}
			else
			{
				return nullptr;
			}
		}
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnloadResource(const std::string& pPath)
	{
		if (auto resource = GetResource(pPath, false); resource)
		{
			DestroyResource(resource);
			UnregisterResource(pPath);
		}
	}

	template<typename T>
	inline bool Core_AResourceManager<T>::MoveResource(const std::string& p_previousPath, const std::string& p_newPath)
	{
		if (IsResourceRegistered(p_previousPath) && !IsResourceRegistered(p_newPath))
		{
			T* toMove = m_resources.at(p_previousPath);
			UnregisterResource(p_previousPath);
			RegisterResource(p_newPath, toMove);
			return true;
		}

		return false;
	}

	template<typename T>
	inline void Core_AResourceManager<T>::ReloadResource(const std::string& pPath)
	{
		if (auto resource = GetResource(pPath, false); resource)
		{
			ReloadResource(resource, pPath);
		}
	}

	template<typename T>
	inline bool Core_AResourceManager<T>::IsResourceRegistered(const std::string& pPath)
	{
		return m_resources.find(pPath) != m_resources.end();
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnloadResources()
	{
		for (auto& [key, value] : m_resources)
		{
			DestroyResource(value);
		}

		m_resources.clear();
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::RegisterResource(const std::string& pPath, T* pInstance)
	{
		if (auto resource = GetResource(pPath, false); resource)
		{
			DestroyResource(resource);
		}

		m_resources[pPath] = pInstance;

		return pInstance;
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnregisterResource(const std::string& pPath)
	{
		m_resources.erase(pPath);
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::GetResource(const std::string& pPath, bool p_tryToLoadIfNotFound)
	{
		if (auto resource = m_resources.find(pPath); resource != m_resources.end())
		{
			return resource->second;
		}
		else if (p_tryToLoadIfNotFound)
		{
			return LoadResource(pPath);
		}

		return nullptr;
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::operator[](const std::string& pPath)
	{
		return GetResource(pPath);
	}

	template<typename T>
	inline void Core_AResourceManager<T>::ProvideAssetPaths(const std::string& p_projectAssetsPath, const std::string& p_engineAssetsPath)
	{
		__PROJECT_ASSETS_PATH = p_projectAssetsPath;
		__ENGINE_ASSETS_PATH = p_engineAssetsPath;
	}

	template<typename T>
	inline std::unordered_map<std::string, T*>& Core_AResourceManager<T>::GetResources()
	{
		return m_resources;
	}

	template<typename T>
	inline std::string Core_AResourceManager<T>::GetRealPath(const std::string& pPath) const
	{
		std::string result;

		if (pPath[0] == ':')
		{
			result = __ENGINE_ASSETS_PATH + std::string(pPath.data() + 1, pPath.data() + pPath.size());
		}
		else
		{
			result = __PROJECT_ASSETS_PATH + pPath;
		}

		return result;
	}
}