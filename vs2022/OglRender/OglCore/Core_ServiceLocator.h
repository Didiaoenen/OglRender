#pragma once

#include <any>
#include <unordered_map>

#define OVSERVICE(Type) Core::Core_ServiceLocator::Get<Type>()

namespace Core
{
	class Core_ServiceLocator
	{
	public:
		template<typename T>
		static void Provide(T& p_service)
		{
			__SERVICES[typeid(T).hash_code()] = std::any(&p_service);
		}

		template<typename T>
		static T& Get()
		{
			return *std::any_cast<T*>(__SERVICES[typeid(T).hash_code()]);
		}

	private:
		static std::unordered_map<size_t, std::any> __SERVICES;
	};
}

