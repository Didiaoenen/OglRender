#pragma once

#include <variant>

namespace Tools
{
	template<typename T>
	class Tools_ReferenceOrValue
	{
	public:
		Tools_ReferenceOrValue(std::reference_wrapper<T> pReference) : mData{ &pReference.get() }
		{
		}

		Tools_ReferenceOrValue(T pValue = T()) : mData{ pValue }
		{
		}

		void MakeReference(T& pReference)
		{
			mData = &pReference;
		}

		void MakeValue(T pValue = T())
		{
			mData = pValue;
		}

		operator T& ()
		{
			return Get();
		}

		Tools_ReferenceOrValue<T>& operator=(T pValue)
		{
			Set(pValue);
			return *this;
		}

		T& Get() const
		{
			if (auto pval = std::get_if<T>(&mData))
			{
				return *pval;
			}
			else
			{
				return *std::get<T*>(mData);
			}
		}

		void Set(T pValue)
		{
			if (auto pval = std::get_if<T>(&mData))
			{
				*pval = pValue;
			}
			else
			{
				*std::get<T*>(mData) = pValue;
			}
		}

	private:
		std::variant<T, T*> mData;
	};
}