// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RecipeData.h"
#include "RecipeSystem.generated.h"

/**
 * 炼金配方系统
 * 
 * 负责管理魔药配方的组合、发现、存储和应用。
 * 
 * ## 核心功能
 * 
 * 1. **配方评估** - 根据输入的基础药剂和添加剂组合，评估并返回配方效果
 * 2. **配方发现** - 自动记录玩家发现的新配方
 * 3. **配方库管理** - 管理已发现的配方库
 * 4. **持久化** - 保存和加载配方库到玩家存档
 * 5. **世界状态联动** - 根据世界状态（气候、晶石分布等）调整配方效果
 * 
 * ## 使用示例（C++）
 * 
 * ```cpp
 * // 获取配方系统实例
 * URecipeSystem* RecipeSystem = URecipeSystem::Get(GetWorld());
 * 
 * // 创建配方输入
 * FRecipeInput Input;
 * Input.BasePotion = EBasePotionType::Ricochet;
 * Input.Additives.Add(EAdditiveType::Split);
 * Input.Additives.Add(EAdditiveType::Fire);
 * 
 * // 评估配方
 * FRecipeOutput Output = RecipeSystem->EvaluateRecipe(Input);
 * 
 * if (Output.bIsNewRecipe)
 * {
 *     UE_LOG(LogTemp, Log, TEXT("发现新配方: %s"), *Output.DisplayName);
 * }
 * 
 * // 获取配方效果
 * float BaseDamage = Output.Effects.FindRef(RecipeEffectKeys::BaseDamage);
 * float SplitCount = Output.Effects.FindRef(RecipeEffectKeys::SplitCount);
 * ```
 * 
 * ## 使用示例（蓝图）
 * 
 * 通过URecipeBlueprintLibrary提供的蓝图函数库使用。
 * 
 * ## 设计原则
 * 
 * - **模块解耦**: 配方系统不依赖其他游戏模块，只使用通用的键值对接口
 * - **数据驱动**: 所有配方定义都存储在数据表中，易于扩展和调整
 * - **性能优化**: 使用TMap快速查找配方，避免重复计算
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API URecipeSystem : public UObject
{
	GENERATED_BODY()

public:
	URecipeSystem();

	// ========== 单例访问 ==========

	/**
	 * 获取配方系统实例
	 * 
	 * @param WorldContext 世界上下文对象
	 * @return 配方系统实例
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static URecipeSystem* Get(UObject* WorldContextObject);

	// ========== 配方评估 ==========

	/**
	 * 评估配方
	 * 
	 * 根据输入的基础药剂和添加剂组合，评估并返回配方效果。
	 * 如果是新配方，会自动记录到配方库中。
	 * 
	 * @param Input 配方输入参数
	 * @return 配方输出结果
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	FRecipeOutput EvaluateRecipe(const FRecipeInput& Input);

	/**
	 * 根据配方ID获取配方
	 * 
	 * @param RecipeID 配方ID
	 * @param OutRecipe 输出配方定义
	 * @return true=找到配方，false=未找到
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	bool GetRecipeByID(const FString& RecipeID, FRecipeDefinition& OutRecipe) const;

	// ========== 配方库管理 ==========

	/**
	 * 获取已发现的配方列表
	 * 
	 * @return 已发现的配方ID列表
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	TArray<FString> GetDiscoveredRecipes() const;

	/**
	 * 获取已发现的配方定义列表
	 * 
	 * @return 已发现的配方定义列表
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	TArray<FRecipeDefinition> GetDiscoveredRecipeDefinitions() const;

	/**
	 * 检查配方是否已发现
	 * 
	 * @param RecipeID 配方ID
	 * @return true=已发现，false=未发现
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	bool IsRecipeDiscovered(const FString& RecipeID) const;

	/**
	 * 手动标记配方为已发现
	 * 
	 * @param RecipeID 配方ID
	 * @return true=成功，false=失败（配方不存在）
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	bool DiscoverRecipe(const FString& RecipeID);

	/**
	 * 获取配方使用次数
	 * 
	 * @param RecipeID 配方ID
	 * @return 使用次数
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	int32 GetRecipeUsageCount(const FString& RecipeID) const;

	/**
	 * 增加配方使用次数
	 * 
	 * @param RecipeID 配方ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	void IncrementRecipeUsage(const FString& RecipeID);

	// ========== 持久化 ==========

	/**
	 * 保存配方库到存档
	 * 
	 * @param SlotName 存档槽名称
	 * @return true=保存成功，false=保存失败
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	bool SaveRecipeLibrary(const FString& SlotName = TEXT("RecipeLibrary"));

	/**
	 * 从存档加载配方库
	 * 
	 * @param SlotName 存档槽名称
	 * @return true=加载成功，false=加载失败
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	bool LoadRecipeLibrary(const FString& SlotName = TEXT("RecipeLibrary"));

	/**
	 * 重置配方库（清除所有已发现的配方）
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	void ResetRecipeLibrary();

	// ========== 配方数据管理 ==========

	/**
	 * 初始化配方数据
	 * 
	 * 从数据表或配置文件加载所有配方定义。
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	void InitializeRecipes();

	/**
	 * 添加自定义配方
	 * 
	 * @param Recipe 配方定义
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	void AddRecipe(const FRecipeDefinition& Recipe);

	/**
	 * 获取所有配方定义
	 * 
	 * @return 所有配方定义列表
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	TArray<FRecipeDefinition> GetAllRecipes() const;

private:
	// ========== 内部数据 ==========

	/** 所有配方定义（配方ID -> 配方定义） */
	UPROPERTY()
	TMap<FString, FRecipeDefinition> RecipeDefinitions;

	/** 已发现的配方ID集合 */
	UPROPERTY()
	TSet<FString> DiscoveredRecipeIDs;

	/** 配方使用次数（配方ID -> 使用次数） */
	UPROPERTY()
	TMap<FString, int32> RecipeUsageCounts;

	/** 配方发现时间戳（配方ID -> 时间戳） */
	UPROPERTY()
	TMap<FString, FDateTime> DiscoveryTimestamps;

	// ========== 内部方法 ==========

	/**
	 * 计算世界状态对配方的效果倍率
	 * 
	 * @param Recipe 配方定义
	 * @param WorldState 世界状态参数
	 * @return 效果倍率
	 */
	float CalculateWorldStateMultiplier(const FRecipeDefinition& Recipe, const TMap<FString, float>& WorldState) const;

	/**
	 * 创建默认配方库
	 * 
	 * 创建一些预定义的配方作为示例。
	 */
	void CreateDefaultRecipes();

	/**
	 * 生成配方显示名称
	 * 
	 * @param BasePotion 基础药剂类型
	 * @param Additives 添加剂列表
	 * @return 显示名称
	 */
	static FString GenerateDisplayName(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives);

	/**
	 * 生成配方描述
	 * 
	 * @param BasePotion 基础药剂类型
	 * @param Additives 添加剂列表
	 * @return 配方描述
	 */
	static FString GenerateDescription(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives);

	/**
	 * 计算配方稀有度
	 * 
	 * @param Additives 添加剂列表
	 * @return 稀有度（0-5）
	 */
	static int32 CalculateRarity(const TArray<EAdditiveType>& Additives);

	/**
	 * 计算基础效果
	 * 
	 * @param BasePotion 基础药剂类型
	 * @param Additives 添加剂列表
	 * @return 效果参数映射
	 */
	TMap<FString, float> CalculateBaseEffects(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives) const;
};
