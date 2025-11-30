# WorldMorphingSystem API 迁移指南

## 概述

WorldMorphingSystem已经进行了重大的API重构，将原来分散的9个函数重新组织为3个功能模块。本文档将帮助您从旧API迁移到新API。

## 为什么要迁移？

### 旧API的问题

- **函数过多**: 9个独立函数，蓝图节点图复杂
- **职责不清**: 混合了模拟、查询、配置功能
- **难以扩展**: 添加新功能需要修改多个地方

### 新API的优势

- **模块化**: 清晰的职责分离
- **简洁**: 减少50%的蓝图节点数量
- **易用**: 更符合直觉的命名
- **可扩展**: 易于添加新功能
- **向后兼容**: 旧API仍然可用（但标记为Deprecated）

## 新API结构

新API分为三个模块：

### 1. Simulation（模拟功能）
负责世界的初始化、更新、重置等核心模拟逻辑。

### 2. Visualization（视觉呈现）
负责获取世界状态数据，用于渲染和可视化。

### 3. Configuration（参数配置）
负责参数的创建、获取、设置。

---

## 迁移映射表

### 模拟功能

| 旧API | 新API | 说明 |
| :--- | :--- | :--- |
| `InitializeWorld()` | `Simulation::Initialize()` | 初始化世界 |
| `TickSimulation()` | `Simulation::Tick()` | 更新模拟 |
| - | `Simulation::Reset()` | 重置世界（新增） |
| - | `Simulation::SetPaused()` | 暂停/继续（新增） |
| `GetTimeStep()` + `GetCycleCount()` + `GetGridSize()` | `Simulation::GetStatus()` | 获取状态（合并） |

### 视觉呈现

| 旧API | 新API | 说明 |
| :--- | :--- | :--- |
| `GetCellAt()` | `Visualization::GetCellState()` | 获取单个单元格 |
| - | `Visualization::GetRegionStates()` | 批量获取区域（新增） |
| - | `Visualization::GetStatistics()` | 获取统计信息（新增） |
| - | `Visualization::GetHeatmapData()` | 获取热力图数据（新增） |

### 参数配置

| 旧API | 新API | 说明 |
| :--- | :--- | :--- |
| `MakeDefaultParams()` | `Configuration::MakeDefault()` | 创建默认参数 |
| - | `Configuration::MakeCustom()` | 创建自定义参数（新增） |
| `GetSimulationParams()` | `Configuration::GetCurrent()` | 获取当前参数 |
| `SetSimulationParams()` | `Configuration::Apply()` | 应用参数 |
| - | `Configuration::ResetToDefault()` | 重置为默认（新增） |
| - | `Configuration::MakePreset()` | 创建预设参数（新增） |

---

## 蓝图迁移示例

### 示例1: 初始化世界

#### 旧方式
```
Event BeginPlay
  → Make Default Params
    → Initialize World
      - Width: 50
      - Height: 50
      - Params: (从上一个节点)
```

#### 新方式
```
Event BeginPlay
  → Simulation.Initialize
    - Width: 50
    - Height: 50
    - Params: (使用默认值)
```

**优势**: 减少1个节点，更简洁。

---

### 示例2: 获取世界信息

#### 旧方式
```
Custom Function: GetWorldInfo
  → Get Grid Size
    - OutWidth
    - OutHeight
  → Get Time Step
    - TimeStep
  → Get Cycle Count
    - CycleCount
  → Return (Width, Height, TimeStep, CycleCount)
```

#### 新方式
```
Custom Function: GetWorldInfo
  → Simulation.GetStatus
    → Return (Status)
```

**优势**: 从4个节点减少到1个节点，返回统一的结构体。

---

### 示例3: 读取单元格并渲染

#### 旧方式
```
Event Tick
  → For Loop (0 to Width*Height)
    → Get Cell At (X, Y)
      → Branch (bExists)
        → True: Spawn Visual (根据 Crystal Type)
```

#### 新方式（单个）
```
Event Tick
  → For Loop (0 to Width*Height)
    → Visualization.GetCellState (X, Y)
      → Branch (bExists)
        → True: Spawn Visual (根据 Crystal Type)
```

#### 新方式（批量，推荐）
```
Event Tick
  → Visualization.GetRegionStates (0, 0, Width, Height)
    → For Each (Cell in Region)
      → Branch (bExists)
        → True: Spawn Visual (根据 Crystal Type)
```

**优势**: 批量获取性能更好。

---

### 示例4: 参数调整

#### 旧方式
```
Custom Function: AdjustParams
  → Get Simulation Params
    → Set Members (修改参数)
      → Set Simulation Params
```

#### 新方式（方法1：直接修改）
```
Custom Function: AdjustParams
  → Configuration.GetCurrent
    → Set Members (修改参数)
      → Configuration.Apply
```

#### 新方式（方法2：使用预设）
```
Custom Function: AdjustParams
  → Configuration.MakePreset (Fast Growth)
    → Configuration.Apply
```

**优势**: 新增预设功能，常用配置一键应用。

---

## Python脚本迁移示例

### 旧方式
```python
# 初始化
params = unreal.WorldMorphingBlueprintLibrary.make_default_params()
unreal.WorldMorphingBlueprintLibrary.initialize_world(
    game_instance, 50, 50, params
)

# 获取信息
width, height = 0, 0
unreal.WorldMorphingBlueprintLibrary.get_grid_size(
    game_instance, width, height
)
time_step = unreal.WorldMorphingBlueprintLibrary.get_time_step(game_instance)
cycle_count = unreal.WorldMorphingBlueprintLibrary.get_cycle_count(game_instance)
```

### 新方式
```python
# 初始化
params = unreal.WorldMorphingConfiguration.make_default()
unreal.WorldMorphingSimulation.initialize(game_instance, 50, 50, params)

# 获取信息（一次调用）
status = unreal.WorldMorphingSimulation.get_status(game_instance)
width = status.width
height = status.height
time_step = status.time_step
cycle_count = status.cycle_count
```

**优势**: 减少API调用次数，代码更简洁。

---

## 新功能介绍

### 1. 统计信息（Visualization::GetStatistics）

获取世界的统计数据，无需手动遍历：

```
→ Visualization.GetStatistics
  → Print String (Total Cells: {stats.total_cells})
  → Print String (Alpha Crystals: {stats.alpha_crystals})
  → Print String (Average Temperature: {stats.average_temperature})
```

### 2. 批量获取区域（Visualization::GetRegionStates）

一次性获取整个区域的单元格状态：

```
→ Visualization.GetRegionStates (StartX, StartY, Width, Height)
  → For Each (Cell)
    → Process Cell
```

### 3. 热力图数据（Visualization::GetHeatmapData）

获取归一化的热力图数据，用于调试可视化：

```
→ Visualization.GetHeatmapData (Mantle Energy)
  → For Each (Value)
    → Set Material Color (Lerp from Blue to Red based on Value)
```

### 4. 参数预设（Configuration::MakePreset）

快速应用预定义的参数配置：

```
→ Configuration.MakePreset (Fast Growth)
  → Configuration.Apply
```

可用预设：
- **Default**: 默认平衡参数
- **Fast Growth**: 快速生长（低扩张阈值）
- **Slow Evolution**: 缓慢演化（高扩张阈值）
- **High Energy**: 高能量（强雷暴）
- **Stable**: 稳定（平衡的参数）

---

## 迁移步骤

### 步骤1: 了解新API

阅读本文档和新的API文档：
- `WorldMorphingSystem_API_Reference.md`

### 步骤2: 更新蓝图

1. 打开使用WorldMorphingSystem的蓝图
2. 找到所有使用旧API的节点
3. 根据迁移映射表替换为新API
4. 利用新功能简化逻辑

### 步骤3: 测试

1. 编译C++代码
2. 重新打开蓝图
3. 运行测试，确保功能正常

### 步骤4: 清理（可选）

旧API仍然可用，但建议尽快迁移：
- 旧API会显示"Deprecated"警告
- 未来版本可能移除旧API

---

## 常见问题

### Q: 旧API还能用吗？

**A**: 可以！旧API已经标记为Deprecated，但内部调用新API，功能完全正常。建议尽快迁移以享受新API的优势。

### Q: 新API会破坏现有蓝图吗？

**A**: 不会！这是向后兼容的重构。现有蓝图可以继续使用旧API，不会有任何问题。

### Q: 我应该立即迁移吗？

**A**: 建议尽快迁移，但不是强制的。新API更简洁、更强大，迁移后维护更容易。

### Q: 迁移会很复杂吗？

**A**: 不会！大多数情况下只需要替换节点名称。参考本文档的示例即可。

### Q: 新API性能更好吗？

**A**: 是的！新API减少了不必要的调用，批量操作性能更优。

---

## 获取帮助

如果在迁移过程中遇到问题：

1. 查看 `WorldMorphingSystem_API_Reference.md`
2. 运行测试脚本 `test_world_morphing_new_api.py`
3. 查看示例蓝图（即将添加）

---

## 总结

新API的核心理念：

- **Simulation**: 控制模拟的生命周期
- **Visualization**: 获取数据用于渲染
- **Configuration**: 管理参数配置

记住这三个模块，迁移就会很简单！

---

**版本**: 1.0  
**更新日期**: 2025-11-29  
**相关任务**: VOI-6
