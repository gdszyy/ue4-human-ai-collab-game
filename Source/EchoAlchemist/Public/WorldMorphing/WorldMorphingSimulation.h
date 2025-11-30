// Copyright Epic Games, Inc. All Rights Reserved.
// 世界变迁系统 - 模拟功能

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldMorphing/WorldMorphingTypes.h"
#include "WorldMorphingSimulation.generated.h"

/**
 * 世界变迁系统 - 模拟功能
 * 负责世界的初始化、更新、重置等核心模拟逻辑
 */
UCLASS()
class ECHOALCHEMIST_API UWorldMorphingSimulation : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 初始化世界
	 * @param WorldContextObject 世界上下文对象
	 * @param Width 网格宽度
	 * @param Height 网格高度
	 * @param Params 模拟参数（可选，默认使用标准参数）
	 * @return 是否成功初始化
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing|Simulation", 
	          meta = (WorldContext = "WorldContextObject"))
	static bool Initialize(UObject* WorldContextObject, 
	                       int32 Width, 
	                       int32 Height, 
	                       const FSimulationParams& Params);

	/**
	 * 更新模拟（单帧）
	 * @param WorldContextObject 世界上下文对象
	 * @param DeltaTime 时间增量
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing|Simulation", 
	          meta = (WorldContext = "WorldContextObject"))
	static void Tick(UObject* WorldContextObject, float DeltaTime);

	/**
	 * 重置世界到初始状态
	 * @param WorldContextObject 世界上下文对象
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing|Simulation", 
	          meta = (WorldContext = "WorldContextObject"))
	static void Reset(UObject* WorldContextObject);

	/**
	 * 暂停/继续模拟
	 * @param WorldContextObject 世界上下文对象
	 * @param bPause 是否暂停
	 */
	UFUNCTION(BlueprintCallable, Category = "WorldMorphing|Simulation", 
	          meta = (WorldContext = "WorldContextObject"))
	static void SetPaused(UObject* WorldContextObject, bool bPause);

	/**
	 * 获取模拟状态
	 * @param WorldContextObject 世界上下文对象
	 * @return 模拟状态信息
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Simulation", 
	          meta = (WorldContext = "WorldContextObject"))
	static FSimulationStatus GetStatus(UObject* WorldContextObject);
};
