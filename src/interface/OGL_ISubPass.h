#pragma once

namespace OGL
{
class OGL_ISubPass
{
public:
	virtual ~OGL_ISubPass() = default;

	virtual void BeginSubPass() = 0;
	virtual void EndSubPass() = 0;
};
}