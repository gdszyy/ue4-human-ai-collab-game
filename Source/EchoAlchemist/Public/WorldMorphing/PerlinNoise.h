// Copyright Epic Games, Inc. All Rights Reserved.
// Perlin噪声生成器

#pragma once

#include "CoreMinimal.h"

/**
 * Perlin噪声生成器
 * 用于地幔层的域扭曲效果
 */
class FPerlinNoise
{
public:
	FPerlinNoise();

	/**
	 * 生成2D Perlin噪声
	 * @param X X坐标
	 * @param Y Y坐标
	 * @return 噪声值 [0, 1]
	 */
	float Generate(float X, float Y) const;

	/**
	 * 生成多层Perlin噪声(Fractal Brownian Motion)
	 * @param X X坐标
	 * @param Y Y坐标
	 * @param Octaves 层数
	 * @param Persistence 持续度
	 * @return 噪声值 [0, 1]
	 */
	float GenerateFBM(float X, float Y, int32 Octaves = 4, float Persistence = 0.5f) const;

private:
	// 置换表
	TArray<int32> Permutation;

	// 辅助函数
	float Fade(float T) const;
	float Lerp(float T, float A, float B) const;
	float Grad(int32 Hash, float X, float Y) const;
};
