# WorldMorphingSystem 实现总结

## 概述

本文档记录了世界变迁系统从Web模拟器到UE4 C++的完整移植过程。

## 实现架构

### 1. 核心模块结构

```
Source/EchoAlchemist/
├── Public/WorldMorphing/
│   ├── WorldMorphingTypes.h              # 核心数据类型定义
│   ├── WorldMorphingSubsystem.h          # 主系统子系统
│   ├── WorldMorphingBlueprintLibrary.h   # 蓝图函数库
│   └── PerlinNoise.h                     # Perlin噪声生成器
└── Private/WorldMorphing/
    ├── WorldMorphingSubsystem.cpp        # 主系统实现（包含四层更新逻辑）
    ├── WorldMorphingBlueprintLibrary.cpp # 蓝图函数库实现
    └── PerlinNoise.cpp                   # Perlin噪声实现
```

### 2. 核心类说明

#### 2.1 UWorldMorphingSubsystem

**类型：** GameInstanceSubsystem

**职责：**
- 管理整个世界网格的生命周期
- 执行四层模拟更新（地幔、气候、晶石、人类）
- 提供蓝图可调用的API接口

**关键方法：**
- `InitializeWorld()` - 初始化网格
- `TickSimulation()` - 每帧更新
- `UpdateMantleLayer()` - 地幔层更新
- `UpdateClimateLayer()` - 气候层更新
- `UpdateCrystalLayer()` - 晶石层更新
- `UpdateHumanLayer()` - 人类层更新

#### 2.2 FCell（内部数据结构）

**存储内容：**
- 地幔层：能量、扩张/收缩累加器
- 气候层：温度、基础温度、温度变化、雷暴标记
- 晶石层：晶石类型、储存能量、吸收状态、能量流
- 人类层：繁荣度

#### 2.3 FCellState（蓝图可见结构）

**用途：** 将内部FCell数据转换为蓝图可读的状态

#### 2.4 FSimulationParams（参数结构）

**包含：** 所有四层的可调参数（共30+个参数）

#### 2.5 UWorldMorphingBlueprintLibrary

**类型：** BlueprintFunctionLibrary

**职责：** 提供便捷的静态蓝图函数，自动获取Subsystem实例

## 算法实现细节

### 1. 地幔层（Mantle Layer）

#### 1.1 Cahn-Hilliard相分离

**原理：** 通过能量扩散实现相分离，使地形自然聚团并填补空缺

**实现：**
```cpp
// 计算邻居能量差
float Diff = Neighbor->MantleEnergy - Cell.MantleEnergy;

// 基础扩散
float Flow = Diff * DiffusionCoeff * 0.1f;

// 迁徙偏置（缓慢流动）
float BiasX = FMath::Sin(Time * 0.5f);
float BiasY = FMath::Cos(Time * 0.5f);
float BiasFlow = (DX * BiasX + DY * BiasY) * 0.05f;

Flow += BiasFlow;
```

**能量守恒：** 所有能量流动都是双向的，总能量保持不变

#### 1.2 边缘能量生成

**机制：** 使用多个随机迁移的供给点，在边缘区域注入能量

**实现：**
- 维护N个供给点（角度+速度）
- 每帧更新供给点位置
- 使用BFS计算每个格子到边缘的距离
- 在指定距离范围内，根据供给点密度注入能量

#### 1.3 边缘扩张/收缩

**扩张条件：** `MantleEnergy > ExpansionThreshold`
**收缩条件：** `MantleEnergy < ShrinkThreshold`

**能量守恒：**
- 扩张时：能量平分给新格子
- 收缩时：能量回流给邻居

### 2. 气候层（Climate Layer）

#### 2.1 温度计算

**基础温度：** 来自地幔能量
```cpp
float NormalizedEnergy = FMath::Min(100.0f, Cell.MantleEnergy);
Cell.BaseTemperature = (NormalizedEnergy - 50.0f) * 0.5f + SeasonalOffset;
```

**季节性变化：**
```cpp
float TimeCycle = (TimeStep % 1000) / 1000.0f;
float SeasonalOffset = SeasonalAmplitude * FMath::Sin(2.0f * PI * TimeCycle);
```

#### 2.2 热扩散

**实现：** 简单的拉普拉斯扩散
```cpp
float AvgTemp = 邻居平均温度;
Cell.TemperatureChange = DiffusionRate * (AvgTemp - Cell.Temperature);
```

#### 2.3 雷暴生成

**条件：** 与邻居的最大温差 > ThunderstormThreshold
```cpp
Cell.bHasThunderstorm = MaxDiff > ThunderstormThreshold;
```

### 3. 晶石层（Crystal Layer）

#### 3.1 能量获取

**来源：**
1. 地幔吸收：`Absorbed = MantleEnergy * MantleAbsorption`
2. 雷暴能量：`ThunderstormEnergy`（如果有雷暴）

**消耗：**
- Alpha晶石：每帧消耗 `AlphaEnergyDemand`

**结算：**
```cpp
float NetEnergy = EnergyInput - AlphaEnergyDemand;
Cell.StoredEnergy += NetEnergy;
```

#### 3.2 能量共享

**机制：** 连通的Alpha晶石可以共享能量

**实现：**
```cpp
float Diff = Neighbor->StoredEnergy - Cell.StoredEnergy;
if (Diff > 0.0f)
{
    float Flow = Diff * EnergySharingRate;
    // 检查接收方是否超过共享上限
    if (Cell.StoredEnergy + Flow <= MaxCrystalEnergy * EnergySharingLimit)
    {
        // 应用能量流动
    }
}
```

#### 3.3 状态转移

**规则：**
1. **Empty → Alpha**：邻居有富裕能量（>= ExpansionCost）且概率触发
2. **Alpha → Beta**：能量耗尽（StoredEnergy <= 0）
3. **Alpha → Empty**：孤立死亡（无Alpha邻居且能量低）
4. **Beta → ?**：不可逆，永久保持

**实现：** 使用双缓冲（NextStates数组）避免顺序依赖

### 4. 人类层（Human Layer）

#### 4.1 初始化

**条件：** 当前无人类存在时，随机生成一个聚落

**优先选择：** 温度适宜的位置

#### 4.2 繁荣度更新

**增长：**
- 温度适宜：`+ HumanProsperityGrowth`
- 人类邻居加成：`+ 邻居数 * 0.1`
- 采矿奖励：`+ HumanMiningReward`（消除Beta晶石）

**衰减：**
- 温度不适宜：`- HumanProsperityDecay`

#### 4.3 行为逻辑

**采矿：** 随机选择一个相邻的Beta晶石消除

**扩张：** 繁荣度 > ExpansionThreshold 时，扩张到空地或Beta格

**迁移：** 繁荣度 < MigrationThreshold 时，寻找更好的位置（温度适宜或有Beta邻居）

**死亡：** 温度超出生存范围（SurvivalMinTemp ~ SurvivalMaxTemp）

#### 4.4 变更应用

**实现：** 使用变更队列（Changes数组）避免顺序依赖

## 性能优化

### 1. 内存布局

- 使用 `TArray<TArray<FCell>>` 二维数组，内存连续
- FCell结构紧凑，避免不必要的指针

### 2. 算法优化

- BFS距离计算：提前终止（MaxDist限制）
- 能量共享：只计算Alpha晶石
- 邻居查询：缓存结果

### 3. 未来优化方向

- 多线程：使用UE4的Task Graph并行更新各层
- 分块更新：只更新活跃区域
- LOD：远离玩家的区域降低更新频率

## 与Web模拟器的差异

### 1. 语言差异

| Web (TypeScript) | UE4 (C++) |
| :--- | :--- |
| `Array<Array<Cell>>` | `TArray<TArray<FCell>>` |
| `Math.random()` | `FMath::FRand()` |
| `Math.sqrt()` | `FMath::Sqrt()` |
| `Math.sin()` | `FMath::Sin()` |

### 2. 架构差异

| Web | UE4 |
| :--- | :--- |
| 独立类 `SimulationEngine` | `GameInstanceSubsystem` |
| 直接调用 | 通过蓝图函数库调用 |
| 无生命周期管理 | 自动管理生命周期 |

### 3. 功能完整性

✅ **已实现：**
- 地幔层：Cahn-Hilliard相分离、边缘能量生成、扩张/收缩
- 气候层：热扩散、季节性变化、雷暴映射
- 晶石层：能量系统、状态转移、能量共享
- 人类层：繁荣度、采矿、扩张、迁移

⚠️ **简化部分：**
- Perlin噪声：未在地幔层中使用（预留接口）
- 可视化：能量流记录（EnergyFlow）已实现但未渲染

## 测试建议

### 1. 单元测试

**地幔层：**
- 能量守恒：总能量不变
- 扩张/收缩：边界正确移动

**气候层：**
- 温度扩散：逐渐趋于均匀
- 雷暴生成：温差大的地方出现

**晶石层：**
- 状态转移：按规则正确转换
- 能量共享：能量流向正确

**人类层：**
- 初始化：成功生成聚落
- 行为：采矿、扩张、迁移正确触发

### 2. 集成测试

**完整演化：**
- 初始化 80x80 网格
- 运行 10000 步
- 观察四层的交互行为

**参数调整：**
- 修改参数后，行为符合预期

### 3. 性能测试

**目标：**
- 80x80 网格：< 5ms/帧
- 100x100 网格：< 10ms/帧

## 已知问题

1. **编译依赖：** 需要确保 `EchoAlchemist.Build.cs` 包含必要的模块依赖
2. **头文件包含：** 可能需要添加 `#include "Engine/World.h"` 等
3. **蓝图暴露：** 确保所有UFUNCTION宏正确标记

## 后续工作

1. **编译测试：** 在UE4编辑器中编译验证
2. **蓝图示例：** 创建演示蓝图
3. **可视化：** 实现网格渲染
4. **性能分析：** 使用UE4 Profiler分析瓶颈
5. **文档完善：** 添加更多使用示例和截图

## 相关文档

- [使用指南](WorldMorphingSystem.md)
- [技术设计文档](../../ue4-human-ai-collab/docs/tech/world_morphing_system_tdd.md)
- [MHP文档](../../ue4-human-ai-collab/docs/mhp/VOI-6_world_morphing.md)
- [交接文档](../../ue4-human-ai-collab/docs/handover/VOI-6_handover.md)
