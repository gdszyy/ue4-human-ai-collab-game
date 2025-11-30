// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldMorphing/WorldMorphingBlueprintLibrary.h"
#include "WorldMorphing/WorldMorphingSimulation.h"
#include "WorldMorphing/WorldMorphingVisualization.h"
#include "WorldMorphing/WorldMorphingConfiguration.h"

void UWorldMorphingBlueprintLibrary::InitializeWorld(UObject* WorldContextObject, int32 Width, int32 Height, const FSimulationParams& Params)
{
	// 调用新API
	UWorldMorphingSimulation::Initialize(WorldContextObject, Width, Height, Params);
}

void UWorldMorphingBlueprintLibrary::TickSimulation(UObject* WorldContextObject, float DeltaTime)
{
	// 调用新API
	UWorldMorphingSimulation::Tick(WorldContextObject, DeltaTime);
}

FCellState UWorldMorphingBlueprintLibrary::GetCellAt(UObject* WorldContextObject, int32 X, int32 Y)
{
	// 调用新API
	return UWorldMorphingVisualization::GetCellState(WorldContextObject, X, Y);
}

void UWorldMorphingBlueprintLibrary::SetSimulationParams(UObject* WorldContextObject, const FSimulationParams& NewParams)
{
	// 调用新API
	UWorldMorphingConfiguration::Apply(WorldContextObject, NewParams);
}

FSimulationParams UWorldMorphingBlueprintLibrary::GetSimulationParams(UObject* WorldContextObject)
{
	// 调用新API
	return UWorldMorphingConfiguration::GetCurrent(WorldContextObject);
}

void UWorldMorphingBlueprintLibrary::GetGridSize(UObject* WorldContextObject, int32& OutWidth, int32& OutHeight)
{
	// 调用新API
	FSimulationStatus Status = UWorldMorphingSimulation::GetStatus(WorldContextObject);
	OutWidth = Status.Width;
	OutHeight = Status.Height;
}

int32 UWorldMorphingBlueprintLibrary::GetTimeStep(UObject* WorldContextObject)
{
	// 调用新API
	FSimulationStatus Status = UWorldMorphingSimulation::GetStatus(WorldContextObject);
	return Status.TimeStep;
}

int32 UWorldMorphingBlueprintLibrary::GetCycleCount(UObject* WorldContextObject)
{
	// 调用新API
	FSimulationStatus Status = UWorldMorphingSimulation::GetStatus(WorldContextObject);
	return Status.CycleCount;
}

FSimulationParams UWorldMorphingBlueprintLibrary::MakeDefaultParams()
{
	// 调用新API
	return UWorldMorphingConfiguration::MakeDefault();
}
