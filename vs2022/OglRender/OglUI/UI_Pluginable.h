#pragma once

#include <vector>

#include "UI_IPlugin.h"

namespace UI
{
	class UI_Pluginable
	{
	public:
		~UI_Pluginable()
		{
			RemoveAllPlugins();
		}

		template<typename T, typename... Args>
		T& AppPlugin(Args&&... pArgs)
		{
			static_assert(std::is_base_of<UI_IPlugin, T>::value, "T Should Derive From IPlugin");

			T* newPlugin = new T(std::forward<Args>(pArgs)...);
			mPlugins.push_back(newPlugin);
			return newPlugin;
		}

		template<typename T>
		T* GetPlugin()
		{
			static_assert(std::is_base_of<UI_IPlugin, T>::value, "T Should Derive From IPlugin");

			for (auto it = mPlugins.begin(); it != mPlugins.end(); ++it)
			{
				return dynamic_cast<T*>(*it);
			}
			return nullptr;
		}

		void ExecutePlugins()
		{
			for (auto& plugin : mPlugins)
			{
				plugin->Execute();
			}
		}

		void RemoveAllPlugins() 
		{
			for (auto& plugin : mPlugins)
			{
				delete plugin;
			}
			mPlugins.clear();
		}

	private:
		std::vector<UI_IPlugin*> mPlugins;
	};
}

