#pragma once

#include <string>

#include "base/OGL_RefCounts.h"

namespace OGL
{
class OGL_Asset : public OGL_RefCounts
{
public:
	OGL_Asset() = default;
	virtual ~OGL_Asset() = default;

public:
	std::string mFileName{};
	bool mLoaded{ false };
};
}

