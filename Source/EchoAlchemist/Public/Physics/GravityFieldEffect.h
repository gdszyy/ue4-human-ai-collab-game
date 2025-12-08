// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Physics/PhysicsEffectBase.h"
#include "GravityFieldEffect.generated.h"

/**
 * 引力奇点效果
 * 
 * 在指定位置创建引力场，吸引附近的魔力露珠。
 * 
 * 蓝图使用示例：
 * 
 * ```
 * // 创建引力场效果
 * UGravityFieldEffect* GravityEffect = NewObject<UGravityFieldEffect>();
 * GravityEffect->FieldCenter = FVector(0, 0, 100);
 * GravityEffect->FieldRadius = 500.0f;
 * GravityEffect->GravityStrength = 1000.0f;
 * GravityEffect->Duration = 5.0f;  // 持续5秒
 * 
 * // 添加到效果管理器
 * EffectManager->AddEffect(GravityEffect);
 * ```
 * 
 * 物理公式：
 * ```
 * F = GravityStrength * (1 - Distance / FieldRadius)
 * Direction = Normalize(FieldCenter - MarblePosition)
 * Acceleration = F * Direction
 * ```
 * 
 * 注意事项：
 * - 引力强度随距离线性衰减
 * - 超出半径范围的魔力露珠不受影响
 * - 引力强度为负值时变成斥力
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API UGravityFieldEffect : public UPhysicsEffectBase
{
	GENERATED_BODY()

public:
	UGravityFieldEffect();

	// ========== 配置参数 ==========
	
	/** 引力场中心位置（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Field")
	FVector FieldCenter = FVector::ZeroVector;

	/** 引力场作用半径（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Field", meta = (ClampMin = "0.0"))
	float FieldRadius = 500.0f;

	/** 引力强度（单位：cm/s²，负值为斥力） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Field")
	float GravityStrength = 1000.0f;

	/** 是否使用平方反比定律（更真实的物理效果） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Field")
	bool bUseInverseSquare = false;

	// ========== 辅助方法 ==========
	
	/**
	 * 计算指定位置的引力加速度
	 * 
	 * @param Position 位置（单位：cm）
	 * @return 引力加速度向量（单位：cm/s²）
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects|Gravity")
	FVector CalculateGravityAcceleration(FVector Position) const;

	/**
	 * 检查位置是否在引力场范围内
	 * 
	 * @param Position 位置（单位：cm）
	 * @return true=在范围内，false=不在范围内
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects|Gravity")
	bool IsInRange(FVector Position) const;

protected:
	virtual void ApplyEffectInternal(FMarbleState& Marble, float DeltaTime) override;
};
