#pragma once

#include <assert.h>

#include "Tools_IniFile.h"

namespace Tools
{
	template<typename T>
	inline T Tools_IniFile::Get(const std::string& pKey)
	{
		if constexpr (std::is_same<bool, T>::value)
		{
			if (!IsKeyExisting(pKey))
			{
				return false;
			}

			return StringToBoolean(mData[pKey]);
		}
		else if constexpr (std::is_same<std::string, T>::value)
		{
			if (!IsKeyExisting(pKey))
			{
				return std::string("NULL");
			}

			return mData[pKey];
		}
		else if constexpr (std::is_integral<T>::value)
		{
			if (!IsKeyExisting(pKey))
			{
				return static_cast<T>(0);
			}

			return static_cast<T>(std::atoi(mData[pKey].c_str()));
		}
		else if constexpr (std::is_floating_point<T>::value)
		{
			if (!IsKeyExisting(pKey))
			{
				return static_cast<T>(0.0f);
			}

			return static_cast<T>(std::atof(mData[pKey].c_str()));
		}
		else
		{
			static_assert(false, "The given type must be : bool, integral, floating point or string");
			return T();
		}
	}

	template<typename T>
	inline T Tools_IniFile::GetOrDefault(const std::string& pKey, T pDefault)
	{
		return IsKeyExisting(pKey) ? Get<T>(pKey) : pDefault;
	}

	template<typename T>
	inline bool Tools_IniFile::Set(const std::string& pKey, const T& pValue)
	{
		if (IsKeyExisting(pKey))
		{
			if constexpr (std::is_same<bool, T>::value)
			{
				mData[pKey] = pValue ? "true" : "false";
			}
			else if constexpr (std::is_same<std::string, T>::value)
			{
				mData[pKey] = pValue;
			}
			else if constexpr (std::is_integral<T>::value)
			{
				mData[pKey] = std::to_string(pValue);
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				mData[pKey] = std::to_string(pValue);
			}
			else
			{
				static_assert(false, "The given type must be : bool, integral, floating point or string");
			}

			return true;
		}

		return false;
	}

	template<typename T>
	inline bool Tools_IniFile::Add(const std::string& pKey, const T& pValue)
	{
		if (!IsKeyExisting(pKey))
		{
			if constexpr (std::is_same<bool, T>::value)
			{
				RegisterPair(pKey, pValue ? "true" : "false");
			}
			else if constexpr (std::is_same<std::string, T>::value)
			{
				RegisterPair(pKey, pValue);
			}
			else if constexpr (std::is_integral<T>::value)
			{
				RegisterPair(pKey, std::to_string(pValue));
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				RegisterPair(pKey, std::to_string(pValue));
			}
			else
			{
				static_assert(false, "The given type must be : bool, integral, floating point or std::string");
			}

			return true;
		}

		return false;
	}
}