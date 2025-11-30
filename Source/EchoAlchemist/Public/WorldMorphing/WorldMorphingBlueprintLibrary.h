// Copyright Epic Games, Inc. All Rights Reserved.
// 世界变迁系统 - 蓝图函数库

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldMorphing/WorldMorphingTypes.h"
#include "WorldMorphingBlueprintLibrary.generated.h"

/**
 * 世界变迁系统蓝图函数库
 * 提供便捷的蓝图调用接口
 */
UCLASS()
class ECHOALCHEMIST_API UWorldMorphingBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 初始化世界网格
	 * @param WorldContextObject 世界上下文对象
	 * @param Width 网格宽度
	 * @param Height 网格高度
	 * @param Params 模拟参数
	 * @deprecated Use UWorldMorphingSimulation::Initialize instead
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingSimulation::Initialize instead"))
	static void InitializeWorld(UObject* WorldContextObject, int32 Width, int32 Height, const FSimulationParams& Params);

	/**
	 * 手动触发一次模拟更新
	 * @param WorldContextObject 世界上下文对象
	 * @param DeltaTime 时间增量
	 * @deprecated Use UWorldMorphingSimulation::Tick instead
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingSimulation::Tick instead"))
	static void TickSimulation(UObject* WorldContextObject, float DeltaTime);

	/**
	 * 获取指定位置的单元格状态
	 * @param WorldContextObject 世界上下文对象
	 * @param X X坐标
	 * @param Y Y坐标
	 * @return 单元格状态
	 * @deprecated Use UWorldMorphingVisualization::GetCellState instead
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingVisualization::GetCellState instead"))
	static FCellState GetCellAt(UObject* WorldContextObject, int32 X, int32 Y);

	/**
	 * 设置模拟参数
	 * @param WorldContextObject 世界上下文对象
	 * @param NewParams 新参数
	 * @deprecated Use UWorldMorphingConfiguration::Apply instead
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingConfiguration::Apply instead"))
	static void SetSimulationParams(UObject* WorldContextObject, const FSimulationParams& NewParams);

	/**
	 * 获取当前模拟参数
	 * @param WorldContextObject 世界上下文对象
	 * @return 当前参数
	 * @deprecated Use UWorldMorphingConfiguration::GetCurrent instead
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingConfiguration::GetCurrent instead"))
	static FSimulationParams GetSimulationParams(UObject* WorldContextObject);

	/**
	 * 获取网格尺寸
	 * @param WorldContextObject 世界上下文对象
	 * @param OutWidth 输出宽度
	 * @param OutHeight 输出高度
	 * @deprecated Use UWorldMorphingSimulation::GetStatus instead
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingSimulation::GetStatus instead"))
	static void GetGridSize(UObject* WorldContextObject, int32& OutWidth, int32& OutHeight);

	/**
	 * 获取当前时间步
	 * @param WorldContextObject 世界上下文对象
	 * @return 时间步
	 * @deprecated Use UWorldMorphingSimulation::GetStatus instead
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingSimulation::GetStatus instead"))
	static int32 GetTimeStep(UObject* WorldContextObject);

	/**
	 * 获取周期计数
	 * @param WorldContextObject 世界上下文对象
	 * @return 周期计数
	 * @deprecated Use UWorldMorphingSimulation::GetStatus instead
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing", 
	          meta = (WorldContext = "WorldContextObject", DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingSimulation::GetStatus instead"))
	static int32 GetCycleCount(UObject* WorldContextObject);

	/**
	 * 创建默认模拟参数
	 * @return 默认参数
	 * @deprecated Use UWorldMorphingConfiguration::MakeDefault instead
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing", 
	          meta = (DeprecatedFunction, 
	                  DeprecationMessage = "Use UWorldMorphingConfiguration::MakeDefault instead"))
	static FSimulationParams MakeDefaultParams();
};
