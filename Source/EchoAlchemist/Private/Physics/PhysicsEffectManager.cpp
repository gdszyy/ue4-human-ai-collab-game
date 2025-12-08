// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/PhysicsEffectManager.h"

void UPhysicsEffectManager::Initialize()
{
	if (bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PhysicsEffectManager] Already initialized"));
		return;
	}

	ActiveEffects.Empty();
	EffectMap.Empty();
	bIsInitialized = true;

	UE_LOG(LogTemp, Log, TEXT("[PhysicsEffectManager] Initialized"));
}

void UPhysicsEffectManager::Cleanup()
{
	ActiveEffects.Empty();
	EffectMap.Empty();
	bIsInitialized = false;

	UE_LOG(LogTemp, Log, TEXT("[PhysicsEffectManager] Cleaned up"));
}

FGuid UPhysicsEffectManager::AddEffect(UPhysicsEffectBase* Effect)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[PhysicsEffectManager] Cannot add effect: Not initialized"));
		return FGuid();
	}

	if (Effect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[PhysicsEffectManager] Cannot add null effect"));
		return FGuid();
	}

	FGuid EffectID = Effect->GetEffectID();

	// 检查是否已存在
	if (EffectMap.Contains(EffectID))
	{
		UE_LOG(LogTemp, Warning, TEXT("[PhysicsEffectManager] Effect already exists: %s"), *EffectID.ToString());
		return EffectID;
	}

	// 添加到列表和映射表
	ActiveEffects.Add(Effect);
	EffectMap.Add(EffectID, Effect);

	UE_LOG(LogTemp, Log, TEXT("[PhysicsEffectManager] Added effect: Type=%s, ID=%s"),
		*UEnum::GetValueAsString(Effect->GetEffectType()),
		*EffectID.ToString());

	return EffectID;
}

bool UPhysicsEffectManager::RemoveEffect(const FGuid& EffectID)
{
	UPhysicsEffectBase** EffectPtr = EffectMap.Find(EffectID);
	if (EffectPtr == nullptr)
	{
		return false;
	}

	UPhysicsEffectBase* Effect = *EffectPtr;

	// 从列表和映射表中移除
	ActiveEffects.Remove(Effect);
	EffectMap.Remove(EffectID);

	UE_LOG(LogTemp, Log, TEXT("[PhysicsEffectManager] Removed effect: ID=%s"), *EffectID.ToString());

	return true;
}

void UPhysicsEffectManager::RemoveAllEffects()
{
	int32 Count = ActiveEffects.Num();
	ActiveEffects.Empty();
	EffectMap.Empty();

	UE_LOG(LogTemp, Log, TEXT("[PhysicsEffectManager] Removed all effects: Count=%d"), Count);
}

UPhysicsEffectBase* UPhysicsEffectManager::GetEffect(const FGuid& EffectID) const
{
	UPhysicsEffectBase* const* EffectPtr = EffectMap.Find(EffectID);
	return EffectPtr ? *EffectPtr : nullptr;
}

TArray<UPhysicsEffectBase*> UPhysicsEffectManager::GetAllEffects() const
{
	return ActiveEffects;
}

TArray<UPhysicsEffectBase*> UPhysicsEffectManager::GetEffectsByType(EPhysicsEffectType EffectType) const
{
	TArray<UPhysicsEffectBase*> Result;

	for (UPhysicsEffectBase* Effect : ActiveEffects)
	{
		if (Effect && Effect->GetEffectType() == EffectType)
		{
			Result.Add(Effect);
		}
	}

	return Result;
}

void UPhysicsEffectManager::UpdateEffects(TArray<FMarbleState>& Marbles, float DeltaTime)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[PhysicsEffectManager] Cannot update effects: Not initialized"));
		return;
	}

	// 应用所有效果到所有魔力露珠
	for (UPhysicsEffectBase* Effect : ActiveEffects)
	{
		if (Effect == nullptr || !Effect->IsActive())
		{
			continue;
		}

		for (FMarbleState& Marble : Marbles)
		{
			Effect->ApplyEffect(Marble, DeltaTime);
		}
	}

	// 移除过期的效果
	int32 RemovedCount = RemoveExpiredEffects();

	if (RemovedCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[PhysicsEffectManager] Removed %d expired effects"), RemovedCount);
	}
}

void UPhysicsEffectManager::GetStatistics(int32& OutTotalEffects, int32& OutActiveEffects, int32& OutInactiveEffects) const
{
	OutTotalEffects = ActiveEffects.Num();
	OutActiveEffects = 0;
	OutInactiveEffects = 0;

	for (UPhysicsEffectBase* Effect : ActiveEffects)
	{
		if (Effect && Effect->IsActive())
		{
			OutActiveEffects++;
		}
		else
		{
			OutInactiveEffects++;
		}
	}
}

int32 UPhysicsEffectManager::RemoveExpiredEffects()
{
	TArray<FGuid> ExpiredEffects;

	// 查找过期的效果
	for (UPhysicsEffectBase* Effect : ActiveEffects)
	{
		if (Effect && Effect->ShouldRemove())
		{
			ExpiredEffects.Add(Effect->GetEffectID());
		}
	}

	// 移除过期的效果
	for (const FGuid& EffectID : ExpiredEffects)
	{
		RemoveEffect(EffectID);
	}

	return ExpiredEffects.Num();
}
