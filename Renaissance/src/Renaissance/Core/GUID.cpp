#include "RenaissancePCH.h"
#include "Renaissance/Core/GUID.h"

#include <random>

namespace Renaissance
{
	static std::random_device sRandomDevice;
	static std::mt19937_64 sEngine(sRandomDevice());
	static std::uniform_int_distribution<uint64_t> sUniformDistribution;

	GUID::GUID()
		: mGuid(sUniformDistribution(sEngine))
	{}

	GUID::GUID(uint64_t id)
		: mGuid(id)
	{}
}