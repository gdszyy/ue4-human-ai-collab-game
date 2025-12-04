// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 战斗系统辅助函数
 * 
 * 提供通用的辅助函数，减少重复代码。
 */
namespace CombatSystemHelpers
{
	/**
	 * 安全创建UObject并记录错误
	 * 
	 * @tparam T UObject类型
	 * @param Outer 外部对象
	 * @param OutErrorMessage 错误信息（输出）
	 * @return 创建的对象指针，失败返回nullptr
	 */
	template<typename T>
	T* CreateObjectSafe(UObject* Outer, FString& OutErrorMessage)
	{
		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must derive from UObject");
		
		if (!Outer)
		{
			OutErrorMessage = TEXT("Outer object is null.");
			return nullptr;
		}
		
		T* NewObject = ::NewObject<T>(Outer);
		if (!NewObject)
		{
			OutErrorMessage = FString::Printf(TEXT("Failed to create %s."), *T::StaticClass()->GetName());
			return nullptr;
		}
		
		return NewObject;
	}
	
	/**
	 * 记录初始化错误
	 * 
	 * @param ErrorMessage 错误信息
	 */
	inline void LogError(const FString& ErrorMessage)
	{
		UE_LOG(LogTemp, Error, TEXT("CombatSystem: %s"), *ErrorMessage);
	}
}
