#pragma once

namespace OGL
{
struct OGL_GfxConfiguration;

class OGL_IApplication
{
public:
	virtual ~OGL_IApplication() = default;

	virtual bool IsQuit() const = 0;
	virtual void RequestQuit() = 0;

	virtual void* GetMainWindowHandler() = 0;

	[[nodiscard]] virtual const OGL_GfxConfiguration& GetConfiguration() const = 0;
};
}
