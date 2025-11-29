// 碰撞检测系统 (Collision Detection System)

## 概述

碰撞检测系统负责管理所有碰撞体并执行高性能的碰撞检测。使用空间网格优化算法，支持圆-圆和圆-矩形碰撞检测。

## 核心类

### 1. UCollisionManager
碰撞管理器，负责管理所有碰撞体并执行碰撞检测。

### 2. FCollisionBody
碰撞体结构体，定义碰撞体的形状、位置等信息。

### 3. FCollisionEvent
碰撞事件结构体，存储碰撞信息（碰撞点、法线、穿透深度等）。

### 4. FSpatialGrid
空间网格数据结构，用于优化碰撞检测性能。

## 蓝图使用指南

### 初始化碰撞管理器

```
// 1. 创建碰撞管理器实例
CollisionManager = NewObject<UCollisionManager>();

// 2. 初始化（设置空间边界和网格尺寸）
CollisionManager->Initialize(
    FVector(-1000, -1000, 0),  // 边界最小点
    FVector(1000, 1000, 1000), // 边界最大点
    100.0f                      // 网格单元尺寸
);
```

### 注册碰撞体

```
// 1. 创建圆形碰撞体
FCollisionBody CircleBody;
CircleBody.Position = GetActorLocation();
CircleBody.ShapeType = ECollisionShapeType::Circle;
CircleBody.Radius = 10.0f;

// 2. 注册到碰撞管理器
FGuid CircleID = CollisionManager->RegisterBody(CircleBody);

// 3. 创建矩形碰撞体
FCollisionBody RectBody;
RectBody.Position = EnemyLocation;
RectBody.ShapeType = ECollisionShapeType::Rectangle;
RectBody.Size = FVector2D(50.0f, 30.0f);
RectBody.Rotation = 0.0f;

// 4. 注册到碰撞管理器
FGuid RectID = CollisionManager->RegisterBody(RectBody);
```

### 更新碰撞体位置

```
// 更新单个碰撞体位置
CollisionManager->UpdateBodyPosition(CircleID, NewPosition);

// 更新后重建空间网格（每帧调用一次）
CollisionManager->UpdateSpatialGrid();
```

### 执行碰撞检测

```
void AYourActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 1. 更新空间网格
    CollisionManager->UpdateSpatialGrid();
    
    // 2. 执行碰撞检测
    TArray<FCollisionEvent> Collisions = CollisionManager->DetectCollisions();
    
    // 3. 处理碰撞
    for (const FCollisionEvent& Event : Collisions)
    {
        HandleCollision(Event);
    }
}
```

### 订阅碰撞事件

```
// 在BeginPlay中绑定事件
void AYourActor::BeginPlay()
{
    Super::BeginPlay();
    
    // 绑定碰撞事件
    CollisionManager->OnCollision.AddDynamic(this, &AYourActor::OnCollisionDetected);
}

// 事件处理函数
void AYourActor::OnCollisionDetected(const FCollisionEvent& Event)
{
    UE_LOG(LogTemp, Log, TEXT("Collision detected: %s <-> %s"),
        *Event.BodyA.ToString(),
        *Event.BodyB.ToString());
    
    // 播放碰撞特效
    SpawnCollisionEffect(Event.HitPoint);
    
    // 处理碰撞逻辑
    ProcessCollision(Event);
}
```

### 检测特定碰撞体的碰撞

```
// 只检测特定碰撞体的碰撞（性能更好）
TArray<FCollisionEvent> Collisions;
int32 CollisionCount = CollisionManager->DetectCollisionsForBody(MarbleID, Collisions);

if (CollisionCount > 0)
{
    // 处理碰撞
    for (const FCollisionEvent& Event : Collisions)
    {
        HandleMarbleCollision(Event);
    }
}
```

## 性能优化

### 空间网格优化

碰撞检测系统使用空间网格优化算法，将空间划分为网格，每个网格存储其中的碰撞体。这样在检测碰撞时只需要检查相邻网格中的碰撞体。

**性能对比**：
- 暴力检测：O(n²) - 100个碰撞体需要10,000次检测
- 空间网格：O(n * k) - 100个碰撞体只需要约1,000次检测（k≈10）

**网格尺寸建议**：
- 炼金工作台：`CellSize = 50.0f`（碰撞体较小）
- 战斗场景：`CellSize = 100.0f`（碰撞体较大）
- 通用规则：设置为碰撞体平均大小的2-3倍

### 性能监控

```
// 获取空间网格统计信息
int32 TotalCells, OccupiedCells, MaxBodiesPerCell;
float AvgBodiesPerCell;

CollisionManager->GetSpatialGridStatistics(
    TotalCells,
    OccupiedCells,
    MaxBodiesPerCell,
    AvgBodiesPerCell
);

UE_LOG(LogTemp, Log, TEXT("Spatial Grid Stats: Total=%d, Occupied=%d, Max=%d, Avg=%.2f"),
    TotalCells, OccupiedCells, MaxBodiesPerCell, AvgBodiesPerCell);
```

**优化建议**：
- 如果 `MaxBodiesPerCell > 20`，考虑减小网格尺寸
- 如果 `AvgBodiesPerCell < 2`，考虑增大网格尺寸
- 如果 `OccupiedCells / TotalCells < 0.1`，考虑减小空间边界

## 碰撞检测算法

### 圆-圆碰撞

检测两个圆是否碰撞，算法简单高效：

```cpp
bool IsColliding = Distance(CircleA.Position, CircleB.Position) < (CircleA.Radius + CircleB.Radius)
```

**精度**：100%准确

**性能**：极快（仅需一次距离计算）

### 圆-矩形碰撞

检测圆与轴对齐矩形（AABB）的碰撞：

1. 找到矩形上距离圆心最近的点
2. 计算该点与圆心的距离
3. 如果距离小于圆的半径，则发生碰撞

**精度**：100%准确（当前版本不支持旋转矩形）

**性能**：快（需要几次比较和一次距离计算）

**限制**：当前版本将矩形视为轴对齐边界盒（AABB），不考虑旋转。如果需要支持旋转矩形，需要在后续版本中实现。

## 与物理系统的集成

碰撞检测系统与物理系统紧密集成：

```
// 1. 物理系统更新魔力露珠位置
PhysicsSystem->Tick(DeltaTime);

// 2. 同步碰撞体位置
TArray<FMarbleState> Marbles = PhysicsSystem->GetAllMarbles();
for (const FMarbleState& Marble : Marbles)
{
    CollisionManager->UpdateBodyPosition(Marble.ID, Marble.Position);
}

// 3. 更新空间网格
CollisionManager->UpdateSpatialGrid();

// 4. 执行碰撞检测
TArray<FCollisionEvent> Collisions = CollisionManager->DetectCollisions();

// 5. 处理碰撞（消耗药效、造成伤害等）
for (const FCollisionEvent& Event : Collisions)
{
    ProcessCollision(Event);
}
```

## 注意事项

1. **初始化顺序**：必须先调用 `Initialize` 才能使用其他功能
2. **空间网格更新**：碰撞体位置更新后必须调用 `UpdateSpatialGrid`
3. **性能考虑**：`UpdateSpatialGrid` 会重建整个空间网格，建议每帧调用一次
4. **碰撞事件**：`DetectCollisions` 会自动触发 `OnCollision` 事件
5. **旋转矩形**：当前版本不支持旋转矩形碰撞检测

## 后续任务

- **VOI-26**: 实现混合物理系统（Actor + Niagara）
- **VOI-27**: 实现GDD特殊物理效果（引力奇点、虫洞等）
- **VOI-28**: 集成测试

## 版本信息

- **版本**: 1.0.0
- **任务**: VOI-25
- **作者**: Manus
- **日期**: 2025-11-29
