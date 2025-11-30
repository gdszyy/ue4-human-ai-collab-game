// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemySystem.h"

void UAlchemySystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogTemp, Log, TEXT("AlchemySystem: Initializing..."));
	
	InitializeElements();
	InitializeRecipes();
	
	UE_LOG(LogTemp, Log, TEXT("AlchemySystem: Initialized with %d elements and %d recipes"), 
		ElementDatabase.Num(), RecipeDatabase.Num());
}

void UAlchemySystem::InitializeElements()
{
	// 初始化基础元素
	ElementDatabase.Add(TEXT("Fire"), FElement(TEXT("Fire"), TEXT("Fire"), 10));
	ElementDatabase.Add(TEXT("Water"), FElement(TEXT("Water"), TEXT("Water"), 10));
	ElementDatabase.Add(TEXT("Earth"), FElement(TEXT("Earth"), TEXT("Earth"), 10));
	ElementDatabase.Add(TEXT("Air"), FElement(TEXT("Air"), TEXT("Air"), 10));
	
	UE_LOG(LogTemp, Log, TEXT("AlchemySystem: Initialized %d base elements"), ElementDatabase.Num());
}

void UAlchemySystem::InitializeRecipes()
{
	// 初始化配方
	RecipeDatabase.Add(FRecipe(TEXT("Fire"), TEXT("Water"), FElement(TEXT("Steam"), TEXT("Hybrid"), 15)));
	RecipeDatabase.Add(FRecipe(TEXT("Earth"), TEXT("Water"), FElement(TEXT("Mud"), TEXT("Hybrid"), 12)));
	RecipeDatabase.Add(FRecipe(TEXT("Fire"), TEXT("Earth"), FElement(TEXT("Lava"), TEXT("Hybrid"), 18)));
	RecipeDatabase.Add(FRecipe(TEXT("Air"), TEXT("Water"), FElement(TEXT("Cloud"), TEXT("Hybrid"), 13)));
	
	UE_LOG(LogTemp, Log, TEXT("AlchemySystem: Initialized %d recipes"), RecipeDatabase.Num());
}

bool UAlchemySystem::CombineElements(const FString& Element1, const FString& Element2, FElement& OutResult)
{
	// 查找配方（支持双向匹配）
	for (const FRecipe& Recipe : RecipeDatabase)
	{
		if ((Recipe.Element1 == Element1 && Recipe.Element2 == Element2) ||
			(Recipe.Element1 == Element2 && Recipe.Element2 == Element1))
		{
			OutResult = Recipe.Result;
			
			UE_LOG(LogTemp, Log, TEXT("AlchemySystem: Successfully combined %s + %s = %s (Power: %d)"), 
				*Element1, *Element2, *OutResult.Name, OutResult.Power);
			
			return true;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("AlchemySystem: No recipe found for %s + %s"), *Element1, *Element2);
	return false;
}

TArray<FElement> UAlchemySystem::GetAllElements() const
{
	TArray<FElement> Elements;
	ElementDatabase.GenerateValueArray(Elements);
	return Elements;
}

TArray<FRecipe> UAlchemySystem::GetAllRecipes() const
{
	return RecipeDatabase;
}
