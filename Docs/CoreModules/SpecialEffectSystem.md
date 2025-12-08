# 特殊效果系统 (Special Effect System)

## 概述

特殊效果系统管理所有特殊物理效果，包括引力奇点、虫洞传送、弹珠分裂、速度修改和连锁触发。这些效果为游戏提供了丰富的战术选择和视觉表现。

## 核心类

### 1. USpecialEffectSystem
特殊效果系统的核心类，负责管理所有特殊效果的创建、更新和销毁。

### 2. FSpecialEffectData
特殊效果数据结构体，存储效果的类型、参数和状态信息。

### 3. USpecialEffectBlueprintLibrary
蓝图函数库，提供便捷的接口用于创建和配置特殊效果参数。

## 特殊效果类型

### 1. 引力奇点 (Gravity Well)

**描述**：对周围弹珠施加引力，将其吸引到奇点位置。

**参数**：
- `Position` - 奇点位置
- `GravityStrength` - 引力强度（单位：cm/s²）
- `EffectRadius` - 影响半径（单位：cm）
- `Duration` - 持续时间（单位：秒，0表示永久）
- `bDestroyOnReach` - 是否在弹珠到达中心时销毁

**使用场景**：
- 吸引敌人到特定位置
- 创建黑洞效果
- 聚集弹珠进行连锁攻击

**蓝图示例**：
```
// 创建弱引力奇点
FGravityWellParams Params = USpecialEffectBlueprintLibrary::MakeWeakGravityWell(TargetPosition);
FGuid EffectID = EffectSystem->CreateGravityWell(Params);

// 创建强引力奇点（黑洞）
FGravityWellParams Params = USpecialEffectBlueprintLibrary::MakeStrongGravityWell(TargetPosition);
FGuid EffectID = EffectSystem->CreateGravityWell(Params);
```

### 2. 虫洞传送 (Wormhole)

**描述**：将进入虫洞入口的弹珠传送到出口位置。

**参数**：
- `EntrancePosition` - 虫洞入口位置
- `ExitPosition` - 虫洞出口位置
- `EntranceRadius` - 入口半径（单位：cm）
- `ExitSpeedMultiplier` - 出口速度倍率
- `Duration` - 持续时间（单位：秒，0表示永久）

**使用场景**：
- 快速移动弹珠到战场另一侧
- 创建战术传送门
- 绕过障碍物

**蓝图示例**：
```
// 创建虫洞
FWormholeParams Params = USpecialEffectBlueprintLibrary::MakeWormholeParams(
    EntrancePosition,
    ExitPosition,
    50.0f,  // EntranceRadius
    1.0f,   // ExitSpeedMultiplier
    10.0f   // Duration
);
FGuid EffectID = EffectSystem->CreateWormhole(Params);
```

### 3. 弹珠分裂 (Split)

**描述**：将一个弹珠分裂成多个子弹珠。

**参数**：
- `SplitCount` - 分裂数量
- `SplitAngle` - 分裂角度（单位：度）
- `SpeedMultiplier` - 子弹珠速度倍率
- `PotencyMultiplier` - 子弹珠药效倍率
- `RadiusMultiplier` - 子弹珠半径倍率

**使用场景**：
- 增加攻击覆盖范围
- 创建弹幕效果
- 对多个敌人造成伤害

**蓝图示例**：
```
// 二分裂
FSplitParams Params = USpecialEffectBlueprintLibrary::MakeBinarySplit();
TArray<FMarbleState> NewMarbles;
int32 Count = EffectSystem->ApplySplitEffect(MarbleState, Params, NewMarbles);

// 三分裂
FSplitParams Params = USpecialEffectBlueprintLibrary::MakeTripleSplit();
TArray<FMarbleState> NewMarbles;
int32 Count = EffectSystem->ApplySplitEffect(MarbleState, Params, NewMarbles);

// 自定义分裂
FSplitParams Params = USpecialEffectBlueprintLibrary::MakeSplitParams(
    5,      // SplitCount
    60.0f,  // SplitAngle
    0.7f,   // SpeedMultiplier
    0.4f,   // PotencyMultiplier
    0.6f    // RadiusMultiplier
);
TArray<FMarbleState> NewMarbles;
int32 Count = EffectSystem->ApplySplitEffect(MarbleState, Params, NewMarbles);
```

**注意事项**：
- 原弹珠需要手动从物理系统中移除
- 新弹珠需要手动添加到物理系统
- 子弹珠的Generation会自动+1

### 4. 速度修改 (Speed Modifier)

**描述**：修改弹珠的速度（加速或减速）。

**参数**：
- `SpeedMultiplier` - 速度倍率（>1加速，<1减速）
- `bApplyImmediately` - 是否立即应用
- `Duration` - 持续时间（单位：秒，0表示瞬间）

**使用场景**：
- 加速弹珠以提高伤害
- 减速弹珠以延长停留时间
- 创建时间膨胀效果

**蓝图示例**：
```
// 加速（2倍速）
FSpeedModifierParams Params = USpecialEffectBlueprintLibrary::MakeSpeedBoost();
FMarbleState ModifiedMarble;
bool bSuccess = EffectSystem->ApplySpeedModifier(MarbleState, Params, ModifiedMarble);

// 减速（0.5倍速）
FSpeedModifierParams Params = USpecialEffectBlueprintLibrary::MakeSpeedSlow();
FMarbleState ModifiedMarble;
bool bSuccess = EffectSystem->ApplySpeedModifier(MarbleState, Params, ModifiedMarble);

// 自定义速度修改
FSpeedModifierParams Params = USpecialEffectBlueprintLibrary::MakeSpeedModifierParams(
    1.5f,   // SpeedMultiplier
    true,   // bApplyImmediately
    0.0f    // Duration
);
FMarbleState ModifiedMarble;
bool bSuccess = EffectSystem->ApplySpeedModifier(MarbleState, Params, ModifiedMarble);
```

### 5. 连锁触发 (Chain Trigger)

**描述**：触发时发射次级弹珠。

**参数**：
- `SecondaryCount` - 次级弹珠数量
- `AngleRange` - 发射角度范围（单位：度）
- `SecondarySpeed` - 次级弹珠速度
- `PotencyMultiplier` - 次级弹珠药效倍率
- `SecondaryRadius` - 次级弹珠半径

**使用场景**：
- 创建连锁反应
- 对周围敌人造成范围伤害
- 触发多段攻击

**蓝图示例**：
```
// 触发连锁效果
FChainTriggerParams Params = USpecialEffectBlueprintLibrary::MakeChainTriggerParams(
    3,        // SecondaryCount
    60.0f,    // AngleRange
    800.0f,   // SecondarySpeed
    0.3f,     // PotencyMultiplier
    5.0f      // SecondaryRadius
);

TArray<FMarbleState> SecondaryMarbles;
int32 Count = EffectSystem->ApplyChainTrigger(
    TriggerPosition,
    TriggerDirection,
    Params,
    SecondaryMarbles
);

// 将次级弹珠添加到物理系统
for (const FMarbleState& Secondary : SecondaryMarbles)
{
    PhysicsSystem->AddMarble(Secondary);
}
```

## 蓝图使用指南

### 初始化特殊效果系统

```
// 1. 创建特殊效果系统实例
EffectSystem = NewObject<USpecialEffectSystem>();

// 2. 初始化
EffectSystem->Initialize();
```

### 更新特殊效果（在Tick中）

```
void AYourActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 获取所有弹珠状态
    TArray<FMarbleState> Marbles = PhysicsSystem->GetAllMarbles();
    
    // 更新特殊效果系统
    TArray<FMarbleState> ModifiedMarbles;
    EffectSystem->Tick(DeltaTime, Marbles, ModifiedMarbles);
    
    // 应用修改后的弹珠状态
    for (const FMarbleState& Modified : ModifiedMarbles)
    {
        PhysicsSystem->UpdateMarbleState(Modified.ID, Modified);
    }
}
```

### 订阅特殊效果事件

```
// 在BeginPlay中绑定事件
void AYourActor::BeginPlay()
{
    Super::BeginPlay();
    
    // 绑定特殊效果触发事件
    EffectSystem->OnEffectTriggered.AddDynamic(this, &AYourActor::OnEffectTriggered);
}

// 事件处理函数
void AYourActor::OnEffectTriggered(ESpecialEffectType EffectType, FGuid TargetMarbleID)
{
    UE_LOG(LogTemp, Log, TEXT("Effect triggered: %s, Target: %s"),
        *USpecialEffectBlueprintLibrary::GetEffectTypeName(EffectType),
        *TargetMarbleID.ToString());
    
    // 播放特效
    SpawnEffectVisual(EffectType, TargetMarbleID);
}
```

### 移除特殊效果

```
// 手动移除效果
bool bSuccess = EffectSystem->RemoveEffect(EffectID);

// 效果会在Duration到期后自动移除
```

## 性能优化

### 效果数量控制

建议同时存在的效果数量：
- 引力奇点：≤5个
- 虫洞：≤3个
- 分裂/速度修改/连锁触发：瞬间效果，无限制

### 更新频率

特殊效果系统的Tick频率建议：
- 正常情况：每帧更新（60FPS）
- 性能受限：每2帧更新（30FPS）

### 效果范围

合理设置效果范围以避免不必要的计算：
- 引力奇点：EffectRadius建议≤500cm
- 虫洞：EntranceRadius建议≤100cm

## 与其他系统的集成

### 与物理系统的集成

```
// 1. 物理系统更新弹珠状态
PhysicsSystem->Tick(DeltaTime);

// 2. 特殊效果系统处理效果
TArray<FMarbleState> Marbles = PhysicsSystem->GetAllMarbles();
TArray<FMarbleState> ModifiedMarbles;
EffectSystem->Tick(DeltaTime, Marbles, ModifiedMarbles);

// 3. 应用修改后的状态
for (const FMarbleState& Modified : ModifiedMarbles)
{
    PhysicsSystem->UpdateMarbleState(Modified.ID, Modified);
}
```

### 与战斗系统的集成

```
// 在碰撞时触发分裂效果
void AYourActor::OnMarbleHitEnemy(FGuid MarbleID, FGuid EnemyID)
{
    // 获取弹珠状态
    FMarbleState Marble;
    PhysicsSystem->GetMarbleState(MarbleID, Marble);
    
    // 应用分裂效果
    FSplitParams Params = USpecialEffectBlueprintLibrary::MakeBinarySplit();
    TArray<FMarbleState> NewMarbles;
    EffectSystem->ApplySplitEffect(Marble, Params, NewMarbles);
    
    // 移除原弹珠
    PhysicsSystem->RemoveMarble(MarbleID);
    
    // 添加新弹珠
    for (const FMarbleState& NewMarble : NewMarbles)
    {
        PhysicsSystem->AddMarble(NewMarble);
    }
}
```

## 注意事项

1. **初始化顺序**：必须先调用Initialize才能使用其他功能
2. **Tick调用**：每帧必须调用Tick来更新持续性效果
3. **弹珠管理**：分裂和连锁触发会生成新弹珠，需要手动添加到物理系统
4. **效果过期**：持续性效果会在Duration到期后自动移除
5. **性能监控**：定期检查效果数量，避免过多效果影响性能

## 后续任务

- **VOI-28**: 集成测试
- **视觉效果**: 在蓝图中添加特效和粒子系统
- **音效**: 添加效果触发音效

## 版本信息

- **版本**: 1.0.0
- **任务**: VOI-27
- **作者**: Manus
- **日期**: 2025-12-08
