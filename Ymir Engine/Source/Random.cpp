#include "Random.h"

#include "External/mmgr/mmgr.h"

unsigned int Random::Generate()
{
    std::uniform_int_distribution<int> distribution(1, std::numeric_limits<int>::max());

    std::random_device rd;
    std::mt19937 gen(rd());

    return distribution(gen);
}

float Random::GenerateRandomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}
