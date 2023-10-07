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
	inline bool Core_AResourceManager<T>::MoveResource(const std::string& pPreviousPath, const std::string& pNewPath)
	{
		if (IsResourceRegistered(pPreviousPath) && !IsResourceRegistered(pNewPath))
		{
			T* toMove = mResources.at(pPreviousPath);
			UnregisterResource(pPreviousPath);
			RegisterResource(pNewPath, toMove);
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
		return mResources.find(pPath) != mResources.end();
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnloadResources()
	{
		for (auto& [key, value] : mResources)
		{
			DestroyResource(value);
		}

		mResources.clear();
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::RegisterResource(const std::string& pPath, T* pInstance)
	{
		if (auto resource = GetResource(pPath, false); resource)
		{
			DestroyResource(resource);
		}

		mResources[pPath] = pInstance;

		return pInstance;
	}

	template<typename T>
	inline void Core_AResourceManager<T>::UnregisterResource(const std::string& pPath)
	{
		mResources.erase(pPath);
	}

	template<typename T>
	inline T* Core_AResourceManager<T>::GetResource(const std::string& pPath, bool pTryToLoadIfNotFound)
	{
		if (auto resource = mResources.find(pPath); resource != mResources.end())
		{
			return resource->second;
		}
		else if (pTryToLoadIfNotFound)
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
	inline void Core_AResourceManager<T>::ProvideAssetPaths(const std::string& pProjectAssetsPath, const std::string& pEngineAssetsPath)
	{
		__PROJECT_ASSETS_PATH = pProjectAssetsPath;
		__ENGINE_ASSETS_PATH = pEngineAssetsPath;
	}

	template<typename T>
	inline std::unordered_map<std::string, T*>& Core_AResourceManager<T>::GetResources()
	{
		return mResources;
	}

	template<typename T>
	inline std::string Core_AResourceManager<T>::GetRealPath(const std::string& pPath)
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