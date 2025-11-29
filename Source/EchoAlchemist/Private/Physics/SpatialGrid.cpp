// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/SpatialGrid.h"

FSpatialGrid::FSpatialGrid(const FBox& InBounds, float InCellSize)
	: Bounds(InBounds)
	, CellSize(InCellSize)
{
	// 计算网格维度
	FVector Size = Bounds.GetSize();
	GridDimensions.X = FMath::CeilToInt(Size.X / CellSize);
	GridDimensions.Y = FMath::CeilToInt(Size.Y / CellSize);
	GridDimensions.Z = FMath::CeilToInt(Size.Z / CellSize);

	// 预分配空间
	Grid.Reserve(GridDimensions.X * GridDimensions.Y * GridDimensions.Z / 4);
}

void FSpatialGrid::Clear()
{
	Grid.Empty();
}

void FSpatialGrid::Insert(const FCollisionBody& Body)
{
	// 获取碰撞体的边界盒
	FBox BodyBounds = Body.GetBoundingBox();

	// 获取边界盒覆盖的网格范围
	FIntVector MinGrid, MaxGrid;
	GetGridRange(BodyBounds, MinGrid, MaxGrid);

	// 将碰撞体插入到所有覆盖的网格中
	for (int32 X = MinGrid.X; X <= MaxGrid.X; ++X)
	{
		for (int32 Y = MinGrid.Y; Y <= MaxGrid.Y; ++Y)
		{
			for (int32 Z = MinGrid.Z; Z <= MaxGrid.Z; ++Z)
			{
				FIntVector GridCoord(X, Y, Z);
				if (IsValidGridCoord(GridCoord))
				{
					int32 Index = GridToIndex(GridCoord);
					Grid.FindOrAdd(Index).Add(Body);
				}
			}
		}
	}
}

void FSpatialGrid::Query(const FVector& Position, float Radius, TArray<FCollisionBody>& OutBodies) const
{
	// 创建查询边界盒
	FVector Extent(Radius, Radius, Radius);
	FBox QueryBounds(Position - Extent, Position + Extent);

	// 查询边界盒内的碰撞体
	QueryBox(QueryBounds, OutBodies);
}

void FSpatialGrid::QueryBox(const FBox& QueryBounds, TArray<FCollisionBody>& OutBodies) const
{
	OutBodies.Empty();

	// 获取边界盒覆盖的网格范围
	FIntVector MinGrid, MaxGrid;
	GetGridRange(QueryBounds, MinGrid, MaxGrid);

	// 使用集合去重
	TSet<FGuid> AddedIDs;

	// 遍历所有覆盖的网格
	for (int32 X = MinGrid.X; X <= MaxGrid.X; ++X)
	{
		for (int32 Y = MinGrid.Y; Y <= MaxGrid.Y; ++Y)
		{
			for (int32 Z = MinGrid.Z; Z <= MaxGrid.Z; ++Z)
			{
				FIntVector GridCoord(X, Y, Z);
				if (IsValidGridCoord(GridCoord))
				{
					int32 Index = GridToIndex(GridCoord);
					const TArray<FCollisionBody>* CellBodies = Grid.Find(Index);
					if (CellBodies)
					{
						for (const FCollisionBody& Body : *CellBodies)
						{
							// 去重
							if (!AddedIDs.Contains(Body.ID))
							{
								OutBodies.Add(Body);
								AddedIDs.Add(Body.ID);
							}
						}
					}
				}
			}
		}
	}
}

void FSpatialGrid::GetStatistics(int32& OutTotalCells, int32& OutOccupiedCells, 
                                  int32& OutMaxBodiesPerCell, float& OutAvgBodiesPerCell) const
{
	OutTotalCells = GridDimensions.X * GridDimensions.Y * GridDimensions.Z;
	OutOccupiedCells = Grid.Num();
	OutMaxBodiesPerCell = 0;
	OutAvgBodiesPerCell = 0.0f;

	if (OutOccupiedCells > 0)
	{
		int32 TotalBodies = 0;
		for (const auto& Pair : Grid)
		{
			int32 CellBodyCount = Pair.Value.Num();
			TotalBodies += CellBodyCount;
			OutMaxBodiesPerCell = FMath::Max(OutMaxBodiesPerCell, CellBodyCount);
		}
		OutAvgBodiesPerCell = static_cast<float>(TotalBodies) / OutOccupiedCells;
	}
}

FIntVector FSpatialGrid::WorldToGrid(const FVector& WorldPosition) const
{
	FVector LocalPosition = WorldPosition - Bounds.Min;
	return FIntVector(
		FMath::FloorToInt(LocalPosition.X / CellSize),
		FMath::FloorToInt(LocalPosition.Y / CellSize),
		FMath::FloorToInt(LocalPosition.Z / CellSize)
	);
}

int32 FSpatialGrid::GridToIndex(const FIntVector& GridCoord) const
{
	return GridCoord.X + GridCoord.Y * GridDimensions.X + GridCoord.Z * GridDimensions.X * GridDimensions.Y;
}

bool FSpatialGrid::IsValidGridCoord(const FIntVector& GridCoord) const
{
	return GridCoord.X >= 0 && GridCoord.X < GridDimensions.X &&
	       GridCoord.Y >= 0 && GridCoord.Y < GridDimensions.Y &&
	       GridCoord.Z >= 0 && GridCoord.Z < GridDimensions.Z;
}

void FSpatialGrid::GetGridRange(const FBox& Box, FIntVector& OutMinGrid, FIntVector& OutMaxGrid) const
{
	OutMinGrid = WorldToGrid(Box.Min);
	OutMaxGrid = WorldToGrid(Box.Max);

	// 限制在有效范围内
	OutMinGrid.X = FMath::Clamp(OutMinGrid.X, 0, GridDimensions.X - 1);
	OutMinGrid.Y = FMath::Clamp(OutMinGrid.Y, 0, GridDimensions.Y - 1);
	OutMinGrid.Z = FMath::Clamp(OutMinGrid.Z, 0, GridDimensions.Z - 1);

	OutMaxGrid.X = FMath::Clamp(OutMaxGrid.X, 0, GridDimensions.X - 1);
	OutMaxGrid.Y = FMath::Clamp(OutMaxGrid.Y, 0, GridDimensions.Y - 1);
	OutMaxGrid.Z = FMath::Clamp(OutMaxGrid.Z, 0, GridDimensions.Z - 1);
}
