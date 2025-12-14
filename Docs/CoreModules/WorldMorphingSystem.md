# 蓝图使用指南：WorldMorphingSystem

**模块：** WorldMorphing
**C++类：** `UWorldMorphingSubsystem`, `UWorldMorphingBlueprintLibrary`

## 1. 功能说明

`WorldMorphingSystem` 是一个基于能量守恒的世界变迁模拟系统，实现了四层交互式演化模型：

- **地幔层**：基于Cahn-Hilliard相分离的地形演化，支持动态扩张和收缩
- **气候层**：热力学扩散模型，自动生成雷暴区域
- **晶石层**：能量驱动的元胞自动机，模拟Alpha/Beta晶石的生长、硬化和消亡
- **人类层**：基于效用函数的智能体行为，包含采矿、扩张和迁移

该系统作为 `GameInstanceSubsystem` 运行，自动管理生命周期，支持实时参数调整。

## 2. 快速开始

### 2.1 初始化世界

在游戏开始时（例如 GameMode 的 BeginPlay），初始化世界网格：

**节点：** `Initialize World`

![Initialize World](https://i.imgur.com/example.png) *（图片占位符）*

**使用方法：**
1. 调用 `Initialize World` 函数
2. 设置 `Width` 和 `Height`（推荐 50-100）
3. 使用 `Make Default Params` 创建默认参数，或自定义参数

**示例蓝图：**
```
Event BeginPlay
  -> Initialize World
     - Width: 80
     - Height: 80
     - Params: Make Default Params
```

### 2.2 每帧更新模拟

在 Tick 事件中调用 `Tick Simulation`：

**节点：** `Tick Simulation`

**使用方法：**
```
Event Tick
  -> Tick Simulation
     - Delta Time: (连接 Event Tick 的 Delta Seconds)
```

### 2.3 读取单元格状态

获取指定坐标的单元格状态，用于渲染或游戏逻辑：

**节点：** `Get Cell At`

**使用方法：**
```
Get Cell At
  - X: 10
  - Y: 20
  -> 返回 Cell State 结构体
     - bExists: 是否存在地形
     - Mantle Energy: 地幔能量
     - Temperature: 温度
     - Crystal Type: 晶石类型（Empty/Alpha/Beta/Human）
     - Stored Energy: 晶石储存能量
     - Prosperity: 人类繁荣度
```

## 3. 核心数据结构

### 3.1 FCellState（单元格状态）

| 字段 | 类型 | 描述 |
| :--- | :--- | :--- |
| **bExists** | `bool` | 地形是否存在 |
| **Mantle Energy** | `float` | 地幔能量（0-150） |
| **Temperature** | `float` | 温度（-50到50） |
| **bHasThunderstorm** | `bool` | 是否有雷暴 |
| **Crystal Type** | `ECrystalType` | 晶石类型（Empty/Alpha/Beta/Human） |
| **Stored Energy** | `float` | 晶石储存能量（0-80） |
| **bIsAbsorbing** | `bool` | 是否正在吸收能量 |
| **Prosperity** | `float` | 人类繁荣度（0-100） |

### 3.2 FSimulationParams（模拟参数）

#### 地幔层参数

| 参数 | 默认值 | 描述 |
| :--- | :--- | :--- |
| **Expansion Threshold** | 123.0 | 地形扩张阈值 |
| **Shrink Threshold** | 2.0 | 地形缩减阈值 |
| **Mantle Energy Level** | 100.0 | 地幔能量等级倍率 |
| **Max Radius** | 25.0 | 最大半径限制 |
| **Min Radius** | 5.0 | 最小半径限制 |
| **Edge Generation Energy** | 4.0 | 边缘能量生成速率 |

#### 气候层参数

| 参数 | 默认值 | 描述 |
| :--- | :--- | :--- |
| **Diffusion Rate** | 0.12 | 温度扩散速率 |
| **Thunderstorm Threshold** | 18.0 | 雷暴触发温差阈值 |
| **Seasonal Amplitude** | 5.0 | 季节性温度波动幅度 |

#### 晶石层参数

| 参数 | 默认值 | 描述 |
| :--- | :--- | :--- |
| **Alpha Energy Demand** | 1.5 | Alpha晶石每帧能量需求 |
| **Mantle Absorption** | 0.1 | 地幔能量吸收效率 |
| **Thunderstorm Energy** | 10.0 | 雷暴提供的能量 |
| **Expansion Cost** | 8.0 | 扩张所需能量 |
| **Max Crystal Energy** | 80.0 | 晶石能量上限 |
| **Energy Sharing Rate** | 0.1 | 能量共享速率 |

#### 人类层参数

| 参数 | 默认值 | 描述 |
| :--- | :--- | :--- |
| **Human Min Temp** | -10.0 | 适宜温度下限 |
| **Human Max Temp** | 25.0 | 适宜温度上限 |
| **Human Prosperity Growth** | 0.5 | 适宜温度下的繁荣度增长 |
| **Human Expansion Threshold** | 80.0 | 扩张所需繁荣度阈值 |
| **Human Mining Reward** | 20.0 | 消除Beta晶石获得的繁荣度 |

## 4. 高级用法

### 4.1 动态调整参数

在运行时修改模拟参数，实现难度调整或特殊事件：

**节点：** `Set Simulation Params`

**使用方法：**
```
Get Simulation Params
  -> Break Simulation Params
  -> 修改某些字段（例如 Thunderstorm Threshold）
  -> Make Simulation Params
  -> Set Simulation Params
```

### 4.2 遍历整个网格

批量读取所有单元格状态，用于渲染或数据分析：

**示例蓝图：**
```
Get Grid Size
  -> For Loop (0 to Width-1)
     -> For Loop (0 to Height-1)
        -> Get Cell At (X, Y)
        -> 处理单元格状态
```

### 4.3 监控系统状态

获取模拟的时间步和周期数，用于UI显示或触发事件：

**节点：** `Get Time Step`, `Get Cycle Count`

**使用方法：**
```
Get Cycle Count
  -> Branch (Cycle Count > 10)
     -> 触发特殊事件
```

## 5. 性能优化建议

### 5.1 网格尺寸

- **小型地图（测试）**：50x50
- **中型地图（标准）**：80x80
- **大型地图（高端）**：100x100

### 5.2 更新频率

如果性能不足，可以降低更新频率：

```
Event Tick
  -> 每N帧调用一次 Tick Simulation
     （使用计数器或定时器）
```

### 5.3 分块渲染

只渲染玩家视野范围内的单元格，避免全图遍历。

## 6. 常见问题

### Q1: 世界为什么不演化？

**A:** 确保在 Tick 中调用了 `Tick Simulation`，并且已经调用过 `Initialize World`。

### Q2: 如何让晶石生长更快？

**A:** 增加 `Thunderstorm Energy` 和 `Mantle Absorption`，降低 `Alpha Energy Demand`。

### Q3: 人类为什么会消失？

**A:** 检查温度是否在生存范围内（`Human Survival Min/Max Temp`），以及繁荣度是否过低。

### Q4: 如何重置世界？

**A:** 再次调用 `Initialize World` 即可重新初始化。

## 7. 相关技术文档

- [世界变迁系统 TDD](../../ue5-human-ai-collab/docs/tech/world_morphing_system_tdd.md)
- [交互式模拟器](../../ue5-human-ai-collab/docs/tech/simulator/)
- [MHP文档](../../ue5-human-ai-collab/docs/mhp/VOI-6_world_morphing.md)

## 8. 示例项目

完整的示例蓝图可以在以下位置找到：

- `Content/Blueprints/Systems/WorldMorphing/BP_WorldMorphingDemo`
- `Content/Maps/TestMaps/WorldMorphingTest`

## 9. 注意事项

- 该系统是 **GameInstanceSubsystem**，在整个游戏实例中只有一个实例
- 所有蓝图函数都需要 **World Context Object**（通常使用 `Self`）
- 模拟是 **确定性的**（相同参数和初始状态会产生相同结果）
- 系统支持 **实时参数调整**，但频繁修改可能影响性能

## 10. 未来扩展

计划中的功能：

- [ ] 多线程优化
- [ ] 网格分块LOD
- [ ] 保存/加载世界状态
- [ ] 更多晶石类型
- [ ] 玩家交互接口（手动放置晶石、改变温度等）
