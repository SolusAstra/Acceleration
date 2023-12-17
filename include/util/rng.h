#pragma once
#include <random>   

__forceinline int getRandomAxis() {
    // Static generator to avoid reseeding
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> randInt(0, 3);
    return (int) randInt(gen);
}