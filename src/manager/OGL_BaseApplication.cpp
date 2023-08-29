#include "interface/OGL_IRuntimeModule.h"
#include "OGL_BaseApplication.h"

using namespace OGL;

bool OGL_BaseApplication::Initialize()
{
	for (const auto& rmodule : mRuntimeModules)
	{
		if (!rmodule->Initialize())
		{
			return false;
		}
	}

    return true;
}

void OGL_BaseApplication::Finalize()
{
	for (const auto& rmodule : mRuntimeModules)
	{
		rmodule->Finalize();
	}

	mRuntimeModules.clear();
}

void OGL_BaseApplication::Tick(double dt)
{
	for (const auto& rmodule : mRuntimeModules)
	{
		rmodule->Tick(dt);
	}
}

void OGL_BaseApplication::RegisterRuntimeModule(OGL_IRuntimeModule* rmodule)
{
	rmodule->SetApp(this);
	mRuntimeModules.push_back(rmodule);
}
