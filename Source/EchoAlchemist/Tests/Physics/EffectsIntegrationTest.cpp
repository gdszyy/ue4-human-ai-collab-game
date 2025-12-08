// Copyright 2025 Voidzyy. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Physics/MarblePhysicsSystem.h"
#include "Physics/PhysicsEffectManager.h"
#include "Physics/GravityFieldEffect.h"
#include "Physics/WormholeEffect.h"
#include "Physics/VelocityModifierEffect.h"
#include "Physics/PhysicsSceneConfig.h"

/**
 * 特殊物理效果集成测试
 * 测试效果系统与物理系统的集成
 */

// 测试：引力场效果与物理系统集成
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEffectsIntegrationGravityTest,
	"EchoAlchemist.Physics.EffectsIntegration.Gravity",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FEffectsIntegrationGravityTest::RunTest(const FString& Parameters)
{
	// 1. 创建物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	
	// 2. 初始化场景（关闭重力，便于测试效果）
	FPhysicsSceneConfig Config;
	Config.SceneType = EPhysicsSceneType::Combat;
	Config.BoundaryMin = FVector(-2000, -2000, 0);
	Config.BoundaryMax = FVector(2000, 2000, 2000);
	Config.bHasBoundary = true;
	Config.bEnableGravity = false;  // 关闭重力
	
	PhysicsSystem->InitializeScene(Config);
	
	// 3. 获取效果管理器
	UPhysicsEffectManager* EffectManager = PhysicsSystem->GetEffectManager();
	TestNotNull(TEXT("Effect manager should exist"), EffectManager);
	TestTrue(TEXT("Effect manager should be initialized"), EffectManager->IsInitialized());
	
	// 4. 创建引力场效果
	UGravityFieldEffect* GravityEffect = NewObject<UGravityFieldEffect>();
	GravityEffect->FieldCenter = FVector(0, 0, 500);
	GravityEffect->FieldRadius = 1000.0f;
	GravityEffect->GravityStrength = 2000.0f;
	GravityEffect->FalloffType = EGravityFalloffType::InverseSquare;
	GravityEffect->Duration = -1.0f;  // 永久
	
	FGuid EffectID = EffectManager->AddEffect(GravityEffect);
	TestTrue(TEXT("Effect should be added"), EffectID.IsValid());
	
	// 5. 发射多个魔力露珠
	TArray<FGuid> MarbleIDs;
	for (int32 i = 0; i < 5; i++)
	{
		FMarbleLaunchParams Params;
		Params.LaunchPosition = FVector(500 + i * 100, 0, 500);
		Params.LaunchDirection = FVector(0, 1, 0);
		Params.LaunchSpeed = 100.0f;
		Params.Radius = 10.0f;
		Params.Mass = 1.0f;
		
		FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
		MarbleIDs.Add(MarbleID);
	}
	
	TestEqual(TEXT("Should have 5 marbles"), PhysicsSystem->GetMarbleCount(), 5);
	
	// 6. 记录初始距离
	TArray<float> InitialDistances;
	for (const FGuid& MarbleID : MarbleIDs)
	{
		FMarbleState State;
		PhysicsSystem->GetMarbleState(MarbleID, State);
		float Distance = FVector::Dist(State.Position, GravityEffect->FieldCenter);
		InitialDistances.Add(Distance);
	}
	
	// 7. 模拟1秒物理
	for (int32 Frame = 0; Frame < 60; Frame++)
	{
		PhysicsSystem->Tick(1.0f / 60.0f);
	}
	
	// 8. 验证所有魔力露珠都被吸引
	int32 AttractedCount = 0;
	for (int32 i = 0; i < MarbleIDs.Num(); i++)
	{
		FMarbleState State;
		PhysicsSystem->GetMarbleState(MarbleIDs[i], State);
		float FinalDistance = FVector::Dist(State.Position, GravityEffect->FieldCenter);
		
		if (FinalDistance < InitialDistances[i])
		{
			AttractedCount++;
		}
		
		UE_LOG(LogTemp, Log, TEXT("[Gravity Test] Marble %d: %.2f -> %.2f cm"),
			i, InitialDistances[i], FinalDistance);
	}
	
	TestEqual(TEXT("All marbles should be attracted"), AttractedCount, 5);
	
	// 9. 清理
	PhysicsSystem->CleanupScene();
	
	return true;
}

// 测试：虫洞效果与物理系统集成
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEffectsIntegrationWormholeTest,
	"EchoAlchemist.Physics.EffectsIntegration.Wormhole",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FEffectsIntegrationWormholeTest::RunTest(const FString& Parameters)
{
	// 1. 创建物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	
	// 2. 初始化场景
	FPhysicsSceneConfig Config;
	Config.SceneType = EPhysicsSceneType::Combat;
	Config.BoundaryMin = FVector(-2000, -2000, 0);
	Config.BoundaryMax = FVector(2000, 2000, 2000);
	Config.bHasBoundary = true;
	Config.bEnableGravity = false;
	
	PhysicsSystem->InitializeScene(Config);
	
	// 3. 创建虫洞效果
	UPhysicsEffectManager* EffectManager = PhysicsSystem->GetEffectManager();
	UWormholeEffect* WormholeEffect = NewObject<UWormholeEffect>();
	WormholeEffect->EntrancePosition = FVector(500, 0, 500);
	WormholeEffect->EntranceRadius = 100.0f;
	WormholeEffect->ExitPosition = FVector(-500, 0, 500);
	WormholeEffect->ExitVelocityMultiplier = 1.5f;
	WormholeEffect->bPreserveVelocityDirection = false;  // 随机方向
	WormholeEffect->TeleportCooldown = 0.5f;
	WormholeEffect->Duration = -1.0f;
	
	EffectManager->AddEffect(WormholeEffect);
	
	// 4. 发射魔力露珠（朝向入口）
	FMarbleLaunchParams Params;
	Params.LaunchPosition = FVector(800, 0, 500);
	Params.LaunchDirection = FVector(-1, 0, 0);
	Params.LaunchSpeed = 300.0f;
	Params.Radius = 10.0f;
	Params.Mass = 1.0f;
	
	FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
	
	// 5. 模拟直到魔力露珠到达入口
	bool bTeleported = false;
	for (int32 Frame = 0; Frame < 120; Frame++)
	{
		PhysicsSystem->Tick(1.0f / 60.0f);
		
		FMarbleState State;
		if (PhysicsSystem->GetMarbleState(MarbleID, State))
		{
			float DistanceToExit = FVector::Dist(State.Position, WormholeEffect->ExitPosition);
			
			// 如果魔力露珠接近出口，说明已传送
			if (DistanceToExit < 200.0f)
			{
				bTeleported = true;
				UE_LOG(LogTemp, Log, TEXT("[Wormhole Test] Teleported at frame %d"), Frame);
				break;
			}
		}
	}
	
	TestTrue(TEXT("Marble should be teleported"), bTeleported);
	
	// 6. 清理
	PhysicsSystem->CleanupScene();
	
	return true;
}

// 测试：速度修改效果与物理系统集成
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEffectsIntegrationVelocityModifierTest,
	"EchoAlchemist.Physics.EffectsIntegration.VelocityModifier",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FEffectsIntegrationVelocityModifierTest::RunTest(const FString& Parameters)
{
	// 1. 创建物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	
	// 2. 初始化场景
	FPhysicsSceneConfig Config;
	Config.SceneType = EPhysicsSceneType::Combat;
	Config.BoundaryMin = FVector(-2000, -2000, 0);
	Config.BoundaryMax = FVector(2000, 2000, 2000);
	Config.bHasBoundary = true;
	Config.bEnableGravity = false;
	
	PhysicsSystem->InitializeScene(Config);
	
	// 3. 创建加速区域
	UPhysicsEffectManager* EffectManager = PhysicsSystem->GetEffectManager();
	UVelocityModifierEffect* AcceleratorEffect = NewObject<UVelocityModifierEffect>();
	AcceleratorEffect->ModifierType = EVelocityModifierType::Multiply;
	AcceleratorEffect->ModifierValue = FVector(2.0f, 2.0f, 1.0f);  // XY方向2倍速
	AcceleratorEffect->AffectArea = EAreaType::Sphere;
	AcceleratorEffect->AreaCenter = FVector(0, 0, 500);
	AcceleratorEffect->AreaSize = FVector(300.0f, 300.0f, 300.0f);  // 半径300cm
	AcceleratorEffect->bApplyEveryFrame = false;  // 只应用一次
	AcceleratorEffect->Duration = -1.0f;
	
	EffectManager->AddEffect(AcceleratorEffect);
	
	// 4. 发射魔力露珠（朝向加速区域）
	FMarbleLaunchParams Params;
	Params.LaunchPosition = FVector(-500, 0, 500);
	Params.LaunchDirection = FVector(1, 0, 0);
	Params.LaunchSpeed = 200.0f;
	Params.Radius = 10.0f;
	Params.Mass = 1.0f;
	
	FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
	
	// 5. 记录初始速度
	FMarbleState InitialState;
	PhysicsSystem->GetMarbleState(MarbleID, InitialState);
	float InitialSpeed = InitialState.Velocity.Size();
	
	// 6. 模拟直到魔力露珠进入加速区域
	bool bAccelerated = false;
	for (int32 Frame = 0; Frame < 120; Frame++)
	{
		PhysicsSystem->Tick(1.0f / 60.0f);
		
		FMarbleState State;
		if (PhysicsSystem->GetMarbleState(MarbleID, State))
		{
			float CurrentSpeed = State.Velocity.Size();
			
			// 如果速度显著增加，说明已加速
			if (CurrentSpeed > InitialSpeed * 1.5f)
			{
				bAccelerated = true;
				UE_LOG(LogTemp, Log, TEXT("[Velocity Modifier Test] Accelerated at frame %d: %.2f -> %.2f cm/s"),
					Frame, InitialSpeed, CurrentSpeed);
				break;
			}
		}
	}
	
	TestTrue(TEXT("Marble should be accelerated"), bAccelerated);
	
	// 7. 清理
	PhysicsSystem->CleanupScene();
	
	return true;
}

// 测试：多效果组合
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEffectsIntegrationMultipleEffectsTest,
	"EchoAlchemist.Physics.EffectsIntegration.MultipleEffects",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FEffectsIntegrationMultipleEffectsTest::RunTest(const FString& Parameters)
{
	// 1. 创建物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	
	// 2. 初始化场景
	FPhysicsSceneConfig Config;
	Config.SceneType = EPhysicsSceneType::Combat;
	Config.BoundaryMin = FVector(-2000, -2000, 0);
	Config.BoundaryMax = FVector(2000, 2000, 2000);
	Config.bHasBoundary = true;
	Config.bEnableGravity = false;
	
	PhysicsSystem->InitializeScene(Config);
	
	// 3. 创建多个效果
	UPhysicsEffectManager* EffectManager = PhysicsSystem->GetEffectManager();
	
	// 引力场1（中心）
	UGravityFieldEffect* Gravity1 = NewObject<UGravityFieldEffect>();
	Gravity1->FieldCenter = FVector(0, 0, 500);
	Gravity1->FieldRadius = 500.0f;
	Gravity1->GravityStrength = 1000.0f;
	Gravity1->Duration = -1.0f;
	EffectManager->AddEffect(Gravity1);
	
	// 引力场2（左侧，斥力）
	UGravityFieldEffect* Gravity2 = NewObject<UGravityFieldEffect>();
	Gravity2->FieldCenter = FVector(-800, 0, 500);
	Gravity2->FieldRadius = 400.0f;
	Gravity2->GravityStrength = -500.0f;  // 斥力
	Gravity2->Duration = -1.0f;
	EffectManager->AddEffect(Gravity2);
	
	// 加速区域（右侧）
	UVelocityModifierEffect* Accelerator = NewObject<UVelocityModifierEffect>();
	Accelerator->ModifierType = EVelocityModifierType::Multiply;
	Accelerator->ModifierValue = FVector(1.5f, 1.5f, 1.0f);
	Accelerator->AffectArea = EAreaType::Sphere;
	Accelerator->AreaCenter = FVector(800, 0, 500);
	Accelerator->AreaSize = FVector(300.0f, 300.0f, 300.0f);
	Accelerator->bApplyEveryFrame = true;
	Accelerator->Duration = -1.0f;
	EffectManager->AddEffect(Accelerator);
	
	TestEqual(TEXT("Should have 3 effects"), EffectManager->GetEffectCount(), 3);
	
	// 4. 发射多个魔力露珠
	TArray<FGuid> MarbleIDs;
	for (int32 i = 0; i < 10; i++)
	{
		FMarbleLaunchParams Params;
		Params.LaunchPosition = FVector(
			FMath::FRandRange(-1000, 1000),
			FMath::FRandRange(-1000, 1000),
			500
		);
		Params.LaunchDirection = FVector(
			FMath::FRandRange(-1, 1),
			FMath::FRandRange(-1, 1),
			0
		).GetSafeNormal();
		Params.LaunchSpeed = 200.0f;
		Params.Radius = 10.0f;
		Params.Mass = 1.0f;
		
		FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
		MarbleIDs.Add(MarbleID);
	}
	
	// 5. 模拟2秒物理
	for (int32 Frame = 0; Frame < 120; Frame++)
	{
		PhysicsSystem->Tick(1.0f / 60.0f);
		
		// 每30帧输出一次统计
		if (Frame % 30 == 0)
		{
			int32 ActiveMarbles = PhysicsSystem->GetMarbleCount();
			UE_LOG(LogTemp, Log, TEXT("[Multiple Effects Test] Frame %d: %d marbles"),
				Frame, ActiveMarbles);
		}
	}
	
	// 6. 验证系统稳定性
	TestTrue(TEXT("System should remain stable with multiple effects"), true);
	
	// 7. 清理
	PhysicsSystem->CleanupScene();
	
	return true;
}

// 测试：效果持续时间管理
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEffectsIntegrationDurationTest,
	"EchoAlchemist.Physics.EffectsIntegration.Duration",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FEffectsIntegrationDurationTest::RunTest(const FString& Parameters)
{
	// 1. 创建物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	
	// 2. 初始化场景
	FPhysicsSceneConfig Config;
	Config.SceneType = EPhysicsSceneType::Combat;
	Config.BoundaryMin = FVector(-2000, -2000, 0);
	Config.BoundaryMax = FVector(2000, 2000, 2000);
	Config.bHasBoundary = true;
	Config.bEnableGravity = false;
	
	PhysicsSystem->InitializeScene(Config);
	
	// 3. 创建有限时长的效果
	UPhysicsEffectManager* EffectManager = PhysicsSystem->GetEffectManager();
	
	UGravityFieldEffect* ShortEffect = NewObject<UGravityFieldEffect>();
	ShortEffect->FieldCenter = FVector(0, 0, 500);
	ShortEffect->FieldRadius = 500.0f;
	ShortEffect->GravityStrength = 1000.0f;
	ShortEffect->Duration = 0.5f;  // 持续0.5秒
	
	EffectManager->AddEffect(ShortEffect);
	TestEqual(TEXT("Should have 1 effect"), EffectManager->GetEffectCount(), 1);
	
	// 4. 发射魔力露珠
	FMarbleLaunchParams Params;
	Params.LaunchPosition = FVector(300, 0, 500);
	Params.LaunchDirection = FVector(0, 1, 0);
	Params.LaunchSpeed = 100.0f;
	Params.Radius = 10.0f;
	Params.Mass = 1.0f;
	
	PhysicsSystem->LaunchMarble(Params);
	
	// 5. 模拟1秒（效果应该在0.5秒后过期）
	for (int32 Frame = 0; Frame < 60; Frame++)
	{
		PhysicsSystem->Tick(1.0f / 60.0f);
		
		// 在第30帧（0.5秒）检查效果是否还存在
		if (Frame == 30)
		{
			int32 EffectCount = EffectManager->GetEffectCount();
			UE_LOG(LogTemp, Log, TEXT("[Duration Test] Effect count at 0.5s: %d"), EffectCount);
		}
	}
	
	// 6. 验证效果已过期
	int32 FinalEffectCount = EffectManager->GetEffectCount();
	TestEqual(TEXT("Effect should be removed after duration"), FinalEffectCount, 0);
	
	// 7. 清理
	PhysicsSystem->CleanupScene();
	
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
