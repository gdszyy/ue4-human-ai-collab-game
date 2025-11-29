// Copyright Epic Games, Inc. All Rights Reserved.
// 世界变迁系统 - 核心子系统

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldMorphing/WorldMorphingTypes.h"
#include "WorldMorphing/PerlinNoise.h"
#include "WorldMorphingSubsystem.generated.h"

/**
 * 边缘供给点数据
 */
struct FEdgeSupplyPoint
{
	float Angle;
	float Speed;

	FEdgeSupplyPoint() : Angle(0.0f), Speed(0.0f) {}
	FEdgeSupplyPoint(float InAngle, float InSpeed) : Angle(InAngle), Speed(InSpeed) {}
};

/**
 * 世界变迁子系统
 * 管理整个世界网格的模拟更新
 */
UCLASS()
class ECHOALCHEMIST_API UWorldMorphingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Subsystem接口
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * 初始化世界网格
	 * @param Width 网格宽度
	 * @param Height 网格高度
	 * @param InitParams 初始参数
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing")
	void InitializeWorld(int32 Width, int32 Height, const FSimulationParams& InitParams);

	/**
	 * 更新模拟(每帧调用)
	 * @param DeltaTime 时间增量
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing")
	void TickSimulation(float DeltaTime);

	/**
	 * 获取指定位置的单元格状态
	 * @param X X坐标
	 * @param Y Y坐标
	 * @return 单元格状态
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing")
	FCellState GetCellAt(int32 X, int32 Y) const;

	/**
	 * 设置模拟参数
	 * @param NewParams 新参数
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing")
	void SetSimulationParams(const FSimulationParams& NewParams);

	/**
	 * 获取当前模拟参数
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing")
	FSimulationParams GetSimulationParams() const { return Params; }

	/**
	 * 获取网格尺寸
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing")
	void GetGridSize(int32& OutWidth, int32& OutHeight) const;

	/**
	 * 获取当前时间步
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing")
	int32 GetTimeStep() const { return TimeStep; }

	/**
	 * 获取周期计数
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing")
	int32 GetCycleCount() const { return CycleCount; }

private:
	// 网格数据
	TArray<TArray<FCell>> Grid;
	int32 Width;
	int32 Height;

	// 模拟状态
	int32 TimeStep;
	int32 CycleCount;
	FSimulationParams Params;

	// Perlin噪声生成器
	TUniquePtr<FPerlinNoise> PerlinNoise;

	// 噪声偏移(用于域扭曲)
	float NoiseOffsetX;
	float NoiseOffsetY;

	// 边缘供给点
	TArray<FEdgeSupplyPoint> EdgeSupplyPoints;

	// 更新各层
	void UpdateMantleLayer();
	void UpdateClimateLayer();
	void UpdateCrystalLayer();
	void UpdateHumanLayer();

	// 辅助函数
	TArray<FCell*> GetNeighbors(int32 X, int32 Y);
	TArray<const FCell*> GetNeighbors(int32 X, int32 Y) const;
	bool IsValidCoord(int32 X, int32 Y) const;
};
