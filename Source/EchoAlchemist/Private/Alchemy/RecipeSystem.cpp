// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Alchemy/RecipeSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

URecipeSystem::URecipeSystem()
{
}

URecipeSystem* URecipeSystem::Get(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	// 从GameInstance获取或创建RecipeSystem实例
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	URecipeSystem* RecipeSystem = GameInstance->GetSubsystem<URecipeSystem>();
	if (!RecipeSystem)
	{
		RecipeSystem = NewObject<URecipeSystem>(GameInstance);
		RecipeSystem->InitializeRecipes();
	}

	return RecipeSystem;
}

FRecipeOutput URecipeSystem::EvaluateRecipe(const FRecipeInput& Input)
{
	FRecipeOutput Output;

	// 生成配方ID
	FString RecipeID = FRecipeDefinition::GenerateRecipeID(Input.BasePotion, Input.Additives);
	Output.RecipeID = RecipeID;

	// 查找配方定义
	FRecipeDefinition* RecipeDef = RecipeDefinitions.Find(RecipeID);

	if (!RecipeDef)
	{
		// 配方不存在，创建新配方
		FRecipeDefinition NewRecipe;
		NewRecipe.RecipeID = RecipeID;
		NewRecipe.BasePotion = Input.BasePotion;
		NewRecipe.RequiredAdditives = Input.Additives;
		NewRecipe.DisplayName = GenerateDisplayName(Input.BasePotion, Input.Additives);
		NewRecipe.Description = GenerateDescription(Input.BasePotion, Input.Additives);
		NewRecipe.Rarity = CalculateRarity(Input.Additives);

		// 计算基础效果
		NewRecipe.BaseEffects = CalculateBaseEffects(Input.BasePotion, Input.Additives);

		// 添加到配方库
		RecipeDefinitions.Add(RecipeID, NewRecipe);
		RecipeDef = &RecipeDefinitions[RecipeID];
	}

	// 填充输出
	Output.DisplayName = RecipeDef->DisplayName;
	Output.Description = RecipeDef->Description;
	Output.Rarity = RecipeDef->Rarity;
	Output.Effects = RecipeDef->BaseEffects;

	// 计算世界状态倍率
	Output.EffectMultiplier = CalculateWorldStateMultiplier(*RecipeDef, Input.WorldState);

	// 应用倍率到效果
	for (auto& Pair : Output.Effects)
	{
		Pair.Value *= Output.EffectMultiplier;
	}

	// 检查是否为新发现的配方
	if (!DiscoveredRecipeIDs.Contains(RecipeID))
	{
		Output.bIsNewRecipe = true;
		DiscoveredRecipeIDs.Add(RecipeID);
		DiscoveryTimestamps.Add(RecipeID, FDateTime::Now());
		RecipeDef->bDiscovered = true;
	}

	// 增加使用次数
	IncrementRecipeUsage(RecipeID);

	return Output;
}

bool URecipeSystem::GetRecipeByID(const FString& RecipeID, FRecipeDefinition& OutRecipe) const
{
	const FRecipeDefinition* RecipeDef = RecipeDefinitions.Find(RecipeID);
	if (RecipeDef)
	{
		OutRecipe = *RecipeDef;
		return true;
	}
	return false;
}

TArray<FString> URecipeSystem::GetDiscoveredRecipes() const
{
	return DiscoveredRecipeIDs.Array();
}

TArray<FRecipeDefinition> URecipeSystem::GetDiscoveredRecipeDefinitions() const
{
	TArray<FRecipeDefinition> Result;

	for (const FString& RecipeID : DiscoveredRecipeIDs)
	{
		const FRecipeDefinition* RecipeDef = RecipeDefinitions.Find(RecipeID);
		if (RecipeDef)
		{
			Result.Add(*RecipeDef);
		}
	}

	return Result;
}

bool URecipeSystem::IsRecipeDiscovered(const FString& RecipeID) const
{
	return DiscoveredRecipeIDs.Contains(RecipeID);
}

bool URecipeSystem::DiscoverRecipe(const FString& RecipeID)
{
	FRecipeDefinition* RecipeDef = RecipeDefinitions.Find(RecipeID);
	if (!RecipeDef)
	{
		return false;
	}

	if (!DiscoveredRecipeIDs.Contains(RecipeID))
	{
		DiscoveredRecipeIDs.Add(RecipeID);
		DiscoveryTimestamps.Add(RecipeID, FDateTime::Now());
		RecipeDef->bDiscovered = true;
	}

	return true;
}

int32 URecipeSystem::GetRecipeUsageCount(const FString& RecipeID) const
{
	const int32* Count = RecipeUsageCounts.Find(RecipeID);
	return Count ? *Count : 0;
}

void URecipeSystem::IncrementRecipeUsage(const FString& RecipeID)
{
	int32& Count = RecipeUsageCounts.FindOrAdd(RecipeID, 0);
	Count++;
}

bool URecipeSystem::SaveRecipeLibrary(const FString& SlotName)
{
	FRecipeLibrarySaveData SaveData;
	SaveData.DiscoveredRecipeIDs = DiscoveredRecipeIDs.Array();
	SaveData.DiscoveryTimestamps = DiscoveryTimestamps;
	SaveData.UsageCounts = RecipeUsageCounts;

	// TODO: 实现实际的保存逻辑（使用UGameplayStatics::SaveGameToSlot）
	// 这里需要创建一个USaveGame子类来存储数据

	return true;
}

bool URecipeSystem::LoadRecipeLibrary(const FString& SlotName)
{
	// TODO: 实现实际的加载逻辑（使用UGameplayStatics::LoadGameFromSlot）

	return true;
}

void URecipeSystem::ResetRecipeLibrary()
{
	DiscoveredRecipeIDs.Empty();
	DiscoveryTimestamps.Empty();
	RecipeUsageCounts.Empty();

	// 重置所有配方的发现状态
	for (auto& Pair : RecipeDefinitions)
	{
		Pair.Value.bDiscovered = false;
	}
}

void URecipeSystem::InitializeRecipes()
{
	// 清空现有配方
	RecipeDefinitions.Empty();

	// 创建默认配方库
	CreateDefaultRecipes();
}

void URecipeSystem::AddRecipe(const FRecipeDefinition& Recipe)
{
	RecipeDefinitions.Add(Recipe.RecipeID, Recipe);
}

TArray<FRecipeDefinition> URecipeSystem::GetAllRecipes() const
{
	TArray<FRecipeDefinition> Result;
	RecipeDefinitions.GenerateValueArray(Result);
	return Result;
}

float URecipeSystem::CalculateWorldStateMultiplier(const FRecipeDefinition& Recipe, const TMap<FString, float>& WorldState) const
{
	float Multiplier = 1.0f;

	// 检查配方中是否有元素伤害
	bool bHasFireDamage = Recipe.BaseEffects.Contains(RecipeEffectKeys::FireDamage);
	bool bHasIceDamage = Recipe.BaseEffects.Contains(RecipeEffectKeys::IceDamage);
	bool bHasLightningDamage = Recipe.BaseEffects.Contains(RecipeEffectKeys::LightningDamage);
	bool bHasPoisonDamage = Recipe.BaseEffects.Contains(RecipeEffectKeys::PoisonDamage);

	// 根据世界状态调整倍率
	// 示例：高温气候增强火属性，低温气候增强冰属性
	if (WorldState.Contains(TEXT("Temperature")))
	{
		float Temperature = WorldState[TEXT("Temperature")];

		if (bHasFireDamage && Temperature > 30.0f)
		{
			Multiplier += 0.2f; // 高温增强火属性20%
		}

		if (bHasIceDamage && Temperature < 10.0f)
		{
			Multiplier += 0.2f; // 低温增强冰属性20%
		}
	}

	// 示例：晶石密集区增强所有效果
	if (WorldState.Contains(TEXT("CrystalDensity")))
	{
		float CrystalDensity = WorldState[TEXT("CrystalDensity")];
		Multiplier += CrystalDensity * 0.1f; // 晶石密度每增加1，效果增强10%
	}

	return Multiplier;
}

void URecipeSystem::CreateDefaultRecipes()
{
	// ========== 基础配方（无添加剂） ==========

	// 弹射药剂
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Ricochet, {});
		Recipe.DisplayName = TEXT("基础弹射药剂");
		Recipe.Description = TEXT("最基础的弹射药剂，每次碰撞后伤害递增。");
		Recipe.BasePotion = EBasePotionType::Ricochet;
		Recipe.Rarity = 0;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 10.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 1.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 500.0f);

		AddRecipe(Recipe);
	}

	// 穿透药剂
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Piercing, {});
		Recipe.DisplayName = TEXT("基础穿透药剂");
		Recipe.Description = TEXT("可以穿透敌人，对路径上的所有敌人造成伤害。");
		Recipe.BasePotion = EBasePotionType::Piercing;
		Recipe.Rarity = 0;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 15.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 1.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 600.0f);

		AddRecipe(Recipe);
	}

	// 爆炸药剂
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Explosive, {});
		Recipe.DisplayName = TEXT("基础爆炸药剂");
		Recipe.Description = TEXT("碰撞后产生爆炸，对范围内的敌人造成伤害。");
		Recipe.BasePotion = EBasePotionType::Explosive;
		Recipe.Rarity = 0;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 20.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 1.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 400.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Radius, 200.0f);

		AddRecipe(Recipe);
	}

	// ========== 单添加剂配方 ==========

	// 弹射 + 分裂
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Ricochet, {EAdditiveType::Split});
		Recipe.DisplayName = TEXT("分裂弹射药剂");
		Recipe.Description = TEXT("碰撞后分裂成多个子代魔药，继续攻击敌人。");
		Recipe.BasePotion = EBasePotionType::Ricochet;
		Recipe.RequiredAdditives = {EAdditiveType::Split};
		Recipe.Rarity = 1;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 10.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 1.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 500.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::SplitCount, 3.0f);

		AddRecipe(Recipe);
	}

	// 弹射 + 火焰
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Ricochet, {EAdditiveType::Fire});
		Recipe.DisplayName = TEXT("火焰弹射药剂");
		Recipe.Description = TEXT("附加火焰伤害的弹射药剂，在高温环境下效果更强。");
		Recipe.BasePotion = EBasePotionType::Ricochet;
		Recipe.RequiredAdditives = {EAdditiveType::Fire};
		Recipe.Rarity = 1;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 10.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 1.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 500.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::FireDamage, 15.0f);

		AddRecipe(Recipe);
	}

	// ========== 双添加剂配方 ==========

	// 弹射 + 分裂 + 火焰
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Ricochet, {EAdditiveType::Split, EAdditiveType::Fire});
		Recipe.DisplayName = TEXT("炎爆分裂药剂");
		Recipe.Description = TEXT("分裂后的每个子代都附带火焰伤害，形成火焰风暴。");
		Recipe.BasePotion = EBasePotionType::Ricochet;
		Recipe.RequiredAdditives = {EAdditiveType::Split, EAdditiveType::Fire};
		Recipe.Rarity = 2;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 10.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 1.2f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 500.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::SplitCount, 3.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::FireDamage, 20.0f);

		AddRecipe(Recipe);
	}

	// 爆炸 + 连锁 + 雷电
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Explosive, {EAdditiveType::Chain, EAdditiveType::Lightning});
		Recipe.DisplayName = TEXT("雷霆连锁爆破");
		Recipe.Description = TEXT("爆炸后触发连锁雷电，对大范围敌人造成持续伤害。");
		Recipe.BasePotion = EBasePotionType::Explosive;
		Recipe.RequiredAdditives = {EAdditiveType::Chain, EAdditiveType::Lightning};
		Recipe.Rarity = 3;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 20.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 1.5f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 400.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Radius, 250.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::ChainCount, 5.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::LightningDamage, 25.0f);

		AddRecipe(Recipe);
	}

	// ========== 三添加剂配方（稀有） ==========

	// 穿透 + 引力 + 冰霜 + 毒素
	{
		FRecipeDefinition Recipe;
		Recipe.RecipeID = FRecipeDefinition::GenerateRecipeID(EBasePotionType::Piercing, {EAdditiveType::Gravity, EAdditiveType::Ice, EAdditiveType::Poison});
		Recipe.DisplayName = TEXT("极寒毒素黑洞");
		Recipe.Description = TEXT("创造引力场吸引敌人，同时释放冰霜和毒素，造成持续伤害和减速。");
		Recipe.BasePotion = EBasePotionType::Piercing;
		Recipe.RequiredAdditives = {EAdditiveType::Gravity, EAdditiveType::Ice, EAdditiveType::Poison};
		Recipe.Rarity = 5;

		Recipe.BaseEffects.Add(RecipeEffectKeys::BaseDamage, 15.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Potency, 2.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::Speed, 600.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::GravityStrength, 1000.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::IceDamage, 30.0f);
		Recipe.BaseEffects.Add(RecipeEffectKeys::PoisonDamage, 35.0f);

		AddRecipe(Recipe);
	}
}

FString URecipeSystem::GenerateDisplayName(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives)
{
	FString Name;

	// 基础药剂名称
	switch (BasePotion)
	{
	case EBasePotionType::Ricochet:
		Name = TEXT("弹射");
		break;
	case EBasePotionType::Piercing:
		Name = TEXT("穿透");
		break;
	case EBasePotionType::Explosive:
		Name = TEXT("爆炸");
		break;
	}

	// 添加剂名称
	for (EAdditiveType Additive : Additives)
	{
		switch (Additive)
		{
		case EAdditiveType::Split:
			Name += TEXT("分裂");
			break;
		case EAdditiveType::Chain:
			Name += TEXT("连锁");
			break;
		case EAdditiveType::Fire:
			Name += TEXT("火焰");
			break;
		case EAdditiveType::Ice:
			Name += TEXT("冰霜");
			break;
		case EAdditiveType::Lightning:
			Name += TEXT("雷电");
			break;
		case EAdditiveType::Poison:
			Name += TEXT("毒素");
			break;
		case EAdditiveType::Gravity:
			Name += TEXT("引力");
			break;
		case EAdditiveType::Teleport:
			Name += TEXT("传送");
			break;
		case EAdditiveType::Speed:
			Name += TEXT("加速");
			break;
		case EAdditiveType::Slow:
			Name += TEXT("减速");
			break;
		case EAdditiveType::Duration:
			Name += TEXT("持续");
			break;
		case EAdditiveType::Amplify:
			Name += TEXT("强化");
			break;
		default:
			break;
		}
	}

	Name += TEXT("药剂");

	return Name;
}

FString URecipeSystem::GenerateDescription(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives)
{
	FString Description;

	// 基础药剂描述
	switch (BasePotion)
	{
	case EBasePotionType::Ricochet:
		Description = TEXT("弹射药剂，每次碰撞后伤害递增");
		break;
	case EBasePotionType::Piercing:
		Description = TEXT("穿透药剂，可以穿透敌人");
		break;
	case EBasePotionType::Explosive:
		Description = TEXT("爆炸药剂，碰撞后产生范围伤害");
		break;
	}

	// 添加剂描述
	if (Additives.Num() > 0)
	{
		Description += TEXT("，附加");

		for (int32 i = 0; i < Additives.Num(); ++i)
		{
			switch (Additives[i])
			{
			case EAdditiveType::Split:
				Description += TEXT("分裂效果");
				break;
			case EAdditiveType::Chain:
				Description += TEXT("连锁效果");
				break;
			case EAdditiveType::Fire:
				Description += TEXT("火焰伤害");
				break;
			case EAdditiveType::Ice:
				Description += TEXT("冰霜伤害");
				break;
			case EAdditiveType::Lightning:
				Description += TEXT("雷电伤害");
				break;
			case EAdditiveType::Poison:
				Description += TEXT("毒素伤害");
				break;
			case EAdditiveType::Gravity:
				Description += TEXT("引力场");
				break;
			case EAdditiveType::Teleport:
				Description += TEXT("传送效果");
				break;
			case EAdditiveType::Speed:
				Description += TEXT("加速效果");
				break;
			case EAdditiveType::Slow:
				Description += TEXT("减速效果");
				break;
			case EAdditiveType::Duration:
				Description += TEXT("持续效果");
				break;
			case EAdditiveType::Amplify:
				Description += TEXT("强化效果");
				break;
			default:
				break;
			}

			if (i < Additives.Num() - 1)
			{
				Description += TEXT("、");
			}
		}
	}

	Description += TEXT("。");

	return Description;
}

int32 URecipeSystem::CalculateRarity(const TArray<EAdditiveType>& Additives)
{
	// 稀有度基于添加剂数量
	// 0个添加剂 = 0（普通）
	// 1个添加剂 = 1（常见）
	// 2个添加剂 = 2-3（稀有）
	// 3个添加剂 = 4-5（史诗/传说）

	int32 Rarity = Additives.Num();

	// 某些特殊添加剂增加稀有度
	for (EAdditiveType Additive : Additives)
	{
		if (Additive == EAdditiveType::Gravity || Additive == EAdditiveType::Teleport)
		{
			Rarity += 1;
		}
	}

	return FMath::Clamp(Rarity, 0, 5);
}

TMap<FString, float> URecipeSystem::CalculateBaseEffects(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives) const
{
	TMap<FString, float> Effects;

	// 基础药剂效果
	switch (BasePotion)
	{
	case EBasePotionType::Ricochet:
		Effects.Add(RecipeEffectKeys::BaseDamage, 10.0f);
		Effects.Add(RecipeEffectKeys::Speed, 500.0f);
		break;
	case EBasePotionType::Piercing:
		Effects.Add(RecipeEffectKeys::BaseDamage, 15.0f);
		Effects.Add(RecipeEffectKeys::Speed, 600.0f);
		break;
	case EBasePotionType::Explosive:
		Effects.Add(RecipeEffectKeys::BaseDamage, 20.0f);
		Effects.Add(RecipeEffectKeys::Speed, 400.0f);
		Effects.Add(RecipeEffectKeys::Radius, 200.0f);
		break;
	}

	// 添加剂效果
	float PotencyMultiplier = 1.0f;

	for (EAdditiveType Additive : Additives)
	{
		switch (Additive)
		{
		case EAdditiveType::Duration:
			PotencyMultiplier += 0.2f;
			break;
		case EAdditiveType::Amplify:
			Effects[RecipeEffectKeys::BaseDamage] *= 1.5f;
			break;
		case EAdditiveType::Split:
			Effects.Add(RecipeEffectKeys::SplitCount, 3.0f);
			break;
		case EAdditiveType::Chain:
			Effects.Add(RecipeEffectKeys::ChainCount, 5.0f);
			break;
		case EAdditiveType::Gravity:
			Effects.Add(RecipeEffectKeys::GravityStrength, 1000.0f);
			break;
		case EAdditiveType::Teleport:
			Effects.Add(RecipeEffectKeys::TeleportRange, 500.0f);
			break;
		case EAdditiveType::Speed:
			Effects[RecipeEffectKeys::Speed] *= 1.5f;
			break;
		case EAdditiveType::Slow:
			Effects[RecipeEffectKeys::Speed] *= 0.5f;
			break;
		case EAdditiveType::Fire:
			Effects.Add(RecipeEffectKeys::FireDamage, 15.0f);
			break;
		case EAdditiveType::Ice:
			Effects.Add(RecipeEffectKeys::IceDamage, 15.0f);
			break;
		case EAdditiveType::Lightning:
			Effects.Add(RecipeEffectKeys::LightningDamage, 15.0f);
			break;
		case EAdditiveType::Poison:
			Effects.Add(RecipeEffectKeys::PoisonDamage, 15.0f);
			break;
		default:
			break;
		}
	}

	// 应用药效倍率
	Effects.Add(RecipeEffectKeys::Potency, PotencyMultiplier);

	return Effects;
}
