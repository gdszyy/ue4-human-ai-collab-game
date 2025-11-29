// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldMorphing/WorldMorphingBlueprintLibrary.h"
#include "WorldMorphing/WorldMorphingSubsystem.h"
#include "Engine/GameInstance.h"

void UWorldMorphingBlueprintLibrary::InitializeWorld(UObject* WorldContextObject, int32 Width, int32 Height, const FSimulationParams& Params)
{
	if (!WorldContextObject) return;
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return;
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		Subsystem->InitializeWorld(Width, Height, Params);
	}
}

void UWorldMorphingBlueprintLibrary::TickSimulation(UObject* WorldContextObject, float DeltaTime)
{
	if (!WorldContextObject) return;
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return;
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		Subsystem->TickSimulation(DeltaTime);
	}
}

FCellState UWorldMorphingBlueprintLibrary::GetCellAt(UObject* WorldContextObject, int32 X, int32 Y)
{
	if (!WorldContextObject) return FCellState();
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return FCellState();
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		return Subsystem->GetCellAt(X, Y);
	}
	
	return FCellState();
}

void UWorldMorphingBlueprintLibrary::SetSimulationParams(UObject* WorldContextObject, const FSimulationParams& NewParams)
{
	if (!WorldContextObject) return;
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return;
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		Subsystem->SetSimulationParams(NewParams);
	}
}

FSimulationParams UWorldMorphingBlueprintLibrary::GetSimulationParams(UObject* WorldContextObject)
{
	if (!WorldContextObject) return FSimulationParams();
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return FSimulationParams();
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		return Subsystem->GetSimulationParams();
	}
	
	return FSimulationParams();
}

void UWorldMorphingBlueprintLibrary::GetGridSize(UObject* WorldContextObject, int32& OutWidth, int32& OutHeight)
{
	OutWidth = 0;
	OutHeight = 0;
	
	if (!WorldContextObject) return;
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return;
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		Subsystem->GetGridSize(OutWidth, OutHeight);
	}
}

int32 UWorldMorphingBlueprintLibrary::GetTimeStep(UObject* WorldContextObject)
{
	if (!WorldContextObject) return 0;
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return 0;
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		return Subsystem->GetTimeStep();
	}
	
	return 0;
}

int32 UWorldMorphingBlueprintLibrary::GetCycleCount(UObject* WorldContextObject)
{
	if (!WorldContextObject) return 0;
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return 0;
	
	UWorldMorphingSubsystem* Subsystem = GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
	if (Subsystem)
	{
		return Subsystem->GetCycleCount();
	}
	
	return 0;
}

FSimulationParams UWorldMorphingBlueprintLibrary::MakeDefaultParams()
{
	return FSimulationParams();
}
