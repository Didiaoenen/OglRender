#pragma once

namespace Render
{
	enum class ECullingOptions
	{
		NONE = 0x0,
		FRUSTUM_PER_MODEL = 0x1,
		FRUSTUM_PER_MESH = 0x2
	};

	inline ECullingOptions operator~ (ECullingOptions a) { return (ECullingOptions)~(int)a; }
	inline ECullingOptions operator| (ECullingOptions a, ECullingOptions b) { return (ECullingOptions)((int)a | (int)b); }
	inline ECullingOptions operator& (ECullingOptions a, ECullingOptions b) { return (ECullingOptions)((int)a & (int)b); }
	inline ECullingOptions operator^ (ECullingOptions a, ECullingOptions b) { return (ECullingOptions)((int)a ^ (int)b); }
	inline ECullingOptions& operator|= (ECullingOptions& a, ECullingOptions b) { return (ECullingOptions&)((int&)a |= (int)b); }
	inline ECullingOptions& operator&= (ECullingOptions& a, ECullingOptions b) { return (ECullingOptions&)((int&)a &= (int)b); }
	inline ECullingOptions& operator^= (ECullingOptions& a, ECullingOptions b) { return (ECullingOptions&)((int&)a ^= (int)b); }
	inline bool IsFlagSet(ECullingOptions p_flag, ECullingOptions pMask) { return (int)p_flag & (int)pMask; }
}