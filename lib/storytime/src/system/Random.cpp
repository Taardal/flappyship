#include "Random.h"


namespace storytime
{
    Random::Random()
            : generator(std::random_device()())
    {
    }

    uint32_t Random::Next()
    {
        std::uniform_int_distribution<uint32_t> distribution;
        return distribution(generator);
    }

    uint32_t Random::Next(uint32_t min, uint32_t max)
    {
        std::uniform_int_distribution<uint32_t> distribution(min, max);
        return distribution(generator);
    }

}
