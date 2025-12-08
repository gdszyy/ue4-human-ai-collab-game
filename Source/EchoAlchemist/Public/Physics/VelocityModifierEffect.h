// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Physics/PhysicsEffectBase.h"
#include "VelocityModifierEffect.generated.h"

/**
 * 速度修改类型
 */
UENUM(BlueprintType)
enum class EVelocityModifierType : uint8
{
	Multiply UMETA(DisplayName = "Multiply"),  // 乘法：Velocity *= Value
	Add UMETA(DisplayName = "Add"),            // 加法：Velocity += Value
	Set UMETA(DisplayName = "Set")             // 设置：Velocity = Value
};

/**
 * 影响区域类型
 */
UENUM(BlueprintType)
enum class EAreaType : uint8
{
	Sphere UMETA(DisplayName = "Sphere"),      // 球形区域
	Box UMETA(DisplayName = "Box"),            // 盒形区域
	Cylinder UMETA(DisplayName = "Cylinder")   // 圆柱形区域
};

/**
 * 速度修改效果
 * 
 * 修改魔力露珠的速度（加速/减速）。
 * 
 * 蓝图使用示例：
 * 
 * ```
 * // 创建加速区域
 * UVelocityModifierEffect* AcceleratorEffect = NewObject<UVelocityModifierEffect>();
 * AcceleratorEffect->ModifierType = EVelocityModifierType::Multiply;
 * AcceleratorEffect->ModifierValue = FVector(2.0f, 2.0f, 1.0f);  // XY方向2倍速
 * AcceleratorEffect->AffectArea = EAreaType::Sphere;
 * AcceleratorEffect->AreaCenter = FVector(0, 0, 100);
 * AcceleratorEffect->AreaSize = FVector(200, 200, 200);
 * 
 * // 添加到效果管理器
 * EffectManager->AddEffect(AcceleratorEffect);
 * ```
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API UVelocityModifierEffect : public UPhysicsEffectBase
{
	GENERATED_BODY()

public:
	UVelocityModifierEffect();

	// ========== 配置参数 ==========
	
	/** 修改类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity Modifier")
	EVelocityModifierType ModifierType = EVelocityModifierType::Multiply;

	/** 修改值（根据ModifierType不同含义不同） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity Modifier")
	FVector ModifierValue = FVector(1.0f, 1.0f, 1.0f);

	/** 影响区域类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity Modifier")
	EAreaType AffectArea = EAreaType::Sphere;

	/** 区域中心位置（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity Modifier")
	FVector AreaCenter = FVector::ZeroVector;

	/** 区域大小（单位：cm，Sphere=半径，Box=尺寸，Cylinder=半径+高度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity Modifier")
	FVector AreaSize = FVector(100.0f, 100.0f, 100.0f);

	/** 是否每帧应用（false=只在进入区域时应用一次） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity Modifier")
	bool bApplyEveryFrame = false;

	// ========== 辅助方法 ==========
	
	/**
	 * 检查位置是否在影响区域内
	 * 
	 * @param Position 位置（单位：cm）
	 * @return true=在区域内，false=不在区域内
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects|Velocity")
	bool IsInArea(FVector Position) const;

protected:
	virtual void ApplyEffectInternal(FMarbleState& Marble, float DeltaTime) override;

private:
	/** 已应用过的魔力露珠ID集合（用于bApplyEveryFrame=false时） */
	TSet<FGuid> AppliedMarbles;
};
