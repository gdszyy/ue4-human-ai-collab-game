// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RecipeData.h"
#include "RecipeBlueprintLibrary.generated.h"

/**
 * 炼金配方系统蓝图函数库
 * 
 * 为蓝图提供便捷的配方系统访问接口。
 * 
 * ## 使用示例（蓝图）
 * 
 * 1. **评估配方**
 *    - 创建RecipeInput节点
 *    - 设置BasePotion和Additives
 *    - 调用EvaluateRecipe
 *    - 获取RecipeOutput结果
 * 
 * 2. **查询已发现的配方**
 *    - 调用GetDiscoveredRecipes
 *    - 遍历返回的配方列表
 * 
 * 3. **保存/加载配方库**
 *    - 调用SaveRecipeLibrary保存
 *    - 调用LoadRecipeLibrary加载
 */
UCLASS()
class ECHOALCHEMIST_API URecipeBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========== 配方评估 ==========

	/**
	 * 评估配方（蓝图版本）
	 * 
	 * @param WorldContextObject 世界上下文对象
	 * @param Input 配方输入参数
	 * @return 配方输出结果
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static FRecipeOutput EvaluateRecipe(UObject* WorldContextObject, const FRecipeInput& Input);

	/**
	 * 创建配方输入（辅助函数）
	 * 
	 * @param BasePotion 基础药剂类型
	 * @param Additives 添加剂列表
	 * @return 配方输入参数
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System")
	static FRecipeInput MakeRecipeInput(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives);

	/**
	 * 添加世界状态参数
	 * 
	 * @param Input 配方输入参数（引用）
	 * @param Key 参数键
	 * @param Value 参数值
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System")
	static void AddWorldStateParameter(UPARAM(ref) FRecipeInput& Input, const FString& Key, float Value);

	// ========== 配方库查询 ==========

	/**
	 * 获取已发现的配方列表
	 * 
	 * @param WorldContextObject 世界上下文对象
	 * @return 已发现的配方定义列表
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static TArray<FRecipeDefinition> GetDiscoveredRecipes(UObject* WorldContextObject);

	/**
	 * 检查配方是否已发现
	 * 
	 * @param WorldContextObject 世界上下文对象
	 * @param RecipeID 配方ID
	 * @return true=已发现，false=未发现
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static bool IsRecipeDiscovered(UObject* WorldContextObject, const FString& RecipeID);

	/**
	 * 根据配方ID获取配方
	 * 
	 * @param WorldContextObject 世界上下文对象
	 * @param RecipeID 配方ID
	 * @param OutRecipe 输出配方定义
	 * @return true=找到配方，false=未找到
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static bool GetRecipeByID(UObject* WorldContextObject, const FString& RecipeID, FRecipeDefinition& OutRecipe);

	/**
	 * 获取配方使用次数
	 * 
	 * @param WorldContextObject 世界上下文对象
	 * @param RecipeID 配方ID
	 * @return 使用次数
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static int32 GetRecipeUsageCount(UObject* WorldContextObject, const FString& RecipeID);

	// ========== 配方效果解析 ==========

	/**
	 * 从配方输出中获取效果值
	 * 
	 * @param Output 配方输出结果
	 * @param EffectKey 效果键（使用RecipeEffectKeys中的常量）
	 * @param DefaultValue 默认值（如果效果不存在）
	 * @return 效果值
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System")
	static float GetEffectValue(const FRecipeOutput& Output, const FString& EffectKey, float DefaultValue = 0.0f);

	/**
	 * 检查配方是否包含特定效果
	 * 
	 * @param Output 配方输出结果
	 * @param EffectKey 效果键
	 * @return true=包含，false=不包含
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System")
	static bool HasEffect(const FRecipeOutput& Output, const FString& EffectKey);

	// ========== 持久化 ==========

	/**
	 * 保存配方库
	 * 
	 * @param WorldContextObject 世界上下文对象
	 * @param SlotName 存档槽名称
	 * @return true=保存成功，false=保存失败
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static bool SaveRecipeLibrary(UObject* WorldContextObject, const FString& SlotName = TEXT("RecipeLibrary"));

	/**
	 * 加载配方库
	 * 
	 * @param WorldContextObject 世界上下文对象
	 * @param SlotName 存档槽名称
	 * @return true=加载成功，false=加载失败
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static bool LoadRecipeLibrary(UObject* WorldContextObject, const FString& SlotName = TEXT("RecipeLibrary"));

	/**
	 * 重置配方库
	 * 
	 * @param WorldContextObject 世界上下文对象
	 */
	UFUNCTION(BlueprintCallable, Category = "Recipe System", meta = (WorldContext = "WorldContextObject"))
	static void ResetRecipeLibrary(UObject* WorldContextObject);

	// ========== 辅助函数 ==========

	/**
	 * 获取基础药剂显示名称
	 * 
	 * @param BasePotion 基础药剂类型
	 * @return 显示名称
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System")
	static FString GetBasePotionDisplayName(EBasePotionType BasePotion);

	/**
	 * 获取添加剂显示名称
	 * 
	 * @param Additive 添加剂类型
	 * @return 显示名称
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System")
	static FString GetAdditiveDisplayName(EAdditiveType Additive);

	/**
	 * 获取稀有度颜色
	 * 
	 * @param Rarity 稀有度（0-5）
	 * @return 颜色（用于UI显示）
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System")
	static FLinearColor GetRarityColor(int32 Rarity);

	/**
	 * 获取稀有度名称
	 * 
	 * @param Rarity 稀有度（0-5）
	 * @return 稀有度名称
	 */
	UFUNCTION(BlueprintPure, Category = "Recipe System")
	static FString GetRarityName(int32 Rarity);
};
