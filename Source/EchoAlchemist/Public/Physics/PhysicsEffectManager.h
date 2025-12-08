// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Physics/PhysicsEffectBase.h"
#include "PhysicsEffectManager.generated.h"

/**
 * 物理效果管理器
 * 
 * 管理所有活动的物理效果，负责效果的添加、移除和更新。
 * 
 * 蓝图使用示例：
 * 
 * ```
 * // 1. 创建效果管理器
 * EffectManager = NewObject<UPhysicsEffectManager>();
 * EffectManager->Initialize();
 * 
 * // 2. 添加效果
 * UGravityFieldEffect* GravityEffect = NewObject<UGravityFieldEffect>();
 * GravityEffect->FieldCenter = FVector(0, 0, 100);
 * GravityEffect->FieldRadius = 500.0f;
 * GravityEffect->GravityStrength = 1000.0f;
 * EffectManager->AddEffect(GravityEffect);
 * 
 * // 3. 每帧更新效果
 * TArray<FMarbleState> Marbles = PhysicsSystem->GetAllMarbles();
 * EffectManager->UpdateEffects(Marbles, DeltaTime);
 * 
 * // 4. 移除效果
 * EffectManager->RemoveEffect(GravityEffect->GetEffectID());
 * ```
 * 
 * 注意事项：
 * - 必须先调用Initialize才能使用
 * - UpdateEffects会自动移除过期的效果
 * - 效果按添加顺序应用
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API UPhysicsEffectManager : public UObject
{
	GENERATED_BODY()

public:
	// ========== 初始化和清理 ==========
	
	/**
	 * 初始化效果管理器
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	void Initialize();

	/**
	 * 清理效果管理器
	 * 
	 * 移除所有效果并重置状态。
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	void Cleanup();

	// ========== 效果管理 ==========
	
	/**
	 * 添加效果
	 * 
	 * @param Effect 要添加的效果
	 * @return 效果ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	FGuid AddEffect(UPhysicsEffectBase* Effect);

	/**
	 * 移除效果
	 * 
	 * @param EffectID 效果ID
	 * @return true=移除成功，false=效果不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	bool RemoveEffect(const FGuid& EffectID);

	/**
	 * 移除所有效果
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	void RemoveAllEffects();

	/**
	 * 获取效果
	 * 
	 * @param EffectID 效果ID
	 * @return 效果对象，如果不存在返回nullptr
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	UPhysicsEffectBase* GetEffect(const FGuid& EffectID) const;

	/**
	 * 获取所有效果
	 * 
	 * @return 所有效果的数组
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	TArray<UPhysicsEffectBase*> GetAllEffects() const;

	/**
	 * 获取指定类型的所有效果
	 * 
	 * @param EffectType 效果类型
	 * @return 指定类型的效果数组
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	TArray<UPhysicsEffectBase*> GetEffectsByType(EPhysicsEffectType EffectType) const;

	/**
	 * 获取效果数量
	 * 
	 * @return 当前活动的效果数量
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	int32 GetEffectCount() const { return ActiveEffects.Num(); }

	// ========== 效果更新 ==========
	
	/**
	 * 更新所有效果
	 * 
	 * @param Marbles 魔力露珠数组（会被修改）
	 * @param DeltaTime 时间增量（单位：秒）
	 * 
	 * 注意事项：
	 * - 会自动移除过期的效果
	 * - 效果按添加顺序应用
	 * - 建议每帧调用一次
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects")
	void UpdateEffects(UPARAM(ref) TArray<FMarbleState>& Marbles, float DeltaTime);

	// ========== 统计信息 ==========
	
	/**
	 * 获取效果统计信息
	 * 
	 * @param OutTotalEffects 总效果数
	 * @param OutActiveEffects 激活的效果数
	 * @param OutInactiveEffects 未激活的效果数
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Effects|Debug")
	void GetStatistics(int32& OutTotalEffects, int32& OutActiveEffects, int32& OutInactiveEffects) const;

	/**
	 * 检查管理器是否已初始化
	 * 
	 * @return true=已初始化，false=未初始化
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects")
	bool IsInitialized() const { return bIsInitialized; }

private:
	// ========== 内部状态 ==========
	
	/** 是否已初始化 */
	bool bIsInitialized = false;

	/** 活动效果列表 */
	UPROPERTY()
	TArray<UPhysicsEffectBase*> ActiveEffects;

	/** 效果映射表（EffectID -> Effect） */
	UPROPERTY()
	TMap<FGuid, UPhysicsEffectBase*> EffectMap;

	// ========== 内部辅助方法 ==========
	
	/**
	 * 移除过期的效果
	 * 
	 * @return 移除的效果数量
	 */
	int32 RemoveExpiredEffects();
};
