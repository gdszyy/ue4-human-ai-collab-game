// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Physics/GravityFieldEffect.h"
#include "Physics/MarbleState.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGravityFieldEffectBasicTest, 
	"EchoAlchemist.Physics.GravityFieldEffect.Basic", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGravityFieldEffectBasicTest::RunTest(const FString& Parameters)
{
	// 1. 创建引力场效果
	UGravityFieldEffect* Effect = NewObject<UGravityFieldEffect>();
	Effect->FieldCenter = FVector(0, 0, 0);
	Effect->FieldRadius = 500.0f;
	Effect->GravityStrength = 1000.0f;
	Effect->Duration = -1.0f;  // 永久
	
	// 2. 创建测试魔力露珠（距离中心300cm）
	FMarbleState Marble;
	Marble.ID = FGuid::NewGuid();
	Marble.Position = FVector(300, 0, 0);
	Marble.Velocity = FVector(0, 0, 0);
	Marble.Radius = 10.0f;
	Marble.Mass = 1.0f;
	
	// 3. 应用效果（模拟1帧，约0.016秒）
	Effect->ApplyEffect(Marble, 0.016f);
	
	// 4. 验证魔力露珠被吸引（速度应该指向中心）
	TestTrue("Marble should be attracted to gravity field", Marble.Velocity.X < 0);
	TestTrue("Velocity should be non-zero", Marble.Velocity.SizeSquared() > 0);
	
	// 5. 验证效果未过期
	TestFalse("Effect should not be removed", Effect->ShouldRemove());
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGravityFieldEffectRangeTest, 
	"EchoAlchemist.Physics.GravityFieldEffect.Range", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGravityFieldEffectRangeTest::RunTest(const FString& Parameters)
{
	// 1. 创建引力场效果
	UGravityFieldEffect* Effect = NewObject<UGravityFieldEffect>();
	Effect->FieldCenter = FVector(0, 0, 0);
	Effect->FieldRadius = 500.0f;
	Effect->GravityStrength = 1000.0f;
	
	// 2. 测试范围内的魔力露珠
	FMarbleState MarbleInRange;
	MarbleInRange.ID = FGuid::NewGuid();
	MarbleInRange.Position = FVector(400, 0, 0);  // 距离400cm，在范围内
	MarbleInRange.Velocity = FVector(0, 0, 0);
	
	Effect->ApplyEffect(MarbleInRange, 0.016f);
	TestTrue("Marble in range should be affected", MarbleInRange.Velocity.SizeSquared() > 0);
	
	// 3. 测试范围外的魔力露珠
	FMarbleState MarbleOutOfRange;
	MarbleOutOfRange.ID = FGuid::NewGuid();
	MarbleOutOfRange.Position = FVector(600, 0, 0);  // 距离600cm，超出范围
	MarbleOutOfRange.Velocity = FVector(0, 0, 0);
	
	Effect->ApplyEffect(MarbleOutOfRange, 0.016f);
	TestTrue("Marble out of range should not be affected", MarbleOutOfRange.Velocity.SizeSquared() == 0);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGravityFieldEffectDurationTest, 
	"EchoAlchemist.Physics.GravityFieldEffect.Duration", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGravityFieldEffectDurationTest::RunTest(const FString& Parameters)
{
	// 1. 创建有限时长的引力场效果
	UGravityFieldEffect* Effect = NewObject<UGravityFieldEffect>();
	Effect->FieldCenter = FVector(0, 0, 0);
	Effect->FieldRadius = 500.0f;
	Effect->GravityStrength = 1000.0f;
	Effect->Duration = 1.0f;  // 持续1秒
	
	// 2. 创建测试魔力露珠
	FMarbleState Marble;
	Marble.ID = FGuid::NewGuid();
	Marble.Position = FVector(300, 0, 0);
	Marble.Velocity = FVector(0, 0, 0);
	
	// 3. 应用效果0.5秒（未过期）
	Effect->ApplyEffect(Marble, 0.5f);
	TestFalse("Effect should not be removed after 0.5s", Effect->ShouldRemove());
	TestTrue("Remaining time should be 0.5s", FMath::IsNearlyEqual(Effect->GetRemainingTime(), 0.5f, 0.01f));
	
	// 4. 再应用0.6秒（已过期）
	Effect->ApplyEffect(Marble, 0.6f);
	TestTrue("Effect should be removed after 1.1s", Effect->ShouldRemove());
	TestTrue("Remaining time should be 0", Effect->GetRemainingTime() == 0.0f);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGravityFieldEffectRepulsionTest, 
	"EchoAlchemist.Physics.GravityFieldEffect.Repulsion", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGravityFieldEffectRepulsionTest::RunTest(const FString& Parameters)
{
	// 1. 创建斥力场效果（负引力强度）
	UGravityFieldEffect* Effect = NewObject<UGravityFieldEffect>();
	Effect->FieldCenter = FVector(0, 0, 0);
	Effect->FieldRadius = 500.0f;
	Effect->GravityStrength = -1000.0f;  // 负值 = 斥力
	
	// 2. 创建测试魔力露珠
	FMarbleState Marble;
	Marble.ID = FGuid::NewGuid();
	Marble.Position = FVector(300, 0, 0);
	Marble.Velocity = FVector(0, 0, 0);
	
	// 3. 应用效果
	Effect->ApplyEffect(Marble, 0.016f);
	
	// 4. 验证魔力露珠被推开（速度应该远离中心）
	TestTrue("Marble should be repelled from field center", Marble.Velocity.X > 0);
	
	return true;
}
