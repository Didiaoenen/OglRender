#pragma once

#include <vector>
#include <typeinfo> 

#include "manager/OGL_GraphicsManager.h"
#include "interface/OGL_IApplication.h"

namespace OGL
{

class OGL_IGameLogic;
class OGL_IRuntimeModule;

class OGL_BaseApplication : public OGL_IApplication
{
public:
	OGL_BaseApplication() = default;
	~OGL_BaseApplication() override = default;

	virtual bool Initialize();
	virtual void Finalize();

	virtual void Tick(double dt);

	virtual void* GetMainWindowHandler() override { return nullptr; }

	[[nodiscard]] const OGL_GfxConfiguration& GetConfiguration() const override { return mConfig; }

	[[nodiscard]] bool IsQuit() const override { return mQuit; }

	void RequestQuit() override { mQuit = true; }

	void RegisterRuntimeModule(OGL_IRuntimeModule* rmodule);

	template<typename T>
	T* GetModule();

protected:
	bool mQuit{ false };
	OGL_GfxConfiguration mConfig;

	OGL_IGameLogic* mGameLogic{ nullptr };

private:
	std::vector<OGL_IRuntimeModule*> mRuntimeModules;
};

template<typename T>
inline T* OGL_BaseApplication::GetModule()
{
	for (auto rmodule : mRuntimeModules)
	{
		auto ptr = dynamic_cast<T*>(rmodule);
		if (ptr)
		{
			return ptr;
		}
	}

	return nullptr;
}
}

