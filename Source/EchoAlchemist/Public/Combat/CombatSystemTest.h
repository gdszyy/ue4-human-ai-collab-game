// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatManager.h"
#include "EnemyManager.h"
#include "CircularSceneManager.h"
#include "FallingSceneManager.h"
#include "DamageCalculator.h"
#include "CombatSystemTest.generated.h"

/**
 * 战斗系统集成测试
 * 用于测试战斗系统的各个组件是否正常工作
 * 
 * 测试内容：
 * 1. 环形场景管理器测试
 * 2. 敌人管理器测试
 * 3. 战斗管理器测试
 * 4. 伤害计算器测试
 * 5. 系统集成测试
 */
UCLASS(Blueprintable)
class ECHOALCHEMIST_API UCombatSystemTest : public UObject
{
	GENERATED_BODY()

public:
	// ========== 测试方法 ==========
	
	/**
	 * 测试环形场景管理器
	 * @return 测试是否通过
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Test")
	bool TestCircularSceneManager();

	/**
	 * 测试敌人管理器（环形场景）
	 * @return 测试是否通过
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Test")
	bool TestEnemyManagerCircular();

	/**
	 * 测试敌人管理器（下落式场景）
	 * @return 测试是否通过
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Test")
	bool TestEnemyManagerFalling();

	/**
	 * 测试战斗管理器
	 * @return 测试是否通过
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Test")
	bool TestCombatManager();

	/**
	 * 测试伤害计算器
	 * @return 测试是否通过
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Test")
	bool TestDamageCalculator();

	/**
	 * 运行所有测试
	 * @return 测试是否全部通过
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Test")
	bool RunAllTests();

protected:
	// ========== 辅助方法 ==========
	
	/**
	 * 打印测试结果
	 * @param TestName 测试名称
	 * @param bPassed 是否通过
	 */
	void PrintTestResult(const FString& TestName, bool bPassed);

	/**
	 * 检查浮点数是否近似相等
	 * @param A 值A
	 * @param B 值B
	 * @param Tolerance 容差
	 * @return 是否近似相等
	 */
	bool IsNearlyEqual(float A, float B, float Tolerance = 0.01f) const;
};
