// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyData.h"
#include "SceneManager.h"
#include "CombatEvents.h"
#include "EnemyManager.generated.h"

/**
 * 敌人管理器
 * 负责敌人的生成、更新、死亡、移除
 * 
 * 设计理念：
 * - 生命周期管理：管理敌人的完整生命周期
 * - 场景集成：支持下落式和环形场景
 * - 事件驱动：通过事件总线通知敌人状态变化
 * - 行为控制：简单的移动和攻击行为
 * 
 * 核心功能：
 * 1. 敌人生成（手工设计 + 程序化生成）
 * 2. 敌人更新（移动、攻击、状态更新）
 * 3. 敌人伤害处理
 * 4. 敌人死亡和移除
 */
UCLASS(Blueprintable)
class ECHOALCHEMIST_API UEnemyManager : public UObject
{
	GENERATED_BODY()

public:
	// ========== 构造函数 ==========
	
	UEnemyManager();

	// ========== 初始化 ==========
	
	/**
	 * 初始化敌人管理器
	 * @param InSceneManager 场景管理器
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	void Initialize(TScriptInterface<ISceneManager> InSceneManager);

	// ========== 敌人生成 ==========
	
	/**
	 * 生成敌人
	 * @param EnemyType 敌人类型
	 * @param Position 位置
	 * @param MaxHealth 最大生命值
	 * @return 敌人ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	FGuid SpawnEnemy(EEnemyType EnemyType, FVector Position, float MaxHealth);

	/**
	 * 生成敌人（使用角度，仅环形场景）
	 * @param EnemyType 敌人类型
	 * @param Angle 角度（弧度）
	 * @param MaxHealth 最大生命值
	 * @return 敌人ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	FGuid SpawnEnemyAtAngle(EEnemyType EnemyType, float Angle, float MaxHealth);

	/**
	 * 批量生成敌人
	 * @param EnemyType 敌人类型
	 * @param Count 数量
	 * @param MaxHealth 最大生命值
	 * @return 敌人ID列表
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	TArray<FGuid> SpawnEnemies(EEnemyType EnemyType, int32 Count, float MaxHealth);

	// ========== 敌人更新 ==========
	
	/**
	 * 更新所有敌人
	 * @param DeltaTime 时间增量（秒）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	void UpdateEnemies(float DeltaTime);

	/**
	 * 更新单个敌人
	 * @param Enemy 敌人数据（输入/输出）
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdateEnemy(FEnemyData& Enemy, float DeltaTime);

	// ========== 敌人伤害 ==========
	
	/**
	 * 应用伤害到敌人
	 * @param EnemyID 敌人ID
	 * @param Damage 伤害值
	 * @return 是否死亡
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	bool ApplyDamageToEnemy(FGuid EnemyID, float Damage);

	// ========== 敌人移除 ==========
	
	/**
	 * 移除敌人
	 * @param EnemyID 敌人ID
	 * @return 是否成功移除
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	bool RemoveEnemy(FGuid EnemyID);

	/**
	 * 移除所有死亡的敌人
	 * @return 移除的敌人数量
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	int32 RemoveDeadEnemies();

	/**
	 * 清空所有敌人
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	void ClearAllEnemies();

	// ========== 敌人查询 ==========
	
	/**
	 * 根据ID查找敌人
	 * @param EnemyID 敌人ID
	 * @param OutEnemy 敌人数据（输出）
	 * @return 是否找到
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Enemy")
	bool FindEnemy(FGuid EnemyID, FEnemyData& OutEnemy) const;

	/**
	 * 获取所有存活的敌人
	 * @return 敌人列表
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Enemy")
	TArray<FEnemyData> GetAliveEnemies() const;

	/**
	 * 获取敌人数量
	 * @return 敌人数量
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Enemy")
	int32 GetEnemyCount() const { return Enemies.Num(); }

	/**
	 * 获取存活的敌人数量
	 * @return 存活的敌人数量
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Enemy")
	int32 GetAliveEnemyCount() const;

	/**
	 * 获取最近的敌人
	 * @param Position 参考位置
	 * @param OutEnemy 敌人数据（输出）
	 * @return 是否找到
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Enemy")
	bool GetNearestEnemy(const FVector& Position, FEnemyData& OutEnemy) const;

	// ========== 事件系统 ==========
	
	/**
	 * 发布敌人事件
	 * @param Event 战斗事件
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy")
	void BroadcastEnemyEvent(const FCombatEvent& Event);

	/** 敌人事件委托 */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Enemy")
	FOnCombatEvent OnEnemyEvent;

protected:
	// ========== 场景管理器 ==========
	
	/** 场景管理器 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Enemy")
	TScriptInterface<ISceneManager> SceneManager;

	// ========== 敌人列表 ==========
	
	/** 敌人列表 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Enemy")
	TArray<FEnemyData> Enemies;

	// ========== 敌人行为参数 ==========
	
	/** 敌人移动速度（cm/s） */
	UPROPERTY(BlueprintReadWrite, Category = "Combat|Enemy")
	float EnemyMoveSpeed = 100.0f;

	/** 敌人角速度（弧度/秒，仅环形场景） */
	UPROPERTY(BlueprintReadWrite, Category = "Combat|Enemy")
	float EnemyAngularVelocity = 0.5f;

	/** 敌人攻击间隔（秒） */
	UPROPERTY(BlueprintReadWrite, Category = "Combat|Enemy")
	float EnemyAttackInterval = 2.0f;

	// ========== 内部方法 ==========
	
	/**
	 * 更新敌人移动（下落式场景）
	 * @param Enemy 敌人数据（输入/输出）
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdateEnemyMovementFalling(FEnemyData& Enemy, float DeltaTime);

	/**
	 * 更新敌人移动（环形场景）
	 * @param Enemy 敌人数据（输入/输出）
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdateEnemyMovementCircular(FEnemyData& Enemy, float DeltaTime);

	/**
	 * 更新敌人攻击
	 * @param Enemy 敌人数据（输入/输出）
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdateEnemyAttack(FEnemyData& Enemy, float DeltaTime);

	/**
	 * 根据ID查找敌人索引
	 * @param EnemyID 敌人ID
	 * @return 敌人索引（-1表示未找到）
	 */
	int32 FindEnemyIndex(FGuid EnemyID) const;
};
