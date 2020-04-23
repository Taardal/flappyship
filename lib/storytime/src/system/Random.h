#pragma once

#include <random>

namespace storytime
{
    class Random
    {
        std::mt19937 generator;
    public:
        Random();

        uint32_t UInt(uint32_t min, uint32_t max);
    };
}


