// Copyright Epic Games, Inc. All Rights Reserved.
// 世界变迁系统 - 参数配置

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldMorphing/WorldMorphingTypes.h"
#include "WorldMorphingConfiguration.generated.h"

/**
 * 世界变迁系统 - 参数配置
 * 负责参数的创建、获取、设置
 */
UCLASS()
class ECHOALCHEMIST_API UWorldMorphingConfiguration : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 创建默认参数
	 * @return 默认模拟参数
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Configuration")
	static FSimulationParams MakeDefault();

	/**
	 * 创建自定义参数
	 * @param ExpansionThreshold 扩张阈值
	 * @param ThunderstormThreshold 雷暴阈值
	 * @param AlphaEnergyDemand Alpha能量需求
	 * @return 自定义参数
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Configuration")
	static FSimulationParams MakeCustom(float ExpansionThreshold = 123.0f,
	                                     float ThunderstormThreshold = 18.0f,
	                                     float AlphaEnergyDemand = 1.5f);

	/**
	 * 获取当前参数
	 * @param WorldContextObject 世界上下文对象
	 * @return 当前模拟参数
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Configuration", 
	          meta = (WorldContext = "WorldContextObject"))
	static FSimulationParams GetCurrent(UObject* WorldContextObject);

	/**
	 * 应用参数
	 * @param WorldContextObject 世界上下文对象
	 * @param Params 新参数
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing|Configuration", 
	          meta = (WorldContext = "WorldContextObject"))
	static void Apply(UObject* WorldContextObject, const FSimulationParams& Params);

	/**
	 * 重置为默认参数
	 * @param WorldContextObject 世界上下文对象
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing|Configuration", 
	          meta = (WorldContext = "WorldContextObject"))
	static void ResetToDefault(UObject* WorldContextObject);

	/**
	 * 创建预设参数
	 * @param Preset 预设类型
	 * @return 预设参数
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Configuration")
	static FSimulationParams MakePreset(ESimulationPreset Preset);
};
