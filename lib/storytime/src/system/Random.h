#pragma once

#include <random>

namespace storytime
{
    class Random
    {
    private:
        static std::mt19937 randomEngine;
        static std::uniform_int_distribution<std::mt19937::result_type> distribution;

    public:
        static void Init();

        static float Float();

    };
}


