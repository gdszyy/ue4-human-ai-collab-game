// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/PhysicsEffectBase.h"

UPhysicsEffectBase::UPhysicsEffectBase()
{
	// 生成唯一ID
	EffectID = FGuid::NewGuid();
}

void UPhysicsEffectBase::ApplyEffect(FMarbleState& Marble, float DeltaTime)
{
	// 检查效果是否激活
	if (!bIsActive)
	{
		return;
	}

	// 更新已经过时间
	ElapsedTime += DeltaTime;

	// 应用效果
	ApplyEffectInternal(Marble, DeltaTime);
}

bool UPhysicsEffectBase::ShouldRemove() const
{
	// 未激活的效果应该移除
	if (!bIsActive)
	{
		return true;
	}

	// 持续时间已到期
	if (Duration >= 0.0f && ElapsedTime >= Duration)
	{
		return true;
	}

	return false;
}

void UPhysicsEffectBase::Reset()
{
	ElapsedTime = 0.0f;
	bIsActive = true;
	EffectID = FGuid::NewGuid();
}

float UPhysicsEffectBase::GetRemainingTime() const
{
	if (Duration < 0.0f)
	{
		return -1.0f;  // 永久效果
	}

	return FMath::Max(0.0f, Duration - ElapsedTime);
}

void UPhysicsEffectBase::ApplyEffectInternal(FMarbleState& Marble, float DeltaTime)
{
	// 基类不实现任何效果，由子类重写
	UE_LOG(LogTemp, Warning, TEXT("[PhysicsEffectBase] ApplyEffectInternal not implemented in base class"));
}
