# 特殊物理效果系统 (Special Physics Effects System)

## 概述

特殊物理效果系统为魔力露珠提供各种魔法效果，包括引力场、传送、分裂、速度修改和连锁触发等。这些效果通过"魔法添加剂"（Magic Modifiers）实现，可以在战斗中对魔力露珠施加影响。

## 核心类

### 1. UPhysicsEffectBase (抽象基类)

所有特殊物理效果的基类，定义统一的接口。

**核心方法**：
- `ApplyEffect(FMarbleState& Marble, float DeltaTime)`: 对魔力露珠应用效果
- `ShouldRemove()`: 判断效果是否应该移除
- `GetEffectType()`: 获取效果类型

### 2. UGravityFieldEffect (引力奇点效果)

在指定位置创建引力场，吸引附近的魔力露珠。

**参数**：
- `FieldCenter`: 引力场中心位置
- `FieldRadius`: 引力场作用半径
- `GravityStrength`: 引力强度
- `Duration`: 持续时间（-1表示永久）

**物理公式**：
```
F = GravityStrength * (1 - Distance / FieldRadius)
Direction = Normalize(FieldCenter - MarblePosition)
Acceleration = F * Direction
```

### 3. UWormholeEffect (虫洞传送效果)

在两个位置之间创建虫洞，魔力露珠进入入口后传送到出口。

**参数**：
- `EntrancePosition`: 入口位置
- `ExitPosition`: 出口位置
- `EntranceRadius`: 入口半径
- `ExitVelocityMultiplier`: 出口速度倍数（默认1.0）
- `Duration`: 持续时间（-1表示永久）

**传送逻辑**：
```
if (Distance(MarblePosition, EntrancePosition) < EntranceRadius)
{
    MarblePosition = ExitPosition
    MarbleVelocity *= ExitVelocityMultiplier
}
```

### 4. USplitEffect (弹珠分裂效果)

魔力露珠碰撞时分裂成多个子代魔药。

**参数**：
- `SplitCount`: 分裂数量（2-5个）
- `SplitAngleRange`: 分裂角度范围（度）
- `SpeedMultiplier`: 子代速度倍数
- `PotencyDistribution`: 威力分配方式（均分/随机）
- `MaxGeneration`: 最大代数（超过则不分裂）

**分裂逻辑**：
```
OnCollision:
    if (Marble.Generation < MaxGeneration)
    {
        for (i = 0; i < SplitCount; i++)
        {
            ChildMarble = CreateChildMarble()
            ChildMarble.Generation = Marble.Generation + 1
            ChildMarble.Velocity = CalculateSplitVelocity(i)
            ChildMarble.Potency = Marble.Potency / SplitCount
        }
        DestroyMarble(Marble)
    }
```

### 5. UVelocityModifierEffect (速度修改效果)

修改魔力露珠的速度（加速/减速）。

**参数**：
- `ModifierType`: 修改类型（Multiply/Add/Set）
- `ModifierValue`: 修改值
- `AffectArea`: 影响区域（Point/Box/Sphere）
- `AreaCenter`: 区域中心
- `AreaSize`: 区域大小
- `Duration`: 持续时间（-1表示永久）

**速度修改**：
```
if (IsInArea(MarblePosition))
{
    switch (ModifierType)
    {
        case Multiply: Velocity *= ModifierValue; break;
        case Add: Velocity += ModifierValue; break;
        case Set: Velocity = ModifierValue; break;
    }
}
```

### 6. UChainTriggerEffect (连锁触发效果)

魔力露珠碰撞时触发连锁反应，发射次级魔药。

**参数**：
- `TriggerCount`: 触发次数（每次碰撞）
- `ProjectileSpeed`: 次级魔药速度
- `ProjectileAngleSpread`: 发射角度分散
- `ProjectilePotency`: 次级魔药威力
- `MaxChainDepth`: 最大连锁深度

**连锁逻辑**：
```
OnCollision:
    if (ChainDepth < MaxChainDepth)
    {
        for (i = 0; i < TriggerCount; i++)
        {
            SecondaryMarble = CreateSecondaryMarble()
            SecondaryMarble.Velocity = CalculateProjectileVelocity(i)
            SecondaryMarble.Potency = ProjectilePotency
            SecondaryMarble.ChainDepth = ChainDepth + 1
        }
    }
```

## 效果管理器

### UPhysicsEffectManager

管理所有活动的物理效果，负责效果的添加、移除和更新。

**核心方法**：
- `AddEffect(UPhysicsEffectBase* Effect)`: 添加效果
- `RemoveEffect(FGuid EffectID)`: 移除效果
- `UpdateEffects(TArray<FMarbleState>& Marbles, float DeltaTime)`: 更新所有效果
- `ClearAllEffects()`: 清空所有效果

**更新逻辑**：
```
void UpdateEffects(TArray<FMarbleState>& Marbles, float DeltaTime)
{
    for (Effect in ActiveEffects)
    {
        for (Marble in Marbles)
        {
            Effect->ApplyEffect(Marble, DeltaTime)
        }
        
        if (Effect->ShouldRemove())
        {
            RemoveEffect(Effect->GetID())
        }
    }
}
```

## 蓝图使用指南

### 创建引力奇点

```
// 1. 创建引力场效果
GravityEffect = NewObject<UGravityFieldEffect>()
GravityEffect->FieldCenter = FVector(0, 0, 100)
GravityEffect->FieldRadius = 500.0f
GravityEffect->GravityStrength = 1000.0f
GravityEffect->Duration = 5.0f  // 持续5秒

// 2. 添加到效果管理器
EffectManager->AddEffect(GravityEffect)
```

### 创建虫洞传送

```
// 1. 创建虫洞效果
WormholeEffect = NewObject<UWormholeEffect>()
WormholeEffect->EntrancePosition = FVector(-500, 0, 100)
WormholeEffect->ExitPosition = FVector(500, 0, 100)
WormholeEffect->EntranceRadius = 50.0f
WormholeEffect->ExitVelocityMultiplier = 1.5f
WormholeEffect->Duration = -1  // 永久

// 2. 添加到效果管理器
EffectManager->AddEffect(WormholeEffect)
```

### 创建分裂效果

```
// 1. 创建分裂效果
SplitEffect = NewObject<USplitEffect>()
SplitEffect->SplitCount = 3
SplitEffect->SplitAngleRange = 120.0f
SplitEffect->SpeedMultiplier = 0.8f
SplitEffect->MaxGeneration = 2  // 最多分裂到第2代

// 2. 将效果附加到魔力露珠
Marble->AttachEffect(SplitEffect)
```

### 创建速度修改区域

```
// 1. 创建加速区域
AcceleratorEffect = NewObject<UVelocityModifierEffect>()
AcceleratorEffect->ModifierType = EVelocityModifierType::Multiply
AcceleratorEffect->ModifierValue = 2.0f  // 2倍速
AcceleratorEffect->AffectArea = EAreaType::Sphere
AcceleratorEffect->AreaCenter = FVector(0, 0, 100)
AcceleratorEffect->AreaSize = FVector(200, 200, 200)
AcceleratorEffect->Duration = -1

// 2. 添加到效果管理器
EffectManager->AddEffect(AcceleratorEffect)
```

### 创建连锁触发

```
// 1. 创建连锁效果
ChainEffect = NewObject<UChainTriggerEffect>()
ChainEffect->TriggerCount = 5  // 每次碰撞发射5个次级魔药
ChainEffect->ProjectileSpeed = 800.0f
ChainEffect->ProjectileAngleSpread = 60.0f
ChainEffect->ProjectilePotency = 10.0f
ChainEffect->MaxChainDepth = 2

// 2. 将效果附加到魔力露珠
Marble->AttachEffect(ChainEffect)
```

## 与物理系统的集成

### 在MarblePhysicsSystem中集成

```cpp
// 在UMarblePhysicsSystem中添加效果管理器
UPROPERTY()
UPhysicsEffectManager* EffectManager;

// 在Tick中更新效果
void UMarblePhysicsSystem::Tick(float DeltaTime)
{
    // 1. 更新物理模拟
    UpdatePhysics(DeltaTime);
    
    // 2. 应用特殊效果
    if (EffectManager)
    {
        TArray<FMarbleState> Marbles = GetAllMarbles();
        EffectManager->UpdateEffects(Marbles, DeltaTime);
    }
    
    // 3. 处理碰撞和分裂
    HandleCollisions();
}
```

## 性能优化

### 空间分割优化

对于区域效果（引力场、速度修改），使用空间网格优化：

```cpp
// 只检查在效果范围内的魔力露珠
TArray<FMarbleState> NearbyMarbles;
SpatialGrid->Query(EffectCenter, EffectRadius, NearbyMarbles);

for (FMarbleState& Marble : NearbyMarbles)
{
    Effect->ApplyEffect(Marble, DeltaTime);
}
```

### 效果池化

复用效果对象，避免频繁创建和销毁：

```cpp
TArray<UPhysicsEffectBase*> EffectPool;

UPhysicsEffectBase* AcquireEffect(TSubclassOf<UPhysicsEffectBase> EffectClass)
{
    // 从池中获取或创建新效果
}

void ReleaseEffect(UPhysicsEffectBase* Effect)
{
    // 重置并放回池中
}
```

## 验收标准

### 引力奇点效果
- ✅ 可以在指定位置创建引力场
- ✅ 引力强度随距离衰减
- ✅ 魔力露珠被正确吸引
- ✅ 持续时间到期后自动移除

### 虫洞传送效果
- ✅ 魔力露珠进入入口后传送到出口
- ✅ 速度倍数正确应用
- ✅ 传送位置准确
- ✅ 不会重复传送

### 弹珠分裂效果
- ✅ 碰撞时正确分裂
- ✅ 子代数量正确
- ✅ 子代速度和角度正确
- ✅ 威力正确分配
- ✅ 代数限制正确

### 速度修改效果
- ✅ 区域检测正确
- ✅ 速度修改正确应用
- ✅ 支持多种修改类型
- ✅ 持续时间正确

### 连锁触发效果
- ✅ 碰撞时正确触发
- ✅ 次级魔药数量和速度正确
- ✅ 连锁深度限制正确
- ✅ 威力正确设置

### 性能测试
- ✅ 100个魔力露珠 + 10个效果，帧率 > 30 FPS
- ✅ 分裂效果不会导致魔力露珠数量失控
- ✅ 连锁触发不会导致无限循环

## 单元测试

### 测试文件结构

```
Source/EchoAlchemist/Tests/Physics/
├── GravityFieldEffectTest.cpp
├── WormholeEffectTest.cpp
├── SplitEffectTest.cpp
├── VelocityModifierEffectTest.cpp
├── ChainTriggerEffectTest.cpp
└── PhysicsEffectManagerTest.cpp
```

### 测试用例示例

```cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGravityFieldEffectTest, 
    "EchoAlchemist.Physics.GravityFieldEffect", 
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGravityFieldEffectTest::RunTest(const FString& Parameters)
{
    // 1. 创建引力场效果
    UGravityFieldEffect* Effect = NewObject<UGravityFieldEffect>();
    Effect->FieldCenter = FVector(0, 0, 0);
    Effect->FieldRadius = 500.0f;
    Effect->GravityStrength = 1000.0f;
    
    // 2. 创建测试魔力露珠
    FMarbleState Marble;
    Marble.Position = FVector(300, 0, 0);  // 距离中心300cm
    Marble.Velocity = FVector(0, 0, 0);
    
    // 3. 应用效果
    Effect->ApplyEffect(Marble, 0.016f);  // 1帧
    
    // 4. 验证魔力露珠被吸引
    TestTrue("Marble should be attracted to gravity field", 
        Marble.Velocity.X < 0);  // 向中心移动
    
    return true;
}
```

## 注意事项

1. **效果叠加**：多个效果可以同时作用于同一个魔力露珠
2. **效果优先级**：某些效果可能需要优先级控制（如传送 > 引力）
3. **性能监控**：定期检查效果数量，避免过多效果影响性能
4. **代数限制**：分裂效果必须遵守代数限制，避免无限分裂
5. **连锁深度**：连锁触发必须有深度限制，避免无限循环

## 后续任务

- **VOI-28**: 物理系统集成测试
- **VOI-29**: 蓝图示例创建
- **效果编辑器**: 在编辑器中可视化编辑效果参数

## 版本信息

- **版本**: 1.0.0
- **任务**: VOI-27
- **作者**: Manus
- **日期**: 2025-12-08
