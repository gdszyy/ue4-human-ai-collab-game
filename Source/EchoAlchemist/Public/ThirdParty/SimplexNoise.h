/*
 * OpenSimplex (Simplectic) Noise in C++
 * by Arthur Tombs
 */

#pragma once

#include "CoreMinimal.h"

class USimplexNoise
{
public:
    static float noise2D(float x, float y);
    static void setSeed(int32 seed);
};
