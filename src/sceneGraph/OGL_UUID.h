#pragma once

#include <cstdint>
#include <cstddef>

namespace OGL
{
class OGL_UUID
{
public:
	OGL_UUID();
	OGL_UUID(uint64_t uuid);

	operator uint64_t() const { return mUUID; }
public:
	uint64_t mUUID{ 0 };

};

template<typename T> 
struct hash;

template<>
struct hash<OGL_UUID>
{
	std::size_t operator()(const OGL_UUID& uuid) const
	{
		return (uint64_t)uuid;
	}
};
}

