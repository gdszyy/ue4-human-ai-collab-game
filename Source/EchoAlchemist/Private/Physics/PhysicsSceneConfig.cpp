// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/PhysicsSceneConfig.h"

FPhysicsSceneConfig USceneConfigFactory::CreateWorkbenchConfig()
{
	FPhysicsSceneConfig Config;
	Config.SceneType = EPhysicsSceneType::Workbench;
	
	// 启用重力
	Config.bEnableGravity = true;
	Config.GravityDirection = FVector(0, 0, -1);
	Config.GravityStrength = 980.0f;
	
	// 边界配置
	Config.bHasBoundary = true;
	Config.BoundaryBehavior = EBoundaryBehavior::Delete;
	
	// 碰撞配置
	Config.MarbleShape = ECollisionShapeType::Circle;
	
	// 不使用药效强度系统
	Config.bUsePotencySystem = false;
	
	// 性能配置（炼金工作台魔力露珠数量少，不需要粒子优化）
	Config.MaxActorMarbles = 10;
	Config.bEnableParticleOptimization = false;
	
	return Config;
}

FPhysicsSceneConfig USceneConfigFactory::CreateCombatConfig(FVector BoundaryMin, FVector BoundaryMax)
{
	FPhysicsSceneConfig Config;
	Config.SceneType = EPhysicsSceneType::Combat;
	
	// 无重力
	Config.bEnableGravity = false;
	
	// 边界配置
	Config.bHasBoundary = true;
	Config.BoundaryBox = FBox(BoundaryMin, BoundaryMax);
	Config.BoundaryBehavior = EBoundaryBehavior::Bounce;
	
	// 碰撞配置
	Config.MarbleShape = ECollisionShapeType::Circle;
	
	// 使用药效强度系统
	Config.bUsePotencySystem = true;
	
	// 性能配置（战斗场景魔药数量多，启用粒子优化）
	Config.MaxActorMarbles = 10;
	Config.bEnableParticleOptimization = true;
	
	return Config;
}
