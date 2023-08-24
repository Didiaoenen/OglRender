#pragma once

namespace OGL
{

class OGL_IApplication;

class OGL_IRuntimeModule
{
public:
	OGL_IRuntimeModule() = default;
	virtual ~OGL_IRuntimeModule() = default;

	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;
	
	virtual void Tick() = 0;

	void SetApp(OGL_IApplication* pApp) { mApp = pApp; }

protected:
	OGL_IApplication* mApp;
};
}

