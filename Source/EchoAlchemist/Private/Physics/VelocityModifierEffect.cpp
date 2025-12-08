// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/VelocityModifierEffect.h"

UVelocityModifierEffect::UVelocityModifierEffect()
{
	EffectType = EPhysicsEffectType::VelocityModifier;
}

void UVelocityModifierEffect::ApplyEffectInternal(FMarbleState& Marble, float DeltaTime)
{
	// 检查是否在影响区域内
	if (!IsInArea(Marble.Position))
	{
		// 离开区域，从已应用列表中移除
		AppliedMarbles.Remove(Marble.ID);
		return;
	}

	// 如果不是每帧应用，检查是否已经应用过
	if (!bApplyEveryFrame && AppliedMarbles.Contains(Marble.ID))
	{
		return;
	}

	// 应用速度修改
	FVector OldVelocity = Marble.Velocity;

	switch (ModifierType)
	{
	case EVelocityModifierType::Multiply:
		Marble.Velocity *= ModifierValue;
		break;

	case EVelocityModifierType::Add:
		Marble.Velocity += ModifierValue * DeltaTime;
		break;

	case EVelocityModifierType::Set:
		Marble.Velocity = ModifierValue;
		break;
	}

	// 记录已应用
	if (!bApplyEveryFrame)
	{
		AppliedMarbles.Add(Marble.ID);
	}

	UE_LOG(LogTemp, VeryVerbose, TEXT("[VelocityModifierEffect] Modified velocity of marble %s: %s -> %s"),
		*Marble.ID.ToString(),
		*OldVelocity.ToString(),
		*Marble.Velocity.ToString());
}

bool UVelocityModifierEffect::IsInArea(FVector Position) const
{
	switch (AffectArea)
	{
	case EAreaType::Sphere:
		{
			float Distance = FVector::Dist(Position, AreaCenter);
			return Distance <= AreaSize.X;  // AreaSize.X = 半径
		}

	case EAreaType::Box:
		{
			FBox Box(AreaCenter - AreaSize * 0.5f, AreaCenter + AreaSize * 0.5f);
			return Box.IsInside(Position);
		}

	case EAreaType::Cylinder:
		{
			// 检查XY平面距离
			FVector2D CenterXY(AreaCenter.X, AreaCenter.Y);
			FVector2D PositionXY(Position.X, Position.Y);
			float DistanceXY = FVector2D::Distance(CenterXY, PositionXY);

			// 检查Z轴高度
			float HeightDiff = FMath::Abs(Position.Z - AreaCenter.Z);

			return DistanceXY <= AreaSize.X && HeightDiff <= AreaSize.Z * 0.5f;
		}

	default:
		return false;
	}
}
