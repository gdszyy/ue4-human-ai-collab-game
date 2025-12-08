## 特殊物理效果系统 (Special Effects System)

**版本**: 1.0.0  
**任务**: VOI-27  
**作者**: Manus  
**日期**: 2025-12-08

---

## 概述

特殊物理效果系统为《回声炼金师》提供了丰富的魔药效果，包括引力场、传送、分裂、速度修改和连锁触发等。这些效果可以组合使用，创造出多样化的战斗策略和视觉体验。

---

## 核心类

### 1. USpecialEffectsManager

特殊效果管理器，负责管理和应用所有特殊物理效果。

### 2. 效果参数结构体

- **FGravitySingularityParams** - 引力奇点参数
- **FWormholeTeleportParams** - 虫洞传送参数
- **FMarbleSplitParams** - 弹珠分裂参数
- **FSpeedModifierParams** - 速度修改参数
- **FChainReactionParams** - 连锁触发参数

---

## 蓝图使用指南

### 初始化系统

在战斗管理器或关卡蓝图中创建特殊效果管理器：

```
Event BeginPlay
  ↓
Create Special Effects Manager
  ↓
Set EffectsManager (Variable)
```

### 1. 引力奇点效果

在指定位置创建引力场，吸引附近的魔力露珠。

**创建引力奇点**：
```
// 1. 创建参数
FGravitySingularityParams Params;
Params.Position = GetActorLocation();
Params.Strength = 500.0f;  // 引力强度（cm/s²）
Params.Radius = 300.0f;    // 影响半径（cm）
Params.Duration = 5.0f;    // 持续时间（秒，0=永久）

// 2. 创建引力奇点
FGuid SingularityID = EffectsManager->CreateGravitySingularity(Params);
```

**参数建议**：
- **引力强度**: 100-1000 cm/s²
- **影响半径**: 100-500 cm
- **持续时间**: 3-10秒（战术效果），0秒（永久陷阱）

**使用场景**：
- 控制敌人移动路径
- 创造魔药聚集点
- 设计解谜机关

### 2. 虫洞传送效果

将进入入口的魔力露珠传送到出口位置。

**创建虫洞**：
```
// 1. 创建参数
FWormholeTeleportParams Params;
Params.EntryPosition = EntryLocation;
Params.ExitPosition = ExitLocation;
Params.EntryRadius = 50.0f;
Params.bPreserveVelocity = true;  // 保持速度方向
Params.VelocityMultiplier = 1.0f; // 速度倍率

// 2. 创建虫洞
FGuid WormholeID = EffectsManager->CreateWormhole(Params);
```

**参数建议**：
- **入口半径**: 30-100 cm
- **速度倍率**: 0.5-2.0（可用于加速或减速）
- **保持速度**: true（战术传送），false（随机方向）

**使用场景**：
- 快速移动魔药到战场另一侧
- 创造传送网络
- 设计空间解谜

### 3. 弹珠分裂效果

魔力露珠碰撞后分裂成多个子代魔药。

**应用分裂效果**：
```
// 1. 创建参数
FMarbleSplitParams Params;
Params.SplitCount = 3;           // 分裂成3个
Params.SpeedMultiplier = 0.8f;   // 子代速度为父代的80%
Params.AngleSpread = 60.0f;      // 60度扩散角
Params.PotencyRatio = 0.5f;      // 子代药效为父代的50%
Params.MaxSplitDepth = 2;        // 最大分裂2次

// 2. 应用分裂（通常在碰撞事件中）
TArray<FMarbleState> ChildMarbles;
int32 Count = EffectsManager->ApplyMarbleSplit(ParentMarble, Params, ChildMarbles);

// 3. 将子代魔药添加到物理系统
for (const FMarbleState& Child : ChildMarbles)
{
    PhysicsSystem->AddMarble(Child);
}
```

**参数建议**：
- **分裂数量**: 2-5个（平衡性能和视觉效果）
- **速度倍率**: 0.6-1.0（避免子代过快）
- **角度扩散**: 30-90度（控制覆盖范围）
- **药效比例**: 0.3-0.7（平衡伤害输出）

**使用场景**：
- 群体攻击魔药
- 覆盖更大范围
- 创造弹幕效果

### 4. 速度修改效果

修改魔力露珠的速度（加速或减速）。

**应用速度修改**：
```
// 1. 创建加速参数
FSpeedModifierParams BoostParams;
BoostParams.Multiplier = 1.5f;  // 加速50%
BoostParams.Duration = 0.0f;    // 立即应用

// 2. 应用加速效果
EffectsManager->ApplySpeedModifier(Marble, BoostParams);

// 或创建减速参数
FSpeedModifierParams SlowParams;
SlowParams.Multiplier = 0.5f;  // 减速50%

// 应用减速效果
EffectsManager->ApplySpeedModifier(Marble, SlowParams);
```

**参数建议**：
- **加速倍率**: 1.2-2.0
- **减速倍率**: 0.3-0.8
- **持续时间**: 0（立即应用）或2-5秒（渐变效果）

**使用场景**：
- 进入加速区域
- 敌人减速陷阱
- 时间控制魔药

### 5. 连锁触发效果

魔力露珠碰撞后发射次级魔药。

**应用连锁触发**：
```
// 1. 创建参数
FChainReactionParams Params;
Params.ProjectileCount = 4;      // 发射4个次级魔药
Params.ProjectileSpeed = 800.0f; // 次级魔药速度
Params.AngleSpread = 90.0f;      // 90度扇形扩散
Params.Potency = 2.0f;           // 次级魔药药效
Params.DamageMultiplier = 0.5f;  // 次级魔药伤害为主魔药的50%
Params.MaxChainDepth = 2;        // 最大连锁2次

// 2. 应用连锁触发（通常在碰撞事件中）
TArray<FMarbleState> Projectiles;
int32 Count = EffectsManager->ApplyChainReaction(TriggerMarble, Params, Projectiles);

// 3. 将次级魔药添加到物理系统
for (const FMarbleState& Projectile : Projectiles)
{
    PhysicsSystem->AddMarble(Projectile);
}
```

**参数建议**：
- **次级魔药数量**: 3-8个
- **次级魔药速度**: 600-1000 cm/s
- **角度扩散**: 60-180度
- **伤害倍率**: 0.3-0.7

**使用场景**：
- 碰撞后爆炸效果
- 连锁反应魔药
- 范围攻击

---

## 完整使用示例

### 示例1：创造引力陷阱

```
Event BeginPlay
  ↓
Create Gravity Singularity
  - Position: (0, 0, 100)
  - Strength: 800
  - Radius: 400
  - Duration: 0 (永久)
  ↓
每帧调用
  ↓
Apply Effects to Marbles
  - Marbles: 所有活跃魔力露珠
  - DeltaTime: 帧时间
```

### 示例2：分裂魔药

```
On Marble Collision
  ↓
Apply Marble Split
  - Parent: 碰撞的魔力露珠
  - Split Count: 3
  - Speed Multiplier: 0.8
  - Angle Spread: 60
  ↓
Add Child Marbles to Physics System
```

### 示例3：连锁爆炸

```
On Marble Hit Enemy
  ↓
Apply Chain Reaction
  - Trigger: 碰撞的魔力露珠
  - Projectile Count: 6
  - Projectile Speed: 800
  - Angle Spread: 120
  ↓
Add Projectiles to Physics System
```

---

## 性能优化

### 1. 效果数量限制

建议同时存在的效果数量：
- **引力奇点**: 最多5个
- **虫洞**: 最多10个
- **分裂/连锁**: 根据魔力露珠总数动态调整

### 2. 自动过期

所有效果都支持自动过期：
- 设置`Duration > 0`来限制效果持续时间
- 系统会自动清理过期的效果
- 建议为临时效果设置合理的持续时间

### 3. 深度限制

分裂和连锁效果都有最大深度限制：
- **MaxSplitDepth**: 防止无限分裂
- **MaxChainDepth**: 防止无限连锁
- 建议设置为1-3，平衡效果和性能

---

## 注意事项

### 1. 效果应用顺序

每帧按以下顺序应用效果：
1. 引力场（修改速度）
2. 虫洞（修改位置）
3. 速度修改（修改速度）
4. 分裂/连锁（生成新魔力露珠）

### 2. 坐标系统

所有位置参数使用**世界坐标系**，单位为**厘米（cm）**。

### 3. 效果组合

多个效果可以同时作用于同一个魔力露珠：
- 多个引力奇点的引力会叠加
- 虫洞传送是瞬间的，不会叠加
- 速度修改是累乘的

### 4. 蓝图集成

特殊效果管理器需要与物理系统配合使用：
- 每帧调用`ApplyEffects`应用引力和传送
- 在碰撞事件中调用`ApplyMarbleSplit`或`ApplyChainReaction`
- 将生成的新魔力露珠添加到物理系统

---

## 后续扩展

### 计划中的效果

- **时间停止**: 暂停特定区域的魔力露珠
- **反弹增强**: 增加碰撞后的反弹力度
- **磁力效果**: 吸引或排斥特定类型的魔力露珠
- **黑洞**: 吸收并消灭魔力露珠

### 扩展接口

系统设计为可扩展的，可以轻松添加新的效果类型：
1. 在`ESpecialEffectType`枚举中添加新类型
2. 创建新的参数结构体
3. 在`USpecialEffectsManager`中实现新的效果逻辑
4. 编写单元测试验证效果

---

## 相关任务

- **VOI-24**: 物理系统核心框架
- **VOI-25**: 碰撞检测系统
- **VOI-26**: 混合物理系统
- **VOI-28**: 集成测试

---

**文档维护**: 如有问题或建议，请在Linear任务VOI-27中反馈。
