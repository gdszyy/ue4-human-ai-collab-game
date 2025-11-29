// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldMorphing/WorldMorphingSubsystem.h"

void UWorldMorphingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	Width = 0;
	Height = 0;
	TimeStep = 0;
	CycleCount = 0;
	
	// 初始化Perlin噪声生成器
	PerlinNoise = MakeUnique<FPerlinNoise>();
	
	UE_LOG(LogTemp, Log, TEXT("WorldMorphingSubsystem Initialized"));
}

void UWorldMorphingSubsystem::Deinitialize()
{
	Grid.Empty();
	PerlinNoise.Reset();
	
	Super::Deinitialize();
}

void UWorldMorphingSubsystem::InitializeWorld(int32 InWidth, int32 InHeight, const FSimulationParams& InitParams)
{
	Width = InWidth;
	Height = InHeight;
	Params = InitParams;
	TimeStep = 0;
	CycleCount = 0;
	
	// 初始化噪声偏移
	NoiseOffsetX = FMath::FRand() * 1000.0f;
	NoiseOffsetY = FMath::FRand() * 1000.0f;
	
	// 初始化边缘供给点
	EdgeSupplyPoints.Empty();
	for (int32 i = 0; i < Params.EdgeSupplyPointCount; ++i)
	{
		float Angle = FMath::FRand() * PI * 2.0f;
		float Speed = (FMath::FRand() - 0.5f) * Params.EdgeSupplyPointSpeed;
		EdgeSupplyPoints.Add(FEdgeSupplyPoint(Angle, Speed));
	}
	
	// 初始化网格
	Grid.SetNum(Height);
	const float CenterX = Width / 2.0f;
	const float CenterY = Height / 2.0f;
	const float InitialRadius = FMath::Min(Width, Height) * 0.4f;
	
	for (int32 Y = 0; Y < Height; ++Y)
	{
		Grid[Y].SetNum(Width);
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			Cell.X = X;
			Cell.Y = Y;
			
			// 计算到中心的距离
			float Dist = FMath::Sqrt(FMath::Square(X - CenterX) + FMath::Square(Y - CenterY));
			Cell.bExists = Dist < InitialRadius;
			
			if (Cell.bExists)
			{
				Cell.MantleEnergy = 50.0f + FMath::FRand() * 20.0f;
				
				// 中心区域初始化Alpha晶石
				if (Dist < 3.0f)
				{
					Cell.CrystalState = ECrystalType::Alpha;
				}
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("World Initialized: %dx%d"), Width, Height);
}

void UWorldMorphingSubsystem::TickSimulation(float DeltaTime)
{
	if (Width == 0 || Height == 0)
	{
		return;
	}
	
	TimeStep++;
	if (TimeStep % 1000 == 0)
	{
		CycleCount++;
	}
	
	// 更新各层
	UpdateMantleLayer();
	UpdateClimateLayer();
	UpdateCrystalLayer();
	UpdateHumanLayer();
}

FCellState UWorldMorphingSubsystem::GetCellAt(int32 X, int32 Y) const
{
	if (!IsValidCoord(X, Y))
	{
		return FCellState();
	}
	
	return Grid[Y][X].ToBlueprintState();
}

void UWorldMorphingSubsystem::SetSimulationParams(const FSimulationParams& NewParams)
{
	Params = NewParams;
}

void UWorldMorphingSubsystem::GetGridSize(int32& OutWidth, int32& OutHeight) const
{
	OutWidth = Width;
	OutHeight = Height;
}

bool UWorldMorphingSubsystem::IsValidCoord(int32 X, int32 Y) const
{
	return X >= 0 && X < Width && Y >= 0 && Y < Height;
}

TArray<FCell*> UWorldMorphingSubsystem::GetNeighbors(int32 X, int32 Y)
{
	TArray<FCell*> Neighbors;
	
	// 8邻域
	for (int32 DY = -1; DY <= 1; ++DY)
	{
		for (int32 DX = -1; DX <= 1; ++DX)
		{
			if (DX == 0 && DY == 0) continue;
			
			int32 NX = X + DX;
			int32 NY = Y + DY;
			
			if (IsValidCoord(NX, NY))
			{
				Neighbors.Add(&Grid[NY][NX]);
			}
		}
	}
	
	return Neighbors;
}

TArray<const FCell*> UWorldMorphingSubsystem::GetNeighbors(int32 X, int32 Y) const
{
	TArray<const FCell*> Neighbors;
	
	// 8邻域
	for (int32 DY = -1; DY <= 1; ++DY)
	{
		for (int32 DX = -1; DX <= 1; ++DX)
		{
			if (DX == 0 && DY == 0) continue;
			
			int32 NX = X + DX;
			int32 NY = Y + DY;
			
			if (IsValidCoord(NX, NY))
			{
				Neighbors.Add(&Grid[NY][NX]);
			}
		}
	}
	
	return Neighbors;
}

// ========== 地幔层更新 ==========
void UWorldMorphingSubsystem::UpdateMantleLayer()
{
	const float CenterX = Width / 2.0f;
	const float CenterY = Height / 2.0f;
	
	// 1. Cahn-Hilliard 相分离 + 扩散
	const float DiffusionCoeff = 0.2f;
	TArray<TArray<float>> EnergyChanges;
	EnergyChanges.SetNum(Height);
	for (int32 Y = 0; Y < Height; ++Y)
	{
		EnergyChanges[Y].SetNum(Width);
		for (int32 X = 0; X < Width; ++X)
		{
			EnergyChanges[Y][X] = 0.0f;
		}
	}
	
	// 计算能量流动
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (!Cell.bExists) continue;
			
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			TArray<FCell*> ExistingNeighbors;
			for (FCell* N : Neighbors)
			{
				if (N->bExists) ExistingNeighbors.Add(N);
			}
			
			if (ExistingNeighbors.Num() == 0) continue;
			
			// 缓慢迁徙偏置
			float Time = TimeStep * Params.MantleTimeScale;
			float BiasX = FMath::Sin(Time * 0.5f);
			float BiasY = FMath::Cos(Time * 0.5f);
			
			// 与每个邻居交换能量
			for (FCell* Neighbor : ExistingNeighbors)
			{
				// 能量差
				float Diff = Neighbor->MantleEnergy - Cell.MantleEnergy;
				
				// 基础扩散
				float Flow = Diff * DiffusionCoeff * 0.1f;
				
				// 迁徙偏置
				float DX = Neighbor->X - Cell.X;
				float DY = Neighbor->Y - Cell.Y;
				float BiasFlow = (DX * BiasX + DY * BiasY) * 0.05f;
				
				Flow += BiasFlow;
				
				// 限制流速
				Flow = FMath::Clamp(Flow, -2.0f, 2.0f);
				
				// 累积变化
				EnergyChanges[Y][X] += Flow;
				EnergyChanges[Neighbor->Y][Neighbor->X] -= Flow;
			}
		}
	}
	
	// 应用能量变化
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (Cell.bExists)
			{
				Cell.MantleEnergy += EnergyChanges[Y][X];
				Cell.MantleEnergy = FMath::Clamp(Cell.MantleEnergy, 0.0f, 150.0f);
			}
		}
	}
	
	// 2. 边缘能量生成机制
	// 更新供给点位置
	for (FEdgeSupplyPoint& Point : EdgeSupplyPoints)
	{
		Point.Angle += Point.Speed * (FMath::FRand() * 0.5f + 0.75f);
		if (Point.Angle > PI * 2.0f) Point.Angle -= PI * 2.0f;
		if (Point.Angle < 0.0f) Point.Angle += PI * 2.0f;
		
		// 偶尔改变速度方向
		if (FMath::FRand() < 0.01f)
		{
			Point.Speed = (FMath::FRand() - 0.5f) * Params.EdgeSupplyPointSpeed;
		}
	}
	
	if (Params.EdgeGenerationEnergy > 0.0f)
	{
		// Lambda: 计算某角度受到所有供给点的影响密度
		auto GetDensity = [this](float Angle) -> float
		{
			float Density = 0.0f;
			for (const FEdgeSupplyPoint& Point : EdgeSupplyPoints)
			{
				float Diff = FMath::Abs(Angle - Point.Angle);
				if (Diff > PI) Diff = PI * 2.0f - Diff;
				
				float Sigma = Params.EdgeGenerationWidth * 0.15f;
				Density += FMath::Exp(-(Diff * Diff) / (2.0f * Sigma * Sigma));
			}
			return Density;
		};
		
		// 计算每个点到最近边缘的距离(BFS)
		TArray<TArray<int32>> DistToEdge;
		DistToEdge.SetNum(Height);
		for (int32 Y = 0; Y < Height; ++Y)
		{
			DistToEdge[Y].SetNum(Width);
			for (int32 X = 0; X < Width; ++X)
			{
				DistToEdge[Y][X] = MAX_int32;
			}
		}
		
		TArray<FIntVector> Queue;
		
		// 初始化: 找到所有直接边缘点
		for (int32 Y = 0; Y < Height; ++Y)
		{
			for (int32 X = 0; X < Width; ++X)
			{
				const FCell& Cell = Grid[Y][X];
				if (!Cell.bExists) continue;
				
				TArray<FCell*> Neighbors = GetNeighbors(X, Y);
				bool IsEdge = false;
				for (FCell* N : Neighbors)
				{
					if (!N->bExists)
					{
						IsEdge = true;
						break;
					}
				}
				
				if (IsEdge || Neighbors.Num() < 8)
				{
					DistToEdge[Y][X] = 0;
					Queue.Add(FIntVector(X, Y, 0));
				}
			}
		}
		
		// BFS扩散距离
		int32 Head = 0;
		int32 MaxDist = Params.EdgeGenerationOffset + Params.EdgeGenerationWidth + 1;
		while (Head < Queue.Num())
		{
			FIntVector Current = Queue[Head++];
			int32 X = Current.X;
			int32 Y = Current.Y;
			int32 Dist = Current.Z;
			
			if (Dist >= MaxDist) continue;
			
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			for (FCell* N : Neighbors)
			{
				if (N->bExists && DistToEdge[N->Y][N->X] == MAX_int32)
				{
					DistToEdge[N->Y][N->X] = Dist + 1;
					Queue.Add(FIntVector(N->X, N->Y, Dist + 1));
				}
			}
		}
		
		// 应用能量供给
		for (int32 Y = 0; Y < Height; ++Y)
		{
			for (int32 X = 0; X < Width; ++X)
			{
				FCell& Cell = Grid[Y][X];
				if (!Cell.bExists) continue;
				
				int32 Dist = DistToEdge[Y][X];
				
				// 判断是否在生成范围内
				if (Dist >= Params.EdgeGenerationOffset && Dist < Params.EdgeGenerationOffset + Params.EdgeGenerationWidth)
				{
					// 计算角度
					float DX = X - CenterX;
					float DY = Y - CenterY;
					float Angle = FMath::Atan2(DY, DX);
					if (Angle < 0.0f) Angle += PI * 2.0f;
					
					// 获取供给密度
					float Density = GetDensity(Angle);
					
					// 应用能量供给
					Cell.MantleEnergy += Params.EdgeGenerationEnergy * Density;
				}
			}
		}
	}
	
	// 3. 边缘扩张/缩减逻辑
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (!Cell.bExists) continue;
			
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			bool IsEdge = false;
			for (FCell* N : Neighbors)
			{
				if (!N->bExists)
				{
					IsEdge = true;
					break;
				}
			}
			
			if (IsEdge)
			{
				// 扩张逻辑
				if (Cell.MantleEnergy > Params.ExpansionThreshold)
				{
					TArray<FCell*> EmptyNeighbors;
					for (FCell* N : Neighbors)
					{
						if (!N->bExists)
						{
							float NDist = FMath::Sqrt(FMath::Square(N->X - CenterX) + FMath::Square(N->Y - CenterY));
							if (NDist <= Params.MaxRadius)
							{
								EmptyNeighbors.Add(N);
							}
						}
					}
					
					if (EmptyNeighbors.Num() > 0)
					{
						FCell* Target = EmptyNeighbors[FMath::RandRange(0, EmptyNeighbors.Num() - 1)];
						Target->bExists = true;
						Target->MantleEnergy = Cell.MantleEnergy * 0.5f;
						Cell.MantleEnergy *= 0.5f;
					}
				}
				// 缩减逻辑
				else if (Cell.MantleEnergy < Params.ShrinkThreshold)
				{
					float Dist = FMath::Sqrt(FMath::Square(X - CenterX) + FMath::Square(Y - CenterY));
					if (Dist > Params.MinRadius)
					{
						// 能量回流给邻居
						TArray<FCell*> ExistingNeighbors;
						for (FCell* N : Neighbors)
						{
							if (N->bExists) ExistingNeighbors.Add(N);
						}
						
						if (ExistingNeighbors.Num() > 0)
						{
							float EnergyPerNeighbor = Cell.MantleEnergy / ExistingNeighbors.Num();
							for (FCell* N : ExistingNeighbors)
							{
								N->MantleEnergy += EnergyPerNeighbor;
							}
						}
						
						Cell.bExists = false;
						Cell.MantleEnergy = 0.0f;
						Cell.CrystalState = ECrystalType::Empty;
					}
				}
			}
		}
	}
}

// ========== 气候层更新 ==========
void UWorldMorphingSubsystem::UpdateClimateLayer()
{
	// 季节性偏移
	float TimeCycle = (TimeStep % 1000) / 1000.0f;
	float SeasonalOffset = Params.SeasonalAmplitude * FMath::Sin(2.0f * PI * TimeCycle);
	
	// 第一遍: 计算基础温度和扩散
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			
			if (!Cell.bExists)
			{
				Cell.Temperature = -50.0f;
				Cell.bHasThunderstorm = false;
				continue;
			}
			
			// 基础温度来自地幔能量
			float NormalizedEnergy = FMath::Min(100.0f, Cell.MantleEnergy);
			Cell.BaseTemperature = (NormalizedEnergy - 50.0f) * 0.5f + SeasonalOffset;
			
			// 扩散计算
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			TArray<FCell*> ExistingNeighbors;
			for (FCell* N : Neighbors)
			{
				if (N->bExists) ExistingNeighbors.Add(N);
			}
			
			if (ExistingNeighbors.Num() > 0)
			{
				float AvgTemp = 0.0f;
				for (FCell* N : ExistingNeighbors)
				{
					AvgTemp += N->Temperature;
				}
				AvgTemp /= ExistingNeighbors.Num();
				
				Cell.TemperatureChange = Params.DiffusionRate * (AvgTemp - Cell.Temperature);
			}
			
			// 晶石的冷却效应
			if (Cell.CrystalState == ECrystalType::Alpha)
			{
				Cell.TemperatureChange -= 0.5f;
			}
			else if (Cell.CrystalState == ECrystalType::Beta)
			{
				Cell.TemperatureChange -= 0.2f;
			}
			
			// 应用温度变化
			Cell.Temperature += Cell.TemperatureChange;
			Cell.Temperature += (Cell.BaseTemperature - Cell.Temperature) * 0.1f;
			
			// 温度限制
			Cell.Temperature = FMath::Clamp(Cell.Temperature, -50.0f, 50.0f);
		}
	}
	
	// 第二遍: 计算雷暴
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (!Cell.bExists) continue;
			
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			float MaxDiff = 0.0f;
			for (FCell* N : Neighbors)
			{
				if (N->bExists)
				{
					MaxDiff = FMath::Max(MaxDiff, FMath::Abs(N->Temperature - Cell.Temperature));
				}
			}
			
			Cell.bHasThunderstorm = MaxDiff > Params.ThunderstormThreshold;
		}
	}
}

// ========== 晶石层更新 ==========
void UWorldMorphingSubsystem::UpdateCrystalLayer()
{
	// 1. 能量获取与消耗
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			
			if (!Cell.bExists || Cell.CrystalState != ECrystalType::Alpha)
			{
				Cell.bIsAbsorbing = false;
				Cell.CrystalEnergy = 0.0f;
				continue;
			}
			
			// 吸收地幔能量
			float EnergyInput = 0.0f;
			float Absorbed = Cell.MantleEnergy * Params.MantleAbsorption;
			EnergyInput += Absorbed;
			
			if (Absorbed > 0.1f)
			{
				Cell.MantleEnergy = FMath::Max(0.0f, Cell.MantleEnergy - Absorbed);
				Cell.bIsAbsorbing = true;
			}
			else
			{
				Cell.bIsAbsorbing = false;
			}
			
			// 雷暴能量
			if (Cell.bHasThunderstorm)
			{
				EnergyInput += Params.ThunderstormEnergy;
			}
			
			// 记录输入用于可视化
			Cell.CrystalEnergy = EnergyInput;
			
			// 能量结算
			float NetEnergy = EnergyInput - Params.AlphaEnergyDemand;
			Cell.StoredEnergy += NetEnergy;
			
			// 能量上限
			Cell.StoredEnergy = FMath::Min(Cell.StoredEnergy, Params.MaxCrystalEnergy);
		}
	}
	
	// 1.5 能量共享
	TArray<TArray<float>> EnergyChanges;
	EnergyChanges.SetNum(Height);
	for (int32 Y = 0; Y < Height; ++Y)
	{
		EnergyChanges[Y].SetNum(Width);
		for (int32 X = 0; X < Width; ++X)
		{
			EnergyChanges[Y][X] = 0.0f;
		}
	}
	
	// 清空能量流记录
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			Grid[Y][X].EnergyFlow.Empty();
		}
	}
	
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (!Cell.bExists || Cell.CrystalState != ECrystalType::Alpha) continue;
			
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			for (FCell* Neighbor : Neighbors)
			{
				if (!Neighbor->bExists || Neighbor->CrystalState != ECrystalType::Alpha) continue;
				
				float Diff = Neighbor->StoredEnergy - Cell.StoredEnergy;
				if (Diff > 0.0f)
				{
					float Flow = Diff * Params.EnergySharingRate;
					
					// 检查接收方是否超过共享上限
					float Limit = Params.MaxCrystalEnergy * Params.EnergySharingLimit;
					if (Cell.StoredEnergy + Flow > Limit)
					{
						continue;
					}
					
					EnergyChanges[Y][X] += Flow;
					EnergyChanges[Neighbor->Y][Neighbor->X] -= Flow;
					
					// 记录流向
					Neighbor->EnergyFlow.Add(FVector(Cell.X, Cell.Y, Flow));
				}
			}
		}
	}
	
	// 应用能量共享
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (!Cell.bExists || Cell.CrystalState != ECrystalType::Alpha) continue;
			
			Cell.StoredEnergy += EnergyChanges[Y][X];
			Cell.StoredEnergy = FMath::Clamp(Cell.StoredEnergy, 0.0f, Params.MaxCrystalEnergy);
		}
	}
	
	// 2. 状态转移
	TArray<TArray<ECrystalType>> NextStates;
	TArray<TArray<float>> NextStoredEnergy;
	NextStates.SetNum(Height);
	NextStoredEnergy.SetNum(Height);
	
	for (int32 Y = 0; Y < Height; ++Y)
	{
		NextStates[Y].SetNum(Width);
		NextStoredEnergy[Y].SetNum(Width);
		for (int32 X = 0; X < Width; ++X)
		{
			NextStates[Y][X] = Grid[Y][X].CrystalState;
			NextStoredEnergy[Y][X] = Grid[Y][X].StoredEnergy;
		}
	}
	
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (!Cell.bExists) continue;
			
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			TArray<FCell*> ExistingNeighbors;
			TArray<FCell*> AlphaNeighbors;
			TArray<FCell*> BetaNeighbors;
			
			for (FCell* N : Neighbors)
			{
				if (N->bExists)
				{
					ExistingNeighbors.Add(N);
					if (N->CrystalState == ECrystalType::Alpha) AlphaNeighbors.Add(N);
					if (N->CrystalState == ECrystalType::Beta) BetaNeighbors.Add(N);
				}
			}
			
			if (Cell.CrystalState == ECrystalType::Empty)
			{
				// 规则1: 扩张
				TArray<FCell*> RichNeighbors;
				for (FCell* N : AlphaNeighbors)
				{
					if (N->StoredEnergy >= Params.ExpansionCost)
					{
						RichNeighbors.Add(N);
					}
				}
				
				if (RichNeighbors.Num() > 0 && FMath::FRand() < 0.3f)
				{
					FCell* Parent = RichNeighbors[FMath::RandRange(0, RichNeighbors.Num() - 1)];
					NextStates[Y][X] = ECrystalType::Alpha;
					NextStoredEnergy[Y][X] = 5.0f;
					NextStoredEnergy[Parent->Y][Parent->X] -= Params.ExpansionCost;
				}
			}
			else if (Cell.CrystalState == ECrystalType::Alpha)
			{
				// 规则2: 硬化
				if (Cell.StoredEnergy <= 0.0f)
				{
					NextStates[Y][X] = ECrystalType::Beta;
					NextStoredEnergy[Y][X] = 0.0f;
				}
				
				// 规则3: 孤立死亡
				if (AlphaNeighbors.Num() == 0 && BetaNeighbors.Num() < 2 && Cell.StoredEnergy < 5.0f)
				{
					NextStates[Y][X] = ECrystalType::Empty;
					NextStoredEnergy[Y][X] = 0.0f;
				}
			}
			// Beta晶石不可逆,保持不变
		}
	}
	
	// 应用状态转移
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			Grid[Y][X].CrystalState = NextStates[Y][X];
			Grid[Y][X].StoredEnergy = FMath::Max(0.0f, NextStoredEnergy[Y][X]);
		}
	}
}

// ========== 人类层更新 ==========
void UWorldMorphingSubsystem::UpdateHumanLayer()
{
	// 1. 检查是否需要初始化人类
	int32 HumanCount = 0;
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			if (Grid[Y][X].CrystalState == ECrystalType::Human)
			{
				HumanCount++;
			}
		}
	}
	
	if (HumanCount == 0)
	{
		// 随机生成一个人类聚落
		int32 Attempts = 0;
		while (Attempts < 100)
		{
			int32 RX = FMath::RandRange(0, Width - 1);
			int32 RY = FMath::RandRange(0, Height - 1);
			FCell& Cell = Grid[RY][RX];
			
			bool IsTempSuitable = Cell.Temperature >= Params.HumanMinTemp && Cell.Temperature <= Params.HumanMaxTemp;
			
			if (Cell.bExists && Cell.CrystalState != ECrystalType::Alpha && (IsTempSuitable || Attempts > 50))
			{
				Cell.CrystalState = ECrystalType::Human;
				Cell.Prosperity = 50.0f;
				break;
			}
			Attempts++;
		}
		return; // 这一帧只做初始化
	}
	
	// 2. 更新人类状态
	struct FHumanChange
	{
		int32 X;
		int32 Y;
		enum EType { Prosperity, State, Migrate } Type;
		float Value;
		int32 ToX;
		int32 ToY;
		
		FHumanChange() : X(0), Y(0), Type(Prosperity), Value(0.0f), ToX(0), ToY(0) {}
	};
	
	TArray<FHumanChange> Changes;
	
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			FCell& Cell = Grid[Y][X];
			if (Cell.CrystalState != ECrystalType::Human) continue;
			
			// A. 温度检查
			if (Cell.Temperature < Params.HumanSurvivalMinTemp || Cell.Temperature > Params.HumanSurvivalMaxTemp)
			{
				// 极端温度,直接抹杀
				FHumanChange Change;
				Change.X = X;
				Change.Y = Y;
				Change.Type = FHumanChange::State;
				Change.Value = 0.0f; // 变为Empty
				Changes.Add(Change);
				continue;
			}
			
			// B. 繁荣度更新
			float ProsperityChange = 0.0f;
			if (Cell.Temperature >= Params.HumanMinTemp && Cell.Temperature <= Params.HumanMaxTemp)
			{
				ProsperityChange += Params.HumanProsperityGrowth;
			}
			else
			{
				ProsperityChange -= Params.HumanProsperityDecay;
			}
			
			// 邻居加成
			TArray<FCell*> Neighbors = GetNeighbors(X, Y);
			int32 HumanNeighborCount = 0;
			for (FCell* N : Neighbors)
			{
				if (N->CrystalState == ECrystalType::Human) HumanNeighborCount++;
			}
			ProsperityChange += HumanNeighborCount * 0.1f;
			
			// C. 采矿(消除相邻Beta晶石)
			TArray<FCell*> BetaNeighbors;
			for (FCell* N : Neighbors)
			{
				if (N->CrystalState == ECrystalType::Beta) BetaNeighbors.Add(N);
			}
			
			if (BetaNeighbors.Num() > 0)
			{
				FCell* Target = BetaNeighbors[FMath::RandRange(0, BetaNeighbors.Num() - 1)];
				FHumanChange Change;
				Change.X = Target->X;
				Change.Y = Target->Y;
				Change.Type = FHumanChange::State;
				Change.Value = 0.0f; // 变为Empty
				Changes.Add(Change);
				ProsperityChange += Params.HumanMiningReward;
			}
			
			// 应用繁荣度变化
			FHumanChange ProsperityUpdate;
			ProsperityUpdate.X = X;
			ProsperityUpdate.Y = Y;
			ProsperityUpdate.Type = FHumanChange::Prosperity;
			ProsperityUpdate.Value = Cell.Prosperity + ProsperityChange;
			Changes.Add(ProsperityUpdate);
			
			// D. 扩张
			if (Cell.Prosperity + ProsperityChange > Params.HumanExpansionThreshold)
			{
				TArray<FCell*> ValidTargets;
				for (FCell* N : Neighbors)
				{
					if (N->bExists && N->CrystalState != ECrystalType::Alpha && N->CrystalState != ECrystalType::Human)
					{
						ValidTargets.Add(N);
					}
				}
				
				if (ValidTargets.Num() > 0)
				{
					FCell* Target = ValidTargets[FMath::RandRange(0, ValidTargets.Num() - 1)];
					FHumanChange ExpansionChange;
					ExpansionChange.X = Target->X;
					ExpansionChange.Y = Target->Y;
					ExpansionChange.Type = FHumanChange::State;
					ExpansionChange.Value = 1.0f; // 变为Human
					Changes.Add(ExpansionChange);
					
					// 扩张消耗繁荣度
					ProsperityUpdate.Value *= 0.6f;
				}
			}
			
			// E. 迁移
			if (Cell.Prosperity + ProsperityChange < Params.HumanMigrationThreshold)
			{
				// 寻找更好的位置
				FCell* BestNeighbor = nullptr;
				float BestScore = -1000.0f;
				
				for (FCell* N : Neighbors)
				{
					if (!N->bExists || N->CrystalState != ECrystalType::Empty) continue;
					
					float Score = 0.0f;
					if (N->Temperature >= Params.HumanMinTemp && N->Temperature <= Params.HumanMaxTemp)
					{
						Score += 10.0f;
					}
					
					// 计算Beta邻居数量
					TArray<FCell*> NNeighbors = GetNeighbors(N->X, N->Y);
					for (FCell* NN : NNeighbors)
					{
						if (NN->CrystalState == ECrystalType::Beta) Score += 1.0f;
					}
					
					if (Score > BestScore)
					{
						BestScore = Score;
						BestNeighbor = N;
					}
				}
				
				if (BestNeighbor != nullptr && BestScore > 0.0f)
				{
					FHumanChange MigrateChange;
					MigrateChange.X = X;
					MigrateChange.Y = Y;
					MigrateChange.Type = FHumanChange::Migrate;
					MigrateChange.Value = Cell.Prosperity * 0.8f;
					MigrateChange.ToX = BestNeighbor->X;
					MigrateChange.ToY = BestNeighbor->Y;
					Changes.Add(MigrateChange);
				}
			}
		}
	}
	
	// 3. 应用变更
	for (const FHumanChange& Change : Changes)
	{
		FCell& TargetCell = Grid[Change.Y][Change.X];
		
		if (Change.Type == FHumanChange::State)
		{
			if (Change.Value == 0.0f)
			{
				TargetCell.CrystalState = ECrystalType::Empty;
				TargetCell.Prosperity = 0.0f;
			}
			else if (Change.Value == 1.0f)
			{
				TargetCell.CrystalState = ECrystalType::Human;
				TargetCell.Prosperity = 50.0f;
			}
		}
		else if (Change.Type == FHumanChange::Prosperity)
		{
			TargetCell.Prosperity = FMath::Clamp(Change.Value, 0.0f, 100.0f);
		}
		else if (Change.Type == FHumanChange::Migrate)
		{
			if (TargetCell.CrystalState == ECrystalType::Human)
			{
				FCell& DestCell = Grid[Change.ToY][Change.ToX];
				if (DestCell.CrystalState == ECrystalType::Empty)
				{
					DestCell.CrystalState = ECrystalType::Human;
					DestCell.Prosperity = Change.Value;
					TargetCell.CrystalState = ECrystalType::Empty;
					TargetCell.Prosperity = 0.0f;
				}
			}
		}
	}
}
