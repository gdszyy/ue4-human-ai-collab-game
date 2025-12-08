// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/GravityFieldEffect.h"

UGravityFieldEffect::UGravityFieldEffect()
{
	EffectType = EPhysicsEffectType::GravityField;
}

void UGravityFieldEffect::ApplyEffectInternal(FMarbleState& Marble, float DeltaTime)
{
	// 检查是否在引力场范围内
	if (!IsInRange(Marble.Position))
	{
		return;
	}

	// 计算引力加速度
	FVector Acceleration = CalculateGravityAcceleration(Marble.Position);

	// 应用加速度到速度
	Marble.Velocity += Acceleration * DeltaTime;

	UE_LOG(LogTemp, VeryVerbose, TEXT("[GravityFieldEffect] Applied gravity to marble %s: Acceleration=%s, NewVelocity=%s"),
		*Marble.ID.ToString(),
		*Acceleration.ToString(),
		*Marble.Velocity.ToString());
}

FVector UGravityFieldEffect::CalculateGravityAcceleration(FVector Position) const
{
	// 计算到引力场中心的向量
	FVector ToCenter = FieldCenter - Position;
	float Distance = ToCenter.Size();

	// 超出范围，无引力
	if (Distance > FieldRadius || Distance < KINDA_SMALL_NUMBER)
	{
		return FVector::ZeroVector;
	}

	// 归一化方向
	FVector Direction = ToCenter / Distance;

	// 计算引力强度
	float Strength;
	if (bUseInverseSquare)
	{
		// 平方反比定律：F = k / r²
		// 为避免距离为0时除零，添加一个小的偏移量
		float SafeDistance = FMath::Max(Distance, 10.0f);
		Strength = GravityStrength / (SafeDistance * SafeDistance);
	}
	else
	{
		// 线性衰减：F = k * (1 - r / R)
		float DistanceRatio = Distance / FieldRadius;
		Strength = GravityStrength * (1.0f - DistanceRatio);
	}

	// 返回引力加速度向量
	return Direction * Strength;
}

bool UGravityFieldEffect::IsInRange(FVector Position) const
{
	float Distance = FVector::Dist(Position, FieldCenter);
	return Distance <= FieldRadius;
}
