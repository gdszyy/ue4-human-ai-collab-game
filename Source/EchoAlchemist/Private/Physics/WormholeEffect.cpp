// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/WormholeEffect.h"

UWormholeEffect::UWormholeEffect()
{
	EffectType = EPhysicsEffectType::Wormhole;
}

void UWormholeEffect::Reset()
{
	Super::Reset();
	TeleportCooldowns.Empty();
}

void UWormholeEffect::ApplyEffectInternal(FMarbleState& Marble, float DeltaTime)
{
	// 更新冷却时间
	UpdateCooldowns(DeltaTime);

	// 检查是否在入口范围内
	if (!IsAtEntrance(Marble.Position))
	{
		return;
	}

	// 检查是否可以传送（冷却时间已过）
	if (!CanTeleport(Marble.ID))
	{
		return;
	}

	// 执行传送
	FVector OldPosition = Marble.Position;
	FVector OldVelocity = Marble.Velocity;

	// 传送位置
	Marble.Position = ExitPosition;

	// 修改速度
	if (bPreserveVelocityDirection)
	{
		// 保持原速度方向，只修改大小
		Marble.Velocity *= ExitVelocityMultiplier;
	}
	else
	{
		// 随机方向，保持速度大小
		float Speed = Marble.Velocity.Size() * ExitVelocityMultiplier;
		FVector RandomDirection = FMath::VRand();
		RandomDirection.Z = 0;  // 保持在XY平面
		RandomDirection.Normalize();
		Marble.Velocity = RandomDirection * Speed;
	}

	// 记录冷却时间
	TeleportCooldowns.Add(Marble.ID, TeleportCooldown);

	UE_LOG(LogTemp, Log, TEXT("[WormholeEffect] Teleported marble %s: %s -> %s, Velocity: %s -> %s"),
		*Marble.ID.ToString(),
		*OldPosition.ToString(),
		*Marble.Position.ToString(),
		*OldVelocity.ToString(),
		*Marble.Velocity.ToString());
}

bool UWormholeEffect::IsAtEntrance(FVector Position) const
{
	float Distance = FVector::Dist(Position, EntrancePosition);
	return Distance <= EntranceRadius;
}

bool UWormholeEffect::CanTeleport(const FGuid& MarbleID) const
{
	const float* CooldownPtr = TeleportCooldowns.Find(MarbleID);
	if (CooldownPtr == nullptr)
	{
		return true;  // 没有冷却记录，可以传送
	}

	return *CooldownPtr <= 0.0f;  // 冷却时间已过
}

void UWormholeEffect::UpdateCooldowns(float DeltaTime)
{
	// 更新所有冷却时间
	TArray<FGuid> ExpiredMarbles;

	for (auto& Pair : TeleportCooldowns)
	{
		Pair.Value -= DeltaTime;

		// 记录已过期的冷却
		if (Pair.Value <= 0.0f)
		{
			ExpiredMarbles.Add(Pair.Key);
		}
	}

	// 移除已过期的冷却记录
	for (const FGuid& MarbleID : ExpiredMarbles)
	{
		TeleportCooldowns.Remove(MarbleID);
	}
}
