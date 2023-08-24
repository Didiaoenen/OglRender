#pragma once

#include "interface/OGL_IPass.h"

namespace OGL
{
class OGL_IDispatchPass : public OGL_IPass
{
public:
	virtual void Dispatch(Frame& frame) = 0;

};
}
