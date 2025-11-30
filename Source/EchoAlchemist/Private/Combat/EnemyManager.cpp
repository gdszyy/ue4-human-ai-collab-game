// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/EnemyManager.h"
#include "Combat/CircularSceneManager.h"

UEnemyManager::UEnemyManager()
{
	EnemyMoveSpeed = 100.0f;
	EnemyAngularVelocity = 0.5f;
	EnemyAttackInterval = 2.0f;
}

void UEnemyManager::Initialize(TScriptInterface<ISceneManager> InSceneManager)
{
	SceneManager = InSceneManager;
	Enemies.Empty();
	
	UE_LOG(LogTemp, Log, TEXT("EnemyManager: Initialized with scene type: %s"), 
		SceneManager.GetInterface() ? *SceneManager->GetSceneType() : TEXT("None"));
}

FGuid UEnemyManager::SpawnEnemy(EEnemyType EnemyType, FVector Position, float MaxHealth)
{
	// 创建敌人数据
	FEnemyData Enemy;
	Enemy.ID = FGuid::NewGuid();
	Enemy.EnemyType = EnemyType;
	Enemy.Position = Position;
	Enemy.Health = MaxHealth;
	Enemy.MaxHealth = MaxHealth;
	Enemy.State = EEnemyState::Idle;
	Enemy.bIsActive = true;
	
	// 根据敌人类型设置名称
	switch (EnemyType)
	{
	case EEnemyType::CrystalGolem:
		Enemy.Name = TEXT("Crystal Golem");
		break;
	case EEnemyType::EliteGolem:
		Enemy.Name = TEXT("Elite Golem");
		break;
	case EEnemyType::BossGolem:
		Enemy.Name = TEXT("Boss Golem");
		break;
	}
	
	// 添加到敌人列表
	Enemies.Add(Enemy);
	
	// 发布敌人生成事件
	FCombatEvent Event;
	Event.EventType = ECombatEventType::EnemySpawned;
	Event.Timestamp = FPlatformTime::Seconds();
	Event.EntityID = Enemy.ID;
	Event.ExtraData.Add(TEXT("EnemyType"), static_cast<float>(EnemyType));
	Event.ExtraData.Add(TEXT("MaxHealth"), MaxHealth);
	BroadcastEnemyEvent(Event);
	
	UE_LOG(LogTemp, Log, TEXT("EnemyManager: Spawned enemy %s at (%.1f, %.1f, %.1f)"), 
		*Enemy.Name, Position.X, Position.Y, Position.Z);
	
	return Enemy.ID;
}

FGuid UEnemyManager::SpawnEnemyAtAngle(EEnemyType EnemyType, float Angle, float MaxHealth)
{
	// 检查场景管理器类型
	if (!SceneManager.GetInterface())
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManager: Scene manager not set"));
		return FGuid();
	}
	
	// 获取场景类型
	FString SceneType = SceneManager->GetSceneType();
	
	// 如果是环形场景，使用角度生成敌人
	if (SceneType == TEXT("Circular"))
	{
		UCircularSceneManager* CircularScene = Cast<UCircularSceneManager>(SceneManager.GetObject());
		if (CircularScene)
		{
			FVector Position = CircularScene->GetEnemyPosition(Angle);
			FGuid EnemyID = SpawnEnemy(EnemyType, Position, MaxHealth);
			
			// 存储角度到ExtraData
			int32 Index = FindEnemyIndex(EnemyID);
			if (Index != INDEX_NONE)
			{
				Enemies[Index].ExtraData.Add(TEXT("Angle"), Angle);
			}
			
			return EnemyID;
		}
	}
	
	// 如果不是环形场景，使用默认位置
	UE_LOG(LogTemp, Warning, TEXT("EnemyManager: SpawnEnemyAtAngle called on non-circular scene"));
	return SpawnEnemy(EnemyType, FVector::ZeroVector, MaxHealth);
}

TArray<FGuid> UEnemyManager::SpawnEnemies(EEnemyType EnemyType, int32 Count, float MaxHealth)
{
	TArray<FGuid> EnemyIDs;
	
	// 检查场景管理器类型
	if (!SceneManager.GetInterface())
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManager: Scene manager not set"));
		return EnemyIDs;
	}
	
	FString SceneType = SceneManager->GetSceneType();
	
	// 如果是环形场景，均匀分布在圆环上
	if (SceneType == TEXT("Circular"))
	{
		const float AngleStep = 2.0f * PI / Count;
		for (int32 i = 0; i < Count; ++i)
		{
			float Angle = i * AngleStep;
			FGuid EnemyID = SpawnEnemyAtAngle(EnemyType, Angle, MaxHealth);
			EnemyIDs.Add(EnemyID);
		}
	}
	// 如果是下落式场景，随机分布
	else
	{
		for (int32 i = 0; i < Count; ++i)
		{
			FVector Position = FVector(
				FMath::RandRange(-500.0f, 500.0f),
				FMath::RandRange(-500.0f, 500.0f),
				0.0f
			);
			FGuid EnemyID = SpawnEnemy(EnemyType, Position, MaxHealth);
			EnemyIDs.Add(EnemyID);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("EnemyManager: Spawned %d enemies"), Count);
	
	return EnemyIDs;
}

void UEnemyManager::UpdateEnemies(float DeltaTime)
{
	// 更新所有敌人
	for (FEnemyData& Enemy : Enemies)
	{
		if (Enemy.bIsActive)
		{
			UpdateEnemy(Enemy, DeltaTime);
		}
	}
}

void UEnemyManager::UpdateEnemy(FEnemyData& Enemy, float DeltaTime)
{
	// 检查场景管理器
	if (!SceneManager.GetInterface())
	{
		return;
	}
	
	FString SceneType = SceneManager->GetSceneType();
	
	// 根据场景类型更新移动
	if (SceneType == TEXT("Circular"))
	{
		UpdateEnemyMovementCircular(Enemy, DeltaTime);
	}
	else
	{
		UpdateEnemyMovementFalling(Enemy, DeltaTime);
	}
	
	// 更新攻击
	UpdateEnemyAttack(Enemy, DeltaTime);
}

bool UEnemyManager::ApplyDamageToEnemy(FGuid EnemyID, float Damage)
{
	int32 Index = FindEnemyIndex(EnemyID);
	if (Index == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyManager: Enemy not found: %s"), *EnemyID.ToString());
		return false;
	}
	
	FEnemyData& Enemy = Enemies[Index];
	
	// 应用伤害
	bool bDied = Enemy.ApplyDamage(Damage);
	
	// 发布敌人受伤事件
	FCombatEvent Event;
	Event.EventType = bDied ? ECombatEventType::EnemyKilled : ECombatEventType::EnemyDamaged;
	Event.Timestamp = FPlatformTime::Seconds();
	Event.EntityID = EnemyID;
	Event.ExtraData.Add(TEXT("Damage"), Damage);
	Event.ExtraData.Add(TEXT("RemainingHealth"), Enemy.Health);
	BroadcastEnemyEvent(Event);
	
	UE_LOG(LogTemp, Log, TEXT("EnemyManager: Enemy %s took %.1f damage. Health: %.1f / %.1f"), 
		*Enemy.Name, Damage, Enemy.Health, Enemy.MaxHealth);
	
	return bDied;
}

bool UEnemyManager::RemoveEnemy(FGuid EnemyID)
{
	int32 Index = FindEnemyIndex(EnemyID);
	if (Index == INDEX_NONE)
	{
		return false;
	}
	
	Enemies.RemoveAt(Index);
	
	UE_LOG(LogTemp, Log, TEXT("EnemyManager: Removed enemy: %s"), *EnemyID.ToString());
	
	return true;
}

int32 UEnemyManager::RemoveDeadEnemies()
{
	int32 RemovedCount = 0;
	
	for (int32 i = Enemies.Num() - 1; i >= 0; --i)
	{
		if (!Enemies[i].IsAlive())
		{
			Enemies.RemoveAt(i);
			RemovedCount++;
		}
	}
	
	if (RemovedCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("EnemyManager: Removed %d dead enemies"), RemovedCount);
	}
	
	return RemovedCount;
}

void UEnemyManager::ClearAllEnemies()
{
	Enemies.Empty();
	
	UE_LOG(LogTemp, Log, TEXT("EnemyManager: Cleared all enemies"));
}

bool UEnemyManager::FindEnemy(FGuid EnemyID, FEnemyData& OutEnemy) const
{
	int32 Index = FindEnemyIndex(EnemyID);
	if (Index == INDEX_NONE)
	{
		return false;
	}
	
	OutEnemy = Enemies[Index];
	return true;
}

TArray<FEnemyData> UEnemyManager::GetAliveEnemies() const
{
	TArray<FEnemyData> AliveEnemies;
	
	for (const FEnemyData& Enemy : Enemies)
	{
		if (Enemy.IsAlive())
		{
			AliveEnemies.Add(Enemy);
		}
	}
	
	return AliveEnemies;
}

int32 UEnemyManager::GetAliveEnemyCount() const
{
	int32 Count = 0;
	
	for (const FEnemyData& Enemy : Enemies)
	{
		if (Enemy.IsAlive())
		{
			Count++;
		}
	}
	
	return Count;
}

bool UEnemyManager::GetNearestEnemy(const FVector& Position, FEnemyData& OutEnemy) const
{
	float MinDistance = FLT_MAX;
	int32 NearestIndex = INDEX_NONE;
	
	for (int32 i = 0; i < Enemies.Num(); ++i)
	{
		if (Enemies[i].IsAlive())
		{
			float Distance = FVector::Dist(Position, Enemies[i].Position);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				NearestIndex = i;
			}
		}
	}
	
	if (NearestIndex != INDEX_NONE)
	{
		OutEnemy = Enemies[NearestIndex];
		return true;
	}
	
	return false;
}

void UEnemyManager::BroadcastEnemyEvent(const FCombatEvent& Event)
{
	OnEnemyEvent.Broadcast(Event);
}

void UEnemyManager::UpdateEnemyMovementFalling(FEnemyData& Enemy, float DeltaTime)
{
	// 下落式场景：简单的左右移动
	// 这里可以根据需求实现更复杂的移动逻辑
	
	// 暂时不移动，等待后续实现
}

void UEnemyManager::UpdateEnemyMovementCircular(FEnemyData& Enemy, float DeltaTime)
{
	// 环形场景：沿圆环移动
	UCircularSceneManager* CircularScene = Cast<UCircularSceneManager>(SceneManager.GetObject());
	if (!CircularScene)
	{
		return;
	}
	
	// 获取当前角度
	float CurrentAngle = Enemy.ExtraData.FindRef(TEXT("Angle"));
	
	// 更新角度
	float NewAngle = CircularScene->UpdateEnemyAngle(CurrentAngle, EnemyAngularVelocity, DeltaTime);
	
	// 更新位置
	Enemy.Position = CircularScene->GetEnemyPosition(NewAngle);
	
	// 存储新角度
	Enemy.ExtraData.Add(TEXT("Angle"), NewAngle);
}

void UEnemyManager::UpdateEnemyAttack(FEnemyData& Enemy, float DeltaTime)
{
	// 攻击逻辑：暂时不实现
	// 这里可以根据需求实现敌人的攻击行为
}

int32 UEnemyManager::FindEnemyIndex(FGuid EnemyID) const
{
	for (int32 i = 0; i < Enemies.Num(); ++i)
	{
		if (Enemies[i].ID == EnemyID)
		{
			return i;
		}
	}
	
	return INDEX_NONE;
}
