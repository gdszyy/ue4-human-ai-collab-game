// Copyright Epic Games, Inc. All Rights Reserved.
// 世界变迁系统 - 核心数据类型定义

#pragma once

#include "CoreMinimal.h"
#include "WorldMorphingTypes.generated.h"

/**
 * 晶石类型枚举
 */
UENUM(BlueprintType)
enum class ECrystalType : uint8
{
	Empty UMETA(DisplayName = "Empty"),      // 空白
	Alpha UMETA(DisplayName = "Alpha"),      // Alpha晶石(活体)
	Beta UMETA(DisplayName = "Beta"),        // Beta晶石(硬化)
	Human UMETA(DisplayName = "Human")       // 人类聚落
};

/**
 * 单元格状态结构体
 * 用于蓝图读取单个格子的完整状态
 */
USTRUCT(BlueprintType)
struct FCellState
{
	GENERATED_BODY()

	// 地幔层
	UPROPERTY(BlueprintReadOnly, Category = "Mantle")
	bool bExists = false;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle")
	float MantleEnergy = 0.0f;

	// 气候层
	UPROPERTY(BlueprintReadOnly, Category = "Climate")
	float Temperature = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Climate")
	bool bHasThunderstorm = false;

	// 晶石层
	UPROPERTY(BlueprintReadOnly, Category = "Crystal")
	ECrystalType CrystalType = ECrystalType::Empty;

	UPROPERTY(BlueprintReadOnly, Category = "Crystal")
	float StoredEnergy = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Crystal")
	bool bIsAbsorbing = false;

	// 人类层
	UPROPERTY(BlueprintReadOnly, Category = "Human")
	float Prosperity = 0.0f;
};

/**
 * 模拟参数结构体
 * 包含所有可调参数
 */
USTRUCT(BlueprintType)
struct FSimulationParams
{
	GENERATED_BODY()

	// ========== 地幔层参数 ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float MantleTimeScale = 0.002f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float ExpansionThreshold = 123.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float ShrinkThreshold = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float MantleEnergyLevel = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float MaxRadius = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float MinRadius = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float DistortionSpeed = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	int32 EdgeGenerationWidth = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float EdgeGenerationEnergy = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	int32 EdgeGenerationOffset = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	int32 EdgeSupplyPointCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle")
	float EdgeSupplyPointSpeed = 0.05f;

	// ========== 气候层参数 ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climate")
	float DiffusionRate = 0.12f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climate")
	float AdvectionRate = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climate")
	float ThunderstormThreshold = 18.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climate")
	float SeasonalAmplitude = 5.0f;

	// ========== 晶石层参数 ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float AlphaEnergyDemand = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float BetaEnergyDemand = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float MantleAbsorption = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float ThunderstormEnergy = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float ExpansionCost = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float MaxCrystalEnergy = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float EnergySharingRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float EnergySharingLimit = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal")
	float HarvestThreshold = 0.8f;

	// ========== 人类层参数 ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanMinTemp = -10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanMaxTemp = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanSurvivalMinTemp = -30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanSurvivalMaxTemp = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanProsperityGrowth = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanProsperityDecay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanExpansionThreshold = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanMiningReward = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Human")
	float HumanMigrationThreshold = 40.0f;
};

/**
 * 内部单元格数据
 * 用于模拟引擎内部存储,不暴露给蓝图
 */
struct FCell
{
	int32 X;
	int32 Y;

	// 地幔层
	bool bExists;
	float MantleEnergy;
	float ExpansionPotential;
	float ExpansionAccumulator;
	float ShrinkAccumulator;

	// 气候层
	float Temperature;
	float BaseTemperature;
	float TemperatureChange;
	bool bHasThunderstorm;

	// 晶石层
	ECrystalType CrystalState;
	float CrystalEnergy;
	float StoredEnergy;
	bool bIsAbsorbing;
	TArray<FVector> EnergyFlow;

	// 人类层
	float Prosperity;

	FCell()
		: X(0), Y(0)
		, bExists(false)
		, MantleEnergy(0.0f)
		, ExpansionPotential(0.0f)
		, ExpansionAccumulator(0.0f)
		, ShrinkAccumulator(0.0f)
		, Temperature(0.0f)
		, BaseTemperature(0.0f)
		, TemperatureChange(0.0f)
		, bHasThunderstorm(false)
		, CrystalState(ECrystalType::Empty)
		, CrystalEnergy(0.0f)
		, StoredEnergy(10.0f)
		, bIsAbsorbing(false)
		, Prosperity(0.0f)
	{
	}

	// 转换为蓝图可读的状态
	FCellState ToBlueprintState() const
	{
		FCellState State;
		State.bExists = bExists;
		State.MantleEnergy = MantleEnergy;
		State.Temperature = Temperature;
		State.bHasThunderstorm = bHasThunderstorm;
		State.CrystalType = CrystalState;
		State.StoredEnergy = StoredEnergy;
		State.bIsAbsorbing = bIsAbsorbing;
		State.Prosperity = Prosperity;
		return State;
	}
};

/**
 * 模拟状态结构体
 * 用于获取模拟的整体状态信息
 */
USTRUCT(BlueprintType)
struct FSimulationStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	bool bInitialized = false;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	bool bPaused = false;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	int32 Width = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	int32 Height = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	int32 TimeStep = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	int32 CycleCount = 0;
};

/**
 * 世界统计信息结构体
 * 用于获取世界的统计数据
 */
USTRUCT(BlueprintType)
struct FWorldStatistics
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	int32 TotalCells = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	int32 TerrainCells = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	int32 AlphaCrystals = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	int32 BetaCrystals = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	int32 HumanSettlements = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	int32 ThunderstormCells = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	float AverageMantleEnergy = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Statistics")
	float AverageTemperature = 0.0f;
};

/**
 * 热力图数据类型枚举
 */
UENUM(BlueprintType)
enum class EHeatmapDataType : uint8
{
	MantleEnergy    UMETA(DisplayName = "Mantle Energy"),
	Temperature     UMETA(DisplayName = "Temperature"),
	CrystalDensity  UMETA(DisplayName = "Crystal Density"),
	HumanDensity    UMETA(DisplayName = "Human Density")
};

/**
 * 模拟预设枚举
 */
UENUM(BlueprintType)
enum class ESimulationPreset : uint8
{
	Default         UMETA(DisplayName = "Default"),
	FastGrowth      UMETA(DisplayName = "Fast Growth"),
	SlowEvolution   UMETA(DisplayName = "Slow Evolution"),
	HighEnergy      UMETA(DisplayName = "High Energy"),
	Stable          UMETA(DisplayName = "Stable")
};
