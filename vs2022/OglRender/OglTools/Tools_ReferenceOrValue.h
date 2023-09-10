#pragma once

#include <variant>

namespace Tools
{
	template<typename T>
	class Tools_ReferenceOrValue
	{
	public:
		Tools_ReferenceOrValue(std::reference_wrapper<T> p_reference) : m_data{ &p_reference.get() }
		{
		}

		Tools_ReferenceOrValue(T p_value = T()) : m_data{ p_value }
		{
		}

		void MakeReference(T& p_reference)
		{
			m_data = &p_reference;
		}

		void MakeValue(T p_value = T())
		{
			m_data = p_value;
		}

		operator T& ()
		{
			return Get();
		}

		Tools_ReferenceOrValue<T>& operator=(T p_value)
		{
			Set(p_value);
			return *this;
		}

		T& Get() const
		{
			if (auto pval = std::get_if<T>(&m_data))
				return *pval;
			else
				return *std::get<T*>(m_data);
		}

		void Set(T p_value)
		{
			if (auto pval = std::get_if<T>(&m_data))
				*pval = p_value;
			else
				*std::get<T*>(m_data) = p_value;
		}

	private:
		std::variant<T, T*> m_data;
	};
}