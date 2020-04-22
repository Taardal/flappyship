#include "Random.h"


namespace storytime
{
    std::mt19937 Random::randomEngine;
    std::uniform_int_distribution<std::mt19937::result_type> Random::distribution;

    void Random::Init()
    {
        randomEngine.seed(std::random_device()());
    }

    float Random::Float()
    {
        return (float) distribution(randomEngine) / (float) std::numeric_limits<uint32_t>::max();
    }
}
