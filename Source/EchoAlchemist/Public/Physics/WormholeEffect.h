// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Physics/PhysicsEffectBase.h"
#include "WormholeEffect.generated.h"

/**
 * 虫洞传送效果
 * 
 * 在两个位置之间创建虫洞，魔力露珠进入入口后传送到出口。
 * 
 * 蓝图使用示例：
 * 
 * ```
 * // 创建虫洞效果
 * UWormholeEffect* WormholeEffect = NewObject<UWormholeEffect>();
 * WormholeEffect->EntrancePosition = FVector(-500, 0, 100);
 * WormholeEffect->ExitPosition = FVector(500, 0, 100);
 * WormholeEffect->EntranceRadius = 50.0f;
 * WormholeEffect->ExitVelocityMultiplier = 1.5f;
 * WormholeEffect->Duration = -1;  // 永久
 * 
 * // 添加到效果管理器
 * EffectManager->AddEffect(WormholeEffect);
 * ```
 * 
 * 传送逻辑：
 * ```
 * if (Distance(MarblePosition, EntrancePosition) < EntranceRadius)
 * {
 *     MarblePosition = ExitPosition
 *     MarbleVelocity *= ExitVelocityMultiplier
 * }
 * ```
 * 
 * 注意事项：
 * - 使用冷却时间防止重复传送
 * - 传送后速度可以被放大或缩小
 * - 可以设置传送后的速度方向
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API UWormholeEffect : public UPhysicsEffectBase
{
	GENERATED_BODY()

public:
	UWormholeEffect();

	virtual void Reset() override;

	// ========== 配置参数 ==========
	
	/** 入口位置（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole")
	FVector EntrancePosition = FVector::ZeroVector;

	/** 出口位置（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole")
	FVector ExitPosition = FVector::ZeroVector;

	/** 入口半径（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole", meta = (ClampMin = "0.0"))
	float EntranceRadius = 50.0f;

	/** 出口速度倍数（1.0=保持原速度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole", meta = (ClampMin = "0.0"))
	float ExitVelocityMultiplier = 1.0f;

	/** 是否保持速度方向（false=随机方向） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole")
	bool bPreserveVelocityDirection = true;

	/** 传送冷却时间（单位：秒，防止重复传送） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole", meta = (ClampMin = "0.0"))
	float TeleportCooldown = 0.5f;

	// ========== 辅助方法 ==========
	
	/**
	 * 检查魔力露珠是否在入口范围内
	 * 
	 * @param Position 魔力露珠位置（单位：cm）
	 * @return true=在入口范围内，false=不在范围内
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects|Wormhole")
	bool IsAtEntrance(FVector Position) const;

	/**
	 * 检查魔力露珠是否可以被传送（冷却时间已过）
	 * 
	 * @param MarbleID 魔力露珠ID
	 * @return true=可以传送，false=冷却中
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Effects|Wormhole")
	bool CanTeleport(const FGuid& MarbleID) const;

protected:
	virtual void ApplyEffectInternal(FMarbleState& Marble, float DeltaTime) override;

private:
	/** 已传送的魔力露珠冷却时间映射表（MarbleID -> CooldownRemaining） */
	TMap<FGuid, float> TeleportCooldowns;

	/**
	 * 更新冷却时间
	 * 
	 * @param DeltaTime 时间增量（单位：秒）
	 */
	void UpdateCooldowns(float DeltaTime);
};
