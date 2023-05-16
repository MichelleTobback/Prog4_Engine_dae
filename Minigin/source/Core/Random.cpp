#include "Random.h"

#include <random>

namespace dae
{
	static std::random_device s_RandomDevice{};
}

uint64_t dae::Random::GetRandomUint64()
{
	static std::mt19937_64 sRandomEngine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> sUniformDist{};

	return sUniformDist(sRandomEngine);
}



