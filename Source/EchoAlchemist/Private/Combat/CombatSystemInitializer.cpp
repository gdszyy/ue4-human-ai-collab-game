// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/CombatSystemInitializer.h"
#include "Combat/CombatManager.h"
#include "Combat/CombatPhysicsIntegrator.h"
#include "Combat/EnemyManager.h"
#include "Physics/MarblePhysicsSystem.h"
#include "Physics/CollisionManager.h"
#include "Combat/FallingSceneManager.h"
#include "Combat/CircularSceneManager.h"
#include "Combat/CombatBlueprintLibrary.h"

// Initialize static variable
FString UCombatSystemInitializer::LastInitializationError = TEXT("");

UCombatManager* UCombatSystemInitializer::InitializeCombatSystem(
    UObject* WorldContextObject,
    int32 Difficulty,
    ECombatSceneType SceneType
)
{
    // Create config from difficulty
    FCombatConfig Config = UCombatBlueprintLibrary::CreateCombatConfig(Difficulty);
    return InitializeCombatSystemWithConfig(WorldContextObject, Config, SceneType);
}

UCombatManager* UCombatSystemInitializer::InitializeCombatSystemWithConfig(
    UObject* WorldContextObject,
    const FCombatConfig& Config,
    ECombatSceneType SceneType
)
{
    LastInitializationError.Empty();

    if (!WorldContextObject)
    {
        LogInitializationError(TEXT("WorldContextObject is null."));
        return nullptr;
    }

    // 1. Create Scene Manager
    TScriptInterface<ISceneManager> SceneManager = CreateSceneManager(WorldContextObject, SceneType);
    if (!SceneManager)
    {
        // Error already logged in CreateSceneManager
        return nullptr;
    }

    // 2. Create Combat Manager
    UCombatManager* CombatManager = NewObject<UCombatManager>(WorldContextObject);
    if (!CombatManager)
    {
        LogInitializationError(TEXT("Failed to create UCombatManager."));
        return nullptr;
    }
    CombatManager->Initialize(Config, SceneManager);

    // 3. Create Enemy Manager
    UEnemyManager* EnemyManager = NewObject<UEnemyManager>(WorldContextObject);
    if (!EnemyManager)
    {
        LogInitializationError(TEXT("Failed to create UEnemyManager."));
        return nullptr;
    }
    // EnemyManager->Initialize(); // Assuming EnemyManager has an Initialize function

    // 4. Create Physics System
    UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>(WorldContextObject);
    if (!PhysicsSystem)
    {
        LogInitializationError(TEXT("Failed to create UMarblePhysicsSystem."));
        return nullptr;
    }
    // PhysicsSystem->Initialize(); // Assuming it has an Initialize function

    // 5. Create Collision Manager
    UCollisionManager* CollisionManager = NewObject<UCollisionManager>(WorldContextObject);
    if (!CollisionManager)
    {
        LogInitializationError(TEXT("Failed to create UCollisionManager."));
        return nullptr;
    }
    // CollisionManager->Initialize(); // Assuming it has an Initialize function

    // 6. Create Physics Integrator
    UCombatPhysicsIntegrator* Integrator = NewObject<UCombatPhysicsIntegrator>(WorldContextObject);
    if (!Integrator)
    {
        LogInitializationError(TEXT("Failed to create UCombatPhysicsIntegrator."));
        return nullptr;
    }
    Integrator->Initialize(CombatManager, EnemyManager, PhysicsSystem, CollisionManager);

    // 7. Set Integrator in Combat Manager
    CombatManager->SetPhysicsIntegrator(Integrator);

    UE_LOG(LogTemp, Log, TEXT("Combat System Initialized Successfully."));

    return CombatManager;
}

bool UCombatSystemInitializer::IsCombatSystemValid(UCombatManager* CombatManager)
{
    if (!CombatManager)
    {
        return false;
    }

    if (!CombatManager->GetSceneManager().GetInterface())
    {
        return false;
    }

    if (!CombatManager->GetPhysicsIntegrator())
    {
        return false;
    }

    // We can add more checks here for the sub-systems of the integrator if needed

    return true;
}

FString UCombatSystemInitializer::GetLastInitializationError()
{
    return LastInitializationError;
}

TScriptInterface<ISceneManager> UCombatSystemInitializer::CreateSceneManager(UObject* WorldContextObject, ECombatSceneType SceneType)
{
    UClass* SceneManagerClass = nullptr;
    if (SceneType == ECombatSceneType::Falling)
    {
        SceneManagerClass = UFallingSceneManager::StaticClass();
    }
    else if (SceneType == ECombatSceneType::Circular)
    {
        SceneManagerClass = UCircularSceneManager::StaticClass();
    }
    else
    {
        LogInitializationError(FString::Printf(TEXT("Invalid SceneType: %d"), SceneType));
        return nullptr;
    }

    UObject* SceneManagerObject = NewObject<UObject>(WorldContextObject, SceneManagerClass);
    if (!SceneManagerObject)
    {
        LogInitializationError(FString::Printf(TEXT("Failed to create SceneManager of type %s."), *SceneManagerClass->GetName()));
        return nullptr;
    }

    TScriptInterface<ISceneManager> SceneManager;
    SceneManager.SetObject(SceneManagerObject);
    SceneManager.SetInterface(Cast<ISceneManager>(SceneManagerObject));

    if (!SceneManager)
    {
        LogInitializationError(FString::Printf(TEXT("Failed to cast SceneManager object to ISceneManager interface.")));
        return nullptr;
    }

    return SceneManager;
}

void UCombatSystemInitializer::LogInitializationError(const FString& ErrorMessage)
{
    LastInitializationError = ErrorMessage;
    UE_LOG(LogTemp, Error, TEXT("CombatSystemInitializer: %s"), *ErrorMessage);
}
