// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatSceneTypes.generated.h"

/**
 * 战斗场景类型
 * 用于区分不同的战斗场景，例如下落式或环形
 */
UENUM(BlueprintType)
enum class ECombatSceneType : uint8
{
    Falling     UMETA(DisplayName = "Falling"),
    Circular    UMETA(DisplayName = "Circular")
};
