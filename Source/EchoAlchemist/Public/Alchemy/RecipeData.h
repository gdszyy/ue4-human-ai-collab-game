// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RecipeData.generated.h"

/**
 * 基础药剂类型
 * 
 * 定义三种基础弹射药剂类型，作为配方的基础。
 */
UENUM(BlueprintType)
enum class EBasePotionType : uint8
{
	/** 弹射药剂 - 伤害递增 */
	Ricochet UMETA(DisplayName = "Ricochet"),
	
	/** 穿透药剂 - 穿透敌人 */
	Piercing UMETA(DisplayName = "Piercing"),
	
	/** 爆炸药剂 - 范围伤害 */
	Explosive UMETA(DisplayName = "Explosive")
};

/**
 * 魔法添加剂类型
 * 
 * 定义各种魔法添加剂，可以与基础药剂组合产生特殊效果。
 */
UENUM(BlueprintType)
enum class EAdditiveType : uint8
{
	/** 无添加剂 */
	None UMETA(DisplayName = "None"),
	
	// ========== 持续类添加剂 ==========
	
	/** 持续药剂 - 增加药效强度 */
	Duration UMETA(DisplayName = "Duration"),
	
	/** 强化药剂 - 增加伤害 */
	Amplify UMETA(DisplayName = "Amplify"),
	
	// ========== 特殊效果添加剂 ==========
	
	/** 分裂药剂 - 碰撞后分裂 */
	Split UMETA(DisplayName = "Split"),
	
	/** 连锁药剂 - 触发连锁反应 */
	Chain UMETA(DisplayName = "Chain"),
	
	/** 引力药剂 - 产生引力场 */
	Gravity UMETA(DisplayName = "Gravity"),
	
	/** 传送药剂 - 创建虫洞 */
	Teleport UMETA(DisplayName = "Teleport"),
	
	/** 加速药剂 - 增加速度 */
	Speed UMETA(DisplayName = "Speed"),
	
	/** 减速药剂 - 降低速度 */
	Slow UMETA(DisplayName = "Slow"),
	
	// ========== 元素类添加剂 ==========
	
	/** 火焰药剂 - 火属性伤害 */
	Fire UMETA(DisplayName = "Fire"),
	
	/** 冰霜药剂 - 冰属性伤害 */
	Ice UMETA(DisplayName = "Ice"),
	
	/** 雷电药剂 - 雷属性伤害 */
	Lightning UMETA(DisplayName = "Lightning"),
	
	/** 毒素药剂 - 毒属性伤害 */
	Poison UMETA(DisplayName = "Poison")
};

/**
 * 配方效果键
 * 
 * 定义配方效果的标准化键名，用于TMap<FString, float>。
 */
namespace RecipeEffectKeys
{
	// ========== 基础属性 ==========
	const FString BaseDamage = TEXT("BaseDamage");           // 基础伤害
	const FString Potency = TEXT("Potency");                 // 药效强度
	const FString Speed = TEXT("Speed");                     // 速度
	const FString Radius = TEXT("Radius");                   // 半径
	
	// ========== 特殊效果 ==========
	const FString SplitCount = TEXT("SplitCount");           // 分裂数量
	const FString ChainCount = TEXT("ChainCount");           // 连锁数量
	const FString GravityStrength = TEXT("GravityStrength"); // 引力强度
	const FString TeleportRange = TEXT("TeleportRange");     // 传送距离
	
	// ========== 元素伤害 ==========
	const FString FireDamage = TEXT("FireDamage");           // 火焰伤害
	const FString IceDamage = TEXT("IceDamage");             // 冰霜伤害
	const FString LightningDamage = TEXT("LightningDamage"); // 雷电伤害
	const FString PoisonDamage = TEXT("PoisonDamage");       // 毒素伤害
	
	// ========== 效果倍率 ==========
	const FString DamageMultiplier = TEXT("DamageMultiplier");     // 伤害倍率
	const FString DurationMultiplier = TEXT("DurationMultiplier"); // 持续时间倍率
	const FString RangeMultiplier = TEXT("RangeMultiplier");       // 范围倍率
}

/**
 * 配方输入参数
 * 
 * 用于评估配方时传递输入参数。
 */
USTRUCT(BlueprintType)
struct FRecipeInput
{
	GENERATED_BODY()

	/** 基础药剂类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	EBasePotionType BasePotion = EBasePotionType::Ricochet;

	/** 添加剂列表（最多3个） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TArray<EAdditiveType> Additives;

	/** 世界状态参数（可选，用于计算效果倍率） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TMap<FString, float> WorldState;

	FRecipeInput()
	{
	}
};

/**
 * 配方输出结果
 * 
 * 评估配方后返回的结果，包含配方ID、效果参数等。
 */
USTRUCT(BlueprintType)
struct FRecipeOutput
{
	GENERATED_BODY()

	/** 配方ID（由基础药剂+添加剂组合生成） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FString RecipeID;

	/** 配方显示名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FString DisplayName;

	/** 配方描述 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FString Description;

	/** 效果参数（通用键值对） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TMap<FString, float> Effects;

	/** 效果倍率（受世界状态影响） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	float EffectMultiplier = 1.0f;

	/** 是否为新发现的配方 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	bool bIsNewRecipe = false;

	/** 配方稀有度（0-5，越高越稀有） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	int32 Rarity = 0;

	FRecipeOutput()
	{
	}
};

/**
 * 配方定义
 * 
 * 定义一个完整的配方，包括基础药剂、添加剂、效果等。
 */
USTRUCT(BlueprintType)
struct FRecipeDefinition
{
	GENERATED_BODY()

	/** 配方ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FString RecipeID;

	/** 配方显示名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FString DisplayName;

	/** 配方描述 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FString Description;

	/** 基础药剂类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	EBasePotionType BasePotion = EBasePotionType::Ricochet;

	/** 必需的添加剂列表 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TArray<EAdditiveType> RequiredAdditives;

	/** 基础效果参数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TMap<FString, float> BaseEffects;

	/** 配方稀有度（0-5） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	int32 Rarity = 0;

	/** 是否已发现 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	bool bDiscovered = false;

	FRecipeDefinition()
	{
	}

	/**
	 * 检查输入是否匹配此配方
	 * 
	 * @param Input 配方输入参数
	 * @return true=匹配，false=不匹配
	 */
	bool Matches(const FRecipeInput& Input) const
	{
		// 检查基础药剂
		if (Input.BasePotion != BasePotion)
		{
			return false;
		}

		// 检查添加剂数量
		if (Input.Additives.Num() != RequiredAdditives.Num())
		{
			return false;
		}

		// 检查添加剂是否完全匹配（顺序无关）
		TArray<EAdditiveType> InputAdditives = Input.Additives;
		TArray<EAdditiveType> RequiredAdditivesCopy = RequiredAdditives;

		for (EAdditiveType Additive : RequiredAdditivesCopy)
		{
			if (!InputAdditives.Contains(Additive))
			{
				return false;
			}
			InputAdditives.Remove(Additive);
		}

		return InputAdditives.Num() == 0;
	}

	/**
	 * 生成配方ID
	 * 
	 * @param BasePotion 基础药剂类型
	 * @param Additives 添加剂列表
	 * @return 配方ID字符串
	 */
	static FString GenerateRecipeID(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives)
	{
		FString ID = FString::Printf(TEXT("Recipe_%d"), static_cast<int32>(BasePotion));

		// 排序添加剂以确保ID一致性
		TArray<EAdditiveType> SortedAdditives = Additives;
		SortedAdditives.Sort([](const EAdditiveType& A, const EAdditiveType& B) {
			return static_cast<int32>(A) < static_cast<int32>(B);
		});

		for (EAdditiveType Additive : SortedAdditives)
		{
			ID += FString::Printf(TEXT("_%d"), static_cast<int32>(Additive));
		}

		return ID;
	}
};

/**
 * 配方库存档数据
 * 
 * 用于保存和加载配方库的持久化数据。
 */
USTRUCT(BlueprintType)
struct FRecipeLibrarySaveData
{
	GENERATED_BODY()

	/** 已发现的配方ID列表 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TArray<FString> DiscoveredRecipeIDs;

	/** 配方发现时间戳（配方ID -> 时间戳） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TMap<FString, FDateTime> DiscoveryTimestamps;

	/** 配方使用次数（配方ID -> 使用次数） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TMap<FString, int32> UsageCounts;

	FRecipeLibrarySaveData()
	{
	}
};
