// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SceneManager.h"
#include "SceneManagerBase.generated.h"

/**
 * 场景管理器基类
 * 
 * 为下落式和环形场景管理器提供通用实现，减少重复代码。
 * 
 * 设计理念：
 * - 提取共同属性和行为
 * - 提供默认实现
 * - 保留子类扩展能力
 * 
 * 共同功能：
 * 1. 反弹系数管理
 * 2. 默认坐标转换（逻辑坐标 = 屏幕坐标）
 * 3. 通用的反弹计算辅助函数
 */
UCLASS(Abstract)
class ECHOALCHEMIST_API USceneManagerBase : public UObject, public ISceneManager
{
	GENERATED_BODY()

public:
	// ========== 构造函数 ==========
	
	USceneManagerBase();

	// ========== ISceneManager 接口默认实现 ==========
	
	/**
	 * 默认坐标转换：逻辑坐标 = 屏幕坐标
	 */
	virtual FVector WorldToScreen(const FVector& LogicalPosition) const override;
	
	/**
	 * 默认坐标转换：屏幕坐标 = 逻辑坐标
	 */
	virtual FVector ScreenToWorld(const FVector& ScreenPosition) const override;
	
	/**
	 * 获取场景类型（子类必须重写）
	 */
	virtual FString GetSceneType() const override { return TEXT("Base"); }
	
	/**
	 * 更新位置和速度（子类必须重写）
	 */
	virtual void Advance(FVector& Position, FVector& Velocity, float DeltaTime) override {}
	
	/**
	 * 获取发射向量（默认实现：水平发射）
	 */
	virtual FVector GetLaunchVector(const FVector& StartPosition, float TargetAngle) override;
	
	/**
	 * 检查是否超出边界（默认实现：始终返回false）
	 */
	virtual bool IsOutOfBounds(const FVector& Position) const override { return false; }
	
	/**
	 * 应用环境力（默认实现：无操作）
	 */
	virtual void ApplyEnvironmentalForces(FVector& Velocity, float DeltaTime) override {}
	
	/**
	 * 处理边界交互（默认实现：无操作）
	 */
	virtual void HandleBoundaryInteraction(FVector& Position, FVector& Velocity) override {}

protected:
	// ========== 共同属性 ==========
	
	/** 反弹系数（0-1，0=完全吸收，1=完全弹性） */
	UPROPERTY(BlueprintReadWrite, Category = "Scene")
	float BounceCoefficient;

	// ========== 辅助函数 ==========
	
	/**
	 * 计算反弹速度
	 * @param Velocity 当前速度
	 * @param Normal 碰撞法向量（归一化）
	 * @return 反弹后的速度
	 */
	FVector CalculateBounceVelocity(const FVector& Velocity, const FVector& Normal) const;
	
	/**
	 * 应用反弹系数
	 * @param Velocity 速度（输入/输出）
	 */
	void ApplyBounceCoefficient(FVector& Velocity) const;
};
