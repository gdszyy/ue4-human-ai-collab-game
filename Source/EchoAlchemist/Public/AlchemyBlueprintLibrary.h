// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AlchemySystem.h"
#include "AlchemyBlueprintLibrary.generated.h"

/**
 * 炼金术系统蓝图函数库
 * 封装常用操作，让用户只需要一个节点即可完成功能
 */
UCLASS()
class ECHOALCHEMIST_API UAlchemyBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * 一键测试炼金术系统
	 * 自动运行所有测试用例并输出结果
	 * 
	 * 蓝图使用：
	 * Event BeginPlay -> Test Alchemy System (World Context Object: Self)
	 */
	UFUNCTION(BlueprintCallable, Category = "Alchemy|Test", meta = (WorldContext = "WorldContextObject"))
	static void TestAlchemySystem(UObject* WorldContextObject);
	
	/**
	 * 快速合成元素（带日志输出）
	 * 自动获取炼金术系统并合成元素，输出详细日志
	 * 
	 * 蓝图使用：
	 * Quick Combine (World Context Object: Self, Element1: "Fire", Element2: "Water")
	 * 
	 * @return 合成结果，如果失败则返回空元素
	 */
	UFUNCTION(BlueprintCallable, Category = "Alchemy", meta = (WorldContext = "WorldContextObject"))
	static FElement QuickCombine(UObject* WorldContextObject, const FString& Element1, const FString& Element2);
	
	/**
	 * 获取所有元素（带日志输出）
	 * 自动获取炼金术系统并返回所有元素，输出到日志
	 */
	UFUNCTION(BlueprintCallable, Category = "Alchemy", meta = (WorldContext = "WorldContextObject"))
	static TArray<FElement> GetAllElementsWithLog(UObject* WorldContextObject);
	
	/**
	 * 获取所有配方（带日志输出）
	 * 自动获取炼金术系统并返回所有配方，输出到日志
	 */
	UFUNCTION(BlueprintCallable, Category = "Alchemy", meta = (WorldContext = "WorldContextObject"))
	static TArray<FRecipe> GetAllRecipesWithLog(UObject* WorldContextObject);
};
