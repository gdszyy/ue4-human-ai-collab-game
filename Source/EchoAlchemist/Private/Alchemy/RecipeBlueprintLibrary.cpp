// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Alchemy/RecipeBlueprintLibrary.h"
#include "Alchemy/RecipeSystem.h"

FRecipeOutput URecipeBlueprintLibrary::EvaluateRecipe(UObject* WorldContextObject, const FRecipeInput& Input)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (!RecipeSystem)
	{
		return FRecipeOutput();
	}

	return RecipeSystem->EvaluateRecipe(Input);
}

FRecipeInput URecipeBlueprintLibrary::MakeRecipeInput(EBasePotionType BasePotion, const TArray<EAdditiveType>& Additives)
{
	FRecipeInput Input;
	Input.BasePotion = BasePotion;
	Input.Additives = Additives;
	return Input;
}

void URecipeBlueprintLibrary::AddWorldStateParameter(FRecipeInput& Input, const FString& Key, float Value)
{
	Input.WorldState.Add(Key, Value);
}

TArray<FRecipeDefinition> URecipeBlueprintLibrary::GetDiscoveredRecipes(UObject* WorldContextObject)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (!RecipeSystem)
	{
		return TArray<FRecipeDefinition>();
	}

	return RecipeSystem->GetDiscoveredRecipeDefinitions();
}

bool URecipeBlueprintLibrary::IsRecipeDiscovered(UObject* WorldContextObject, const FString& RecipeID)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (!RecipeSystem)
	{
		return false;
	}

	return RecipeSystem->IsRecipeDiscovered(RecipeID);
}

bool URecipeBlueprintLibrary::GetRecipeByID(UObject* WorldContextObject, const FString& RecipeID, FRecipeDefinition& OutRecipe)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (!RecipeSystem)
	{
		return false;
	}

	return RecipeSystem->GetRecipeByID(RecipeID, OutRecipe);
}

int32 URecipeBlueprintLibrary::GetRecipeUsageCount(UObject* WorldContextObject, const FString& RecipeID)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (!RecipeSystem)
	{
		return 0;
	}

	return RecipeSystem->GetRecipeUsageCount(RecipeID);
}

float URecipeBlueprintLibrary::GetEffectValue(const FRecipeOutput& Output, const FString& EffectKey, float DefaultValue)
{
	const float* Value = Output.Effects.Find(EffectKey);
	return Value ? *Value : DefaultValue;
}

bool URecipeBlueprintLibrary::HasEffect(const FRecipeOutput& Output, const FString& EffectKey)
{
	return Output.Effects.Contains(EffectKey);
}

bool URecipeBlueprintLibrary::SaveRecipeLibrary(UObject* WorldContextObject, const FString& SlotName)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (!RecipeSystem)
	{
		return false;
	}

	return RecipeSystem->SaveRecipeLibrary(SlotName);
}

bool URecipeBlueprintLibrary::LoadRecipeLibrary(UObject* WorldContextObject, const FString& SlotName)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (!RecipeSystem)
	{
		return false;
	}

	return RecipeSystem->LoadRecipeLibrary(SlotName);
}

void URecipeBlueprintLibrary::ResetRecipeLibrary(UObject* WorldContextObject)
{
	URecipeSystem* RecipeSystem = URecipeSystem::Get(WorldContextObject);
	if (RecipeSystem)
	{
		RecipeSystem->ResetRecipeLibrary();
	}
}

FString URecipeBlueprintLibrary::GetBasePotionDisplayName(EBasePotionType BasePotion)
{
	switch (BasePotion)
	{
	case EBasePotionType::Ricochet:
		return TEXT("弹射药剂");
	case EBasePotionType::Piercing:
		return TEXT("穿透药剂");
	case EBasePotionType::Explosive:
		return TEXT("爆炸药剂");
	default:
		return TEXT("未知药剂");
	}
}

FString URecipeBlueprintLibrary::GetAdditiveDisplayName(EAdditiveType Additive)
{
	switch (Additive)
	{
	case EAdditiveType::None:
		return TEXT("无");
	case EAdditiveType::Duration:
		return TEXT("持续");
	case EAdditiveType::Amplify:
		return TEXT("强化");
	case EAdditiveType::Split:
		return TEXT("分裂");
	case EAdditiveType::Chain:
		return TEXT("连锁");
	case EAdditiveType::Gravity:
		return TEXT("引力");
	case EAdditiveType::Teleport:
		return TEXT("传送");
	case EAdditiveType::Speed:
		return TEXT("加速");
	case EAdditiveType::Slow:
		return TEXT("减速");
	case EAdditiveType::Fire:
		return TEXT("火焰");
	case EAdditiveType::Ice:
		return TEXT("冰霜");
	case EAdditiveType::Lightning:
		return TEXT("雷电");
	case EAdditiveType::Poison:
		return TEXT("毒素");
	default:
		return TEXT("未知");
	}
}

FLinearColor URecipeBlueprintLibrary::GetRarityColor(int32 Rarity)
{
	switch (Rarity)
	{
	case 0:
		return FLinearColor::Gray;      // 普通 - 灰色
	case 1:
		return FLinearColor::White;     // 常见 - 白色
	case 2:
		return FLinearColor::Green;     // 稀有 - 绿色
	case 3:
		return FLinearColor::Blue;      // 精良 - 蓝色
	case 4:
		return FLinearColor(0.5f, 0.0f, 0.5f); // 史诗 - 紫色
	case 5:
		return FLinearColor(1.0f, 0.5f, 0.0f); // 传说 - 橙色
	default:
		return FLinearColor::White;
	}
}

FString URecipeBlueprintLibrary::GetRarityName(int32 Rarity)
{
	switch (Rarity)
	{
	case 0:
		return TEXT("普通");
	case 1:
		return TEXT("常见");
	case 2:
		return TEXT("稀有");
	case 3:
		return TEXT("精良");
	case 4:
		return TEXT("史诗");
	case 5:
		return TEXT("传说");
	default:
		return TEXT("未知");
	}
}
