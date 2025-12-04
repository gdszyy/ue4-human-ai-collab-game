// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/SceneManagerBase.h"

USceneManagerBase::USceneManagerBase()
{
	BounceCoefficient = 0.8f;
}

FVector USceneManagerBase::WorldToScreen(const FVector& LogicalPosition) const
{
	// 默认实现：逻辑坐标 = 屏幕坐标
	return LogicalPosition;
}

FVector USceneManagerBase::ScreenToWorld(const FVector& ScreenPosition) const
{
	// 默认实现：屏幕坐标 = 逻辑坐标
	return ScreenPosition;
}

FVector USceneManagerBase::CalculateBounceVelocity(const FVector& Velocity, const FVector& Normal) const
{
	// 反弹公式：V' = V - 2 * (V · N) * N
	// 其中 V 是入射速度，N 是归一化的碰撞法向量
	return Velocity - 2.0f * FVector::DotProduct(Velocity, Normal) * Normal;
}

void USceneManagerBase::ApplyBounceCoefficient(FVector& Velocity) const
{
	Velocity *= BounceCoefficient;
}
