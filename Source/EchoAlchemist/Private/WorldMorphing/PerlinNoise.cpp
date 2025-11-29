// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldMorphing/PerlinNoise.h"

FPerlinNoise::FPerlinNoise()
{
	// 初始化置换表 (标准Perlin噪声置换)
	Permutation.SetNum(512);
	
	// 基础置换表 (0-255)
	TArray<int32> Base;
	Base.SetNum(256);
	for (int32 i = 0; i < 256; ++i)
	{
		Base[i] = i;
	}
	
	// 使用固定种子以保证可重复性(可以后续改为可配置)
	FRandomStream RandStream(12345);
	for (int32 i = 255; i > 0; --i)
	{
		int32 j = RandStream.RandRange(0, i);
		Swap(Base[i], Base[j]);
	}
	
	// 复制两次以避免边界检查
	for (int32 i = 0; i < 256; ++i)
	{
		Permutation[i] = Base[i];
		Permutation[256 + i] = Base[i];
	}
}

float FPerlinNoise::Generate(float X, float Y) const
{
	// 找到单元格坐标
	int32 Xi = FMath::FloorToInt(X) & 255;
	int32 Yi = FMath::FloorToInt(Y) & 255;
	
	// 单元格内相对坐标
	float Xf = X - FMath::FloorToFloat(X);
	float Yf = Y - FMath::FloorToFloat(Y);
	
	// 计算淡化曲线
	float U = Fade(Xf);
	float V = Fade(Yf);
	
	// 哈希坐标
	int32 A = Permutation[Xi] + Yi;
	int32 B = Permutation[Xi + 1] + Yi;
	
	// 插值计算
	float X1 = Lerp(U, Grad(Permutation[A], Xf, Yf), Grad(Permutation[B], Xf - 1, Yf));
	float X2 = Lerp(U, Grad(Permutation[A + 1], Xf, Yf - 1), Grad(Permutation[B + 1], Xf - 1, Yf - 1));
	
	float Result = Lerp(V, X1, X2);
	
	// 归一化到 [0, 1]
	return (Result + 1.0f) * 0.5f;
}

float FPerlinNoise::GenerateFBM(float X, float Y, int32 Octaves, float Persistence) const
{
	float Total = 0.0f;
	float Frequency = 1.0f;
	float Amplitude = 1.0f;
	float MaxValue = 0.0f;
	
	for (int32 i = 0; i < Octaves; ++i)
	{
		Total += Generate(X * Frequency, Y * Frequency) * Amplitude;
		MaxValue += Amplitude;
		Amplitude *= Persistence;
		Frequency *= 2.0f;
	}
	
	return Total / MaxValue;
}

float FPerlinNoise::Fade(float T) const
{
	// 6t^5 - 15t^4 + 10t^3
	return T * T * T * (T * (T * 6.0f - 15.0f) + 10.0f);
}

float FPerlinNoise::Lerp(float T, float A, float B) const
{
	return A + T * (B - A);
}

float FPerlinNoise::Grad(int32 Hash, float X, float Y) const
{
	// 将哈希值转换为梯度向量
	int32 H = Hash & 3;
	float U = (H < 2) ? X : Y;
	float V = (H < 2) ? Y : X;
	return ((H & 1) ? -U : U) + ((H & 2) ? -V : V);
}
