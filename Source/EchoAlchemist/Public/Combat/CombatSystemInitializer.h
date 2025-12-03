// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatConfig.h"
#include "CombatSceneTypes.h"
#include "CombatSystemInitializer.generated.h"

class UCombatManager;

/**
 * 战斗系统初始化器
 * 
 * 负责创建和初始化所有战斗系统组件，将复杂的初始化流程封装在一个函数中。
 * 
 * 设计目标：
 * - **简化蓝图**：将10多个节点的初始化链条缩减为1个节点。
 * - **提高可维护性**：初始化逻辑集中在C++中，易于修改和扩展。
 * - **减少错误**：避免在蓝图中遗漏步骤或连接错误。
 * 
 * @see UCombatManager
 * @see FCombatConfig
 */
UCLASS()
class ECHOALCHEMIST_API UCombatSystemInitializer : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ========== 基础初始化 ==========
    
    /**
     * 使用难度等级初始化战斗系统
     * @param WorldContextObject 世界上下文对象，用于创建新的UObject
     * @param Difficulty 难度等级（1-10），将用于创建对应的FCombatConfig
     * @param SceneType 场景类型（下落式或环形）
     * @return 初始化后的战斗管理器。如果失败，则返回nullptr。
     */
    UFUNCTION(BlueprintCallable, Category = "Combat|Initialization", meta = (WorldContext = "WorldContextObject"))
    static UCombatManager* InitializeCombatSystem(
        UObject* WorldContextObject,
        int32 Difficulty = 5,
        ECombatSceneType SceneType = ECombatSceneType::Falling
    );
    
    /**
     * 使用自定义配置初始化战斗系统
     * @param WorldContextObject 世界上下文对象
     * @param Config 自定义的战斗配置
     * @param SceneType 场景类型
     * @return 初始化后的战斗管理器。如果失败，则返回nullptr。
     */
    UFUNCTION(BlueprintCallable, Category = "Combat|Initialization", meta = (WorldContext = "WorldContextObject"))
    static UCombatManager* InitializeCombatSystemWithConfig(
        UObject* WorldContextObject,
        const FCombatConfig& Config,
        ECombatSceneType SceneType = ECombatSceneType::Falling
    );
    
    // ========== 验证 ==========
    
    /**
     * 验证战斗系统是否已正确初始化
     * @param CombatManager 战斗管理器实例
     * @return 如果战斗管理器及其所有关键子系统都有效，则返回true
     */
    UFUNCTION(BlueprintPure, Category = "Combat|Initialization")
    static bool IsCombatSystemValid(UCombatManager* CombatManager);
    
    /**
     * 获取最后的初始化错误信息
     * @return 如果发生错误，返回错误信息字符串；否则返回空字符串
     */
    UFUNCTION(BlueprintPure, Category = "Combat|Initialization")
    static FString GetLastInitializationError();

private:
    // ========== 内部辅助函数 ==========
    
    /**
     * 根据场景类型创建对应的场景管理器
     * @param WorldContextObject 世界上下文对象
     * @param SceneType 场景类型
     * @return 场景管理器的接口。如果失败，则返回nullptr。
     */
    static TScriptInterface<ISceneManager> CreateSceneManager(UObject* WorldContextObject, ECombatSceneType SceneType);
    
    /**
     * 记录初始化错误并更新错误信息状态
     * @param ErrorMessage 要记录的错误信息
     */
    static void LogInitializationError(const FString& ErrorMessage);
    
    // ========== 静态状态变量 ==========
    
    /** 用于存储最后一次初始化失败时的错误信息 */
    static FString LastInitializationError;
};
