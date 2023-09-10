#pragma once

#include <algorithm>

#include "Core_AResourceManager.h"

namespace Core
{
	template<typename T>
	inline T* Core_AResourceManager<T>::LoadResource(const std::string& p_path)
	{
		if (auto resource = GetResource(p_path, false); resource)
			return resource;
		else
		{
			auto newResource = CreateResource(p_path);
			if (newResource)
				return RegisterResource(p_path, newResource);
			else
				return nullptr;
		}
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnloadResource(const std::string& p_path)
	{
		if (auto resource = GetResource(p_path, false); resource)
		{
			DestroyResource(resource);
			UnregisterResource(p_path);
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
	inline void Core_AResourceManager<T>::ReloadResource(const std::string& p_path)
	{
		if (auto resource = GetResource(p_path, false); resource)
		{
			ReloadResource(resource, p_path);
		}
	}

	template<typename T>
	inline bool Core_AResourceManager<T>::IsResourceRegistered(const std::string& p_path)
	{
		return m_resources.find(p_path) != m_resources.end();
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnloadResources()
	{
		for (auto& [key, value] : m_resources)
			DestroyResource(value);

		m_resources.clear();
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::RegisterResource(const std::string& p_path, T* p_instance)
	{
		if (auto resource = GetResource(p_path, false); resource)
			DestroyResource(resource);

		m_resources[p_path] = p_instance;

		return p_instance;
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnregisterResource(const std::string& p_path)
	{
		m_resources.erase(p_path);
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::GetResource(const std::string& p_path, bool p_tryToLoadIfNotFound)
	{
		if (auto resource = m_resources.find(p_path); resource != m_resources.end())
		{
			return resource->second;
		}
		else if (p_tryToLoadIfNotFound)
		{
			return LoadResource(p_path);
		}

		return nullptr;
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::operator[](const std::string& p_path)
	{
		return GetResource(p_path);
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
	inline std::string Core_AResourceManager<T>::GetRealPath(const std::string& p_path) const
	{
		std::string result;

		if (p_path[0] == ':')
		{
			result = __ENGINE_ASSETS_PATH + std::string(p_path.data() + 1, p_path.data() + p_path.size());
		}
		else
		{
			result = __PROJECT_ASSETS_PATH + p_path;
		}

		return result;
	}
}