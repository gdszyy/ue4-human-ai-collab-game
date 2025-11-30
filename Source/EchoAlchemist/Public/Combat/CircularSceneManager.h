// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneManager.h"
#include "CircularSceneManager.generated.h"

/**
 * 环形场景管理器
 * 处理极坐标系的战斗场景
 * 
 * 特点：
 * - 极坐标系（r, θ）
 * - 环形边界（内半径、外半径）
 * - 敌人在圆环上移动
 * - 魔药在圆环内飞行
 * 
 * 坐标系说明：
 * - 极坐标：(r, θ)，r为半径，θ为角度（弧度）
 * - 笛卡尔坐标：(x, y)，x = r * cos(θ)，y = r * sin(θ)
 * - 角度范围：[0, 2π)
 * - 半径范围：[InnerRadius, OuterRadius]
 */
UCLASS(Blueprintable)
class ECHOALCHEMIST_API UCircularSceneManager : public UObject, public ISceneManager
{
	GENERATED_BODY()

public:
	// ========== 构造函数 ==========
	
	UCircularSceneManager();

	// ========== 初始化 ==========
	
	/**
	 * 初始化场景
	 * @param InInnerRadius 内半径（cm）
	 * @param InOuterRadius 外半径（cm）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Scene")
	void Initialize(float InInnerRadius, float InOuterRadius);

	/**
	 * 设置中心位置
	 * @param InCenter 中心位置（世界坐标）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Scene")
	void SetCenter(FVector InCenter);

	// ========== ISceneManager接口实现 ==========
	
	virtual FString GetSceneType() const override { return TEXT("Circular"); }
	
	virtual void Advance(FVector& Position, FVector& Velocity, float DeltaTime) override;
	
	virtual FVector GetLaunchVector(const FVector& StartPosition, float TargetAngle) override;
	
	virtual bool IsOutOfBounds(const FVector& Position) const override;
	
	virtual void ApplyEnvironmentalForces(FVector& Velocity, float DeltaTime) override;
	
	virtual void HandleBoundaryInteraction(FVector& Position, FVector& Velocity) override;
	
	virtual FVector WorldToScreen(const FVector& LogicalPosition) const override;
	
	virtual FVector ScreenToWorld(const FVector& ScreenPosition) const override;

	// ========== 极坐标转换 ==========
	
	/**
	 * 笛卡尔坐标转极坐标
	 * @param CartesianPosition 笛卡尔坐标（相对于中心）
	 * @param OutRadius 半径（输出）
	 * @param OutAngle 角度（弧度，输出）
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Scene")
	void CartesianToPolar(const FVector& CartesianPosition, float& OutRadius, float& OutAngle) const;

	/**
	 * 极坐标转笛卡尔坐标
	 * @param Radius 半径
	 * @param Angle 角度（弧度）
	 * @return 笛卡尔坐标（相对于中心）
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Scene")
	FVector PolarToCartesian(float Radius, float Angle) const;

	// ========== 敌人位置管理 ==========
	
	/**
	 * 获取敌人在圆环上的位置
	 * @param Angle 角度（弧度）
	 * @return 世界坐标
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Scene")
	FVector GetEnemyPosition(float Angle) const;

	/**
	 * 更新敌人角度（沿圆环移动）
	 * @param CurrentAngle 当前角度（弧度）
	 * @param AngularVelocity 角速度（弧度/秒）
	 * @param DeltaTime 时间增量（秒）
	 * @return 新角度（弧度）
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Scene")
	float UpdateEnemyAngle(float CurrentAngle, float AngularVelocity, float DeltaTime) const;

	// ========== 辅助方法 ==========
	
	/**
	 * 归一化角度到[0, 2π)
	 * @param Angle 角度（弧度）
	 * @return 归一化后的角度
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Scene")
	static float NormalizeAngle(float Angle);

	/**
	 * 计算两个角度之间的最短角度差
	 * @param Angle1 角度1（弧度）
	 * @param Angle2 角度2（弧度）
	 * @return 角度差（弧度，范围[-π, π]）
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Scene")
	static float AngleDifference(float Angle1, float Angle2);

protected:
	// ========== 场景参数 ==========
	
	/** 中心位置（世界坐标） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	FVector Center = FVector::ZeroVector;

	/** 内半径（cm） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	float InnerRadius = 300.0f;

	/** 外半径（cm） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	float OuterRadius = 500.0f;

	/** 敌人半径（敌人在圆环上的位置） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	float EnemyRadius = 400.0f;

	/** 边界反弹系数 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	float BounceCoefficient = 0.8f;
};
