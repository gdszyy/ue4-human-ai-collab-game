// Copyright Epic Games, Inc. All Rights Reserved.
// 世界变迁系统 - 视觉呈现

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldMorphing/WorldMorphingTypes.h"
#include "WorldMorphingVisualization.generated.h"

/**
 * 世界变迁系统 - 视觉呈现
 * 负责获取世界状态数据用于渲染和可视化
 */
UCLASS()
class ECHOALCHEMIST_API UWorldMorphingVisualization : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 获取单个单元格状态
	 * @param WorldContextObject 世界上下文对象
	 * @param X X坐标
	 * @param Y Y坐标
	 * @return 单元格状态
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Visualization", 
	          meta = (WorldContext = "WorldContextObject"))
	static FCellState GetCellState(UObject* WorldContextObject, int32 X, int32 Y);

	/**
	 * 获取区域单元格状态（批量）
	 * @param WorldContextObject 世界上下文对象
	 * @param StartX 起始X坐标
	 * @param StartY 起始Y坐标
	 * @param Width 区域宽度
	 * @param Height 区域高度
	 * @return 单元格状态数组
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Visualization", 
	          meta = (WorldContext = "WorldContextObject"))
	static TArray<FCellState> GetRegionStates(UObject* WorldContextObject, 
	                                           int32 StartX, int32 StartY, 
	                                           int32 Width, int32 Height);

	/**
	 * 获取整个世界的统计信息
	 * @param WorldContextObject 世界上下文对象
	 * @return 世界统计信息
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Visualization", 
	          meta = (WorldContext = "WorldContextObject"))
	static FWorldStatistics GetStatistics(UObject* WorldContextObject);

	/**
	 * 获取热力图数据（用于调试可视化）
	 * @param WorldContextObject 世界上下文对象
	 * @param DataType 数据类型（地幔能量、温度、晶石等）
	 * @return 热力图数据（归一化到0-1）
	 */
	UFUNCTION(BlueprintPure, Category = "WorldMorphing|Visualization", 
	          meta = (WorldContext = "WorldContextObject"))
	static TArray<float> GetHeatmapData(UObject* WorldContextObject, 
	                                     EHeatmapDataType DataType);
};
