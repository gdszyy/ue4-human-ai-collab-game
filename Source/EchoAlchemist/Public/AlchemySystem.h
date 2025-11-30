// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AlchemySystem.generated.h"

/**
 * 元素结构体
 * 用于表示炼金术系统中的基础元素
 */
USTRUCT(BlueprintType)
struct FElement
{
	GENERATED_BODY()
	
	/** 元素名称 */
	UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
	FString Name;
	
	/** 元素类型 (Fire, Water, Earth, Air) */
	UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
	FString Type;
	
	/** 元素强度 */
	UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
	int32 Power;
	
	FElement()
		: Name(TEXT(""))
		, Type(TEXT(""))
		, Power(0)
	{
	}
	
	FElement(const FString& InName, const FString& InType, int32 InPower)
		: Name(InName)
		, Type(InType)
		, Power(InPower)
	{
	}
};

/**
 * 配方结构体
 * 定义两个元素合成的规则
 */
USTRUCT(BlueprintType)
struct FRecipe
{
	GENERATED_BODY()
	
	/** 第一个元素 */
	UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
	FString Element1;
	
	/** 第二个元素 */
	UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
	FString Element2;
	
	/** 合成结果 */
	UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
	FElement Result;
	
	FRecipe()
		: Element1(TEXT(""))
		, Element2(TEXT(""))
	{
	}
	
	FRecipe(const FString& InElement1, const FString& InElement2, const FElement& InResult)
		: Element1(InElement1)
		, Element2(InElement2)
		, Result(InResult)
	{
	}
};

/**
 * 炼金术系统
 * 负责管理元素合成逻辑
 */
UCLASS()
class ECHOALCHEMIST_API UAlchemySystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/** 初始化子系统 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/**
	 * 合成两个元素
	 * @param Element1 第一个元素的名称
	 * @param Element2 第二个元素的名称
	 * @param OutResult 合成结果（如果成功）
	 * @return 是否合成成功
	 */
	UFUNCTION(BlueprintCallable, Category = "Alchemy")
	bool CombineElements(const FString& Element1, const FString& Element2, FElement& OutResult);
	
	/**
	 * 获取所有可用元素
	 * @return 元素数组
	 */
	UFUNCTION(BlueprintPure, Category = "Alchemy")
	TArray<FElement> GetAllElements() const;
	
	/**
	 * 获取所有配方
	 * @return 配方数组
	 */
	UFUNCTION(BlueprintPure, Category = "Alchemy")
	TArray<FRecipe> GetAllRecipes() const;
	
protected:
	/** 初始化基础元素 */
	void InitializeElements();
	
	/** 初始化配方数据库 */
	void InitializeRecipes();
	
	/** 元素数据库（内部使用，不暴露给蓝图） */
	UPROPERTY()
	TMap<FString, FElement> ElementDatabase;
	
	/** 配方数据库（内部使用，不暴露给蓝图） */
	UPROPERTY()
	TArray<FRecipe> RecipeDatabase;
};
