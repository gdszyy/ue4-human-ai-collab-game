// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Physics/MarbleState.h"
#include "PhysicsEffectBase.generated.h"

/**
 * 物理效果类型枚举
 */
UENUM(BlueprintType)
enum class EPhysicsEffectType : uint8
{
	GravityField UMETA(DisplayName = "Gravity Field"),
	Wormhole UMETA(DisplayName = "Wormhole"),
	Split UMETA(DisplayName = "Split"),
	VelocityModifier UMETA(DisplayName = "Velocity Modifier"),
	ChainTrigger UMETA(DisplayName = "Chain Trigger"),
	Custom UMETA(DisplayName = "Custom")
};

/**
 * 物理效果基类
 * 
 * 所有特殊物理效果的抽象基类，定义统一的接口。
 * 
 * 继承此类来创建自定义物理效果：
 * 
 * ```cpp
 * UCLASS()
 * class UMyCustomEffect : public UPhysicsEffectBase
 * {
 *     GENERATED_BODY()
 * 
 * protected:
 *     virtual void ApplyEffectInternal(FMarbleState& Marble, float DeltaTime) override
 *     {
 *         // 实现自定义效果逻辑
 *     }
 * };
 * ```
 * 
 * 注意事项：
 * - 子类必须实现ApplyEffectInternal方法
 * - 使用Duration控制效果持续时间（-1表示永久）
 * - 使用bIsActive控制效果启用状态
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class ECHOALCHEMIST_API UPhysicsEffectBase : public UObject
{
	GENERATED_BODY()

public:
	UPhysicsEffectBase();

	// ========== 核心接口 ==========
	
	/**
	 * 应用效果到魔力露珠
	 * 
	 * @param Marble 魔力露珠状态（会被修改）
	 * @param DeltaTime 时间增量（单位：秒）
	 * 
	 * 注意事项：
	 * - 此方法会自动更新持续时间
	 * - 如果效果未激活，不会应用效果
	 * - 子类应该实现ApplyEffectInternal而不是重写此方法
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	void ApplyEffect(UPARAM(ref) FMarbleState& Marble, float DeltaTime);

	/**
	 * 判断效果是否应该移除
	 * 
	 * @return true=应该移除，false=继续保留
	 * 
	 * 移除条件：
	 * - 持续时间已到期（Duration >= 0 且 ElapsedTime >= Duration）
	 * - 效果未激活（bIsActive = false）
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	bool ShouldRemove() const;

	/**
	 * 重置效果到初始状态
	 * 
	 * 用于对象池复用效果对象。
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	virtual void Reset();

	// ========== 效果属性 ==========
	
	/**
	 * 获取效果类型
	 * 
	 * @return 效果类型枚举
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	EPhysicsEffectType GetEffectType() const { return EffectType; }

	/**
	 * 获取效果唯一标识符
	 * 
	 * @return 效果ID
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	FGuid GetEffectID() const { return EffectID; }

	/**
	 * 获取已经过时间
	 * 
	 * @return 已经过时间（单位：秒）
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	float GetElapsedTime() const { return ElapsedTime; }

	/**
	 * 获取剩余时间
	 * 
	 * @return 剩余时间（单位：秒），-1表示永久
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	float GetRemainingTime() const;

	/**
	 * 设置效果激活状态
	 * 
	 * @param bActive true=激活，false=禁用
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	void SetActive(bool bActive) { bIsActive = bActive; }

	/**
	 * 检查效果是否激活
	 * 
	 * @return true=激活，false=禁用
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	bool IsActive() const { return bIsActive; }

protected:
	// ========== 子类实现 ==========
	
	/**
	 * 应用效果的内部实现（子类必须实现）
	 * 
	 * @param Marble 魔力露珠状态（会被修改）
	 * @param DeltaTime 时间增量（单位：秒）
	 * 
	 * 实现示例：
	 * ```cpp
	 * void UMyEffect::ApplyEffectInternal(FMarbleState& Marble, float DeltaTime)
	 * {
	 *     // 修改魔力露珠的速度
	 *     Marble.Velocity += FVector(0, 0, 100) * DeltaTime;
	 * }
	 * ```
	 */
	virtual void ApplyEffectInternal(FMarbleState& Marble, float DeltaTime);

	// ========== 效果配置 ==========
	
	/** 效果类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Config")
	EPhysicsEffectType EffectType = EPhysicsEffectType::Custom;

	/** 效果唯一标识符 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Config")
	FGuid EffectID;

	/** 持续时间（单位：秒，-1表示永久） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Config", meta = (ClampMin = "-1.0"))
	float Duration = -1.0f;

	/** 是否激活 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Config")
	bool bIsActive = true;

	/** 已经过时间（单位：秒） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect State")
	float ElapsedTime = 0.0f;
};
