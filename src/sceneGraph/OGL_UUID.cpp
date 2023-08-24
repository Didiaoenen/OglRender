#include <random>

#include "OGL_UUID.h"

using namespace OGL;

static std::random_device sRandomDevice;
static std::mt19937_64 sEngine(sRandomDevice());
static std::uniform_int_distribution<uint64_t> sUniformDistribution;

OGL_UUID::OGL_UUID()
	: mUUID(sUniformDistribution(sEngine))
{
	
}

OGL_UUID::OGL_UUID(uint64_t uuid)
	: mUUID(uuid)
{

}
