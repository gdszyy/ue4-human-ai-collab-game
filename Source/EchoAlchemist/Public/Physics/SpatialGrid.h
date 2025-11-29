// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Physics/CollisionShape.h"

/**
 * 空间网格
 * 
 * 用于优化碰撞检测性能的空间分割数据结构。
 * 将空间划分为网格，每个网格存储其中的碰撞体，
 * 这样在检测碰撞时只需要检查相邻网格中的碰撞体。
 * 
 * 性能优化：
 * - 暴力检测：O(n²)
 * - 空间网格：O(n * k)，其中k是每个网格的平均碰撞体数量
 * 
 * 注意事项：
 * - 网格尺寸应该根据碰撞体的平均大小来设置
 * - 太小的网格会导致频繁的跨网格查询
 * - 太大的网格会失去优化效果
 */
class FSpatialGrid
{
public:
	/**
	 * 构造函数
	 * 
	 * @param InBounds 空间边界
	 * @param InCellSize 网格单元尺寸（单位：cm）
	 */
	FSpatialGrid(const FBox& InBounds, float InCellSize);

	/**
	 * 清空网格
	 */
	void Clear();

	/**
	 * 插入碰撞体到网格
	 * 
	 * @param Body 碰撞体
	 */
	void Insert(const FCollisionBody& Body);

	/**
	 * 查询指定位置附近的碰撞体
	 * 
	 * @param Position 查询位置
	 * @param Radius 查询半径
	 * @param OutBodies 输出参数，存储查询结果
	 */
	void Query(const FVector& Position, float Radius, TArray<FCollisionBody>& OutBodies) const;

	/**
	 * 查询指定边界盒内的碰撞体
	 * 
	 * @param Bounds 查询边界盒
	 * @param OutBodies 输出参数，存储查询结果
	 */
	void QueryBox(const FBox& Bounds, TArray<FCollisionBody>& OutBodies) const;

	/**
	 * 获取网格统计信息（用于调试）
	 * 
	 * @param OutTotalCells 总网格数
	 * @param OutOccupiedCells 已占用网格数
	 * @param OutMaxBodiesPerCell 单个网格最大碰撞体数
	 * @param OutAvgBodiesPerCell 单个网格平均碰撞体数
	 */
	void GetStatistics(int32& OutTotalCells, int32& OutOccupiedCells, 
	                   int32& OutMaxBodiesPerCell, float& OutAvgBodiesPerCell) const;

private:
	/** 空间边界 */
	FBox Bounds;

	/** 网格单元尺寸 */
	float CellSize;

	/** 网格维度（X, Y, Z方向的网格数量） */
	FIntVector GridDimensions;

	/** 网格数据（网格索引 -> 碰撞体列表） */
	TMap<int32, TArray<FCollisionBody>> Grid;

	/**
	 * 将世界坐标转换为网格坐标
	 * 
	 * @param WorldPosition 世界坐标
	 * @return 网格坐标
	 */
	FIntVector WorldToGrid(const FVector& WorldPosition) const;

	/**
	 * 将网格坐标转换为网格索引
	 * 
	 * @param GridCoord 网格坐标
	 * @return 网格索引
	 */
	int32 GridToIndex(const FIntVector& GridCoord) const;

	/**
	 * 检查网格坐标是否有效
	 * 
	 * @param GridCoord 网格坐标
	 * @return true=有效，false=无效
	 */
	bool IsValidGridCoord(const FIntVector& GridCoord) const;

	/**
	 * 获取边界盒覆盖的网格范围
	 * 
	 * @param Box 边界盒
	 * @param OutMinGrid 输出参数，最小网格坐标
	 * @param OutMaxGrid 输出参数，最大网格坐标
	 */
	void GetGridRange(const FBox& Box, FIntVector& OutMinGrid, FIntVector& OutMaxGrid) const;
};
