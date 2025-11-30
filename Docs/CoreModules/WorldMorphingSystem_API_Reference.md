# WorldMorphingSystem API 参考文档

## 概述

WorldMorphingSystem提供了三个模块化的蓝图函数库，用于控制世界变迁系统。

---

## 模块1: Simulation (模拟功能)

**类名**: `UWorldMorphingSimulation`  
**职责**: 管理世界的初始化、更新、重置等核心模拟逻辑

### Initialize

**签名**:
```cpp
static bool Initialize(UObject* WorldContextObject, 
                       int32 Width, 
                       int32 Height, 
                       const FSimulationParams& Params)
```

**描述**: 初始化世界网格

**参数**:
- `WorldContextObject`: 世界上下文对象
- `Width`: 网格宽度（建议范围: 30-100）
- `Height`: 网格高度（建议范围: 30-100）
- `Params`: 模拟参数（可使用Configuration模块创建）

**返回值**: 是否成功初始化

**示例**:
```
Event BeginPlay
  → Configuration.MakeDefault
    → Simulation.Initialize (Width: 50, Height: 50, Params: ...)
```

---

### Tick

**签名**:
```cpp
static void Tick(UObject* WorldContextObject, float DeltaTime)
```

**描述**: 更新模拟一帧

**参数**:
- `WorldContextObject`: 世界上下文对象
- `DeltaTime`: 时间增量（秒）

**示例**:
```
Event Tick
  → Simulation.Tick (DeltaTime)
```

---

### Reset

**签名**:
```cpp
static void Reset(UObject* WorldContextObject)
```

**描述**: 重置世界到初始状态（保持相同的尺寸和参数）

**参数**:
- `WorldContextObject`: 世界上下文对象

**示例**:
```
On Button Clicked (Reset)
  → Simulation.Reset
```

---

### SetPaused

**签名**:
```cpp
static void SetPaused(UObject* WorldContextObject, bool bPause)
```

**描述**: 暂停或继续模拟

**参数**:
- `WorldContextObject`: 世界上下文对象
- `bPause`: true=暂停，false=继续

**注意**: 当前版本尚未完全实现，预留接口

**示例**:
```
On Button Clicked (Pause)
  → Simulation.SetPaused (true)
```

---

### GetStatus

**签名**:
```cpp
static FSimulationStatus GetStatus(UObject* WorldContextObject)
```

**描述**: 获取模拟的整体状态信息

**参数**:
- `WorldContextObject`: 世界上下文对象

**返回值**: `FSimulationStatus` 结构体，包含：
- `bInitialized`: 是否已初始化
- `bPaused`: 是否暂停
- `Width`: 网格宽度
- `Height`: 网格高度
- `TimeStep`: 当前时间步
- `CycleCount`: 周期计数

**示例**:
```
Event Tick
  → Simulation.GetStatus
    → Break Simulation Status
      → Print String ("Time Step: {TimeStep}")
```

---

## 模块2: Visualization (视觉呈现)

**类名**: `UWorldMorphingVisualization`  
**职责**: 获取世界状态数据，用于渲染和可视化

### GetCellState

**签名**:
```cpp
static FCellState GetCellState(UObject* WorldContextObject, int32 X, int32 Y)
```

**描述**: 获取单个单元格的状态

**参数**:
- `WorldContextObject`: 世界上下文对象
- `X`: X坐标（0-based）
- `Y`: Y坐标（0-based）

**返回值**: `FCellState` 结构体，包含：
- `bExists`: 是否存在地形
- `MantleEnergy`: 地幔能量
- `Temperature`: 温度
- `bHasThunderstorm`: 是否有雷暴
- `CrystalType`: 晶石类型（Empty/Alpha/Beta/Human）
- `StoredEnergy`: 储存能量
- `bIsAbsorbing`: 是否正在吸收能量
- `Prosperity`: 繁荣度

**示例**:
```
Event Tick
  → Visualization.GetCellState (25, 25)
    → Break Cell State
      → Branch (bExists)
        → True: Spawn Visual
```

---

### GetRegionStates

**签名**:
```cpp
static TArray<FCellState> GetRegionStates(UObject* WorldContextObject, 
                                           int32 StartX, int32 StartY, 
                                           int32 Width, int32 Height)
```

**描述**: 批量获取区域内所有单元格的状态

**参数**:
- `WorldContextObject`: 世界上下文对象
- `StartX`: 起始X坐标
- `StartY`: 起始Y坐标
- `Width`: 区域宽度
- `Height`: 区域高度

**返回值**: `FCellState` 数组，按行优先顺序排列

**性能提示**: 批量获取比逐个获取性能更好

**示例**:
```
Event Tick
  → Visualization.GetRegionStates (0, 0, 50, 50)
    → For Each Loop
      → Process Cell
```

---

### GetStatistics

**签名**:
```cpp
static FWorldStatistics GetStatistics(UObject* WorldContextObject)
```

**描述**: 获取整个世界的统计信息

**参数**:
- `WorldContextObject`: 世界上下文对象

**返回值**: `FWorldStatistics` 结构体，包含：
- `TotalCells`: 总单元格数
- `TerrainCells`: 地形单元格数
- `AlphaCrystals`: Alpha晶石数量
- `BetaCrystals`: Beta晶石数量
- `HumanSettlements`: 人类聚落数量
- `ThunderstormCells`: 雷暴区域数量
- `AverageMantleEnergy`: 平均地幔能量
- `AverageTemperature`: 平均温度

**示例**:
```
On Button Clicked (Show Stats)
  → Visualization.GetStatistics
    → Break World Statistics
      → Print String ("Alpha Crystals: {AlphaCrystals}")
```

---

### GetHeatmapData

**签名**:
```cpp
static TArray<float> GetHeatmapData(UObject* WorldContextObject, 
                                     EHeatmapDataType DataType)
```

**描述**: 获取热力图数据（归一化到0-1）

**参数**:
- `WorldContextObject`: 世界上下文对象
- `DataType`: 数据类型
  - `MantleEnergy`: 地幔能量
  - `Temperature`: 温度
  - `CrystalDensity`: 晶石密度
  - `HumanDensity`: 人类密度

**返回值**: float数组，每个值在0-1之间，按行优先顺序排列

**用途**: 用于调试可视化、生成热力图材质等

**示例**:
```
Event Tick
  → Visualization.GetHeatmapData (Mantle Energy)
    → For Each Loop (with Index)
      → Lerp Color (Blue to Red, Alpha: Value)
        → Set Pixel Color (Index)
```

---

## 模块3: Configuration (参数配置)

**类名**: `UWorldMorphingConfiguration`  
**职责**: 管理模拟参数的创建、获取、设置

### MakeDefault

**签名**:
```cpp
static FSimulationParams MakeDefault()
```

**描述**: 创建默认模拟参数

**返回值**: `FSimulationParams` 结构体，包含所有默认参数

**示例**:
```
Event BeginPlay
  → Configuration.MakeDefault
    → Simulation.Initialize (Width, Height, Params)
```

---

### MakeCustom

**签名**:
```cpp
static FSimulationParams MakeCustom(float ExpansionThreshold = 123.0f,
                                     float ThunderstormThreshold = 18.0f,
                                     float AlphaEnergyDemand = 1.5f)
```

**描述**: 创建自定义参数（只修改关键参数，其他使用默认值）

**参数**:
- `ExpansionThreshold`: 扩张阈值（默认: 123.0）
- `ThunderstormThreshold`: 雷暴阈值（默认: 18.0）
- `AlphaEnergyDemand`: Alpha能量需求（默认: 1.5）

**返回值**: `FSimulationParams` 结构体

**示例**:
```
Event BeginPlay
  → Configuration.MakeCustom (150.0, 15.0, 1.0)
    → Simulation.Initialize (Width, Height, Params)
```

---

### GetCurrent

**签名**:
```cpp
static FSimulationParams GetCurrent(UObject* WorldContextObject)
```

**描述**: 获取当前使用的模拟参数

**参数**:
- `WorldContextObject`: 世界上下文对象

**返回值**: `FSimulationParams` 结构体

**示例**:
```
On Button Clicked (Show Params)
  → Configuration.GetCurrent
    → Break Simulation Params
      → Print String ("Expansion Threshold: {ExpansionThreshold}")
```

---

### Apply

**签名**:
```cpp
static void Apply(UObject* WorldContextObject, const FSimulationParams& Params)
```

**描述**: 应用新的模拟参数

**参数**:
- `WorldContextObject`: 世界上下文对象
- `Params`: 新参数

**注意**: 参数会立即生效，影响后续的模拟更新

**示例**:
```
On Slider Value Changed
  → Configuration.GetCurrent
    → Set Expansion Threshold (New Value)
      → Configuration.Apply
```

---

### ResetToDefault

**签名**:
```cpp
static void ResetToDefault(UObject* WorldContextObject)
```

**描述**: 重置参数为默认值

**参数**:
- `WorldContextObject`: 世界上下文对象

**示例**:
```
On Button Clicked (Reset Params)
  → Configuration.ResetToDefault
```

---

### MakePreset

**签名**:
```cpp
static FSimulationParams MakePreset(ESimulationPreset Preset)
```

**描述**: 创建预设参数

**参数**:
- `Preset`: 预设类型
  - `Default`: 默认平衡参数
  - `FastGrowth`: 快速生长（低扩张阈值，高能量）
  - `SlowEvolution`: 缓慢演化（高扩张阈值，低能量）
  - `HighEnergy`: 高能量（强雷暴，高地幔能量）
  - `Stable`: 稳定（平衡的参数）

**返回值**: `FSimulationParams` 结构体

**示例**:
```
On Dropdown Selection Changed
  → Configuration.MakePreset (Selected Preset)
    → Configuration.Apply
```

---

## 数据结构参考

### FSimulationStatus

```cpp
struct FSimulationStatus
{
    bool bInitialized;    // 是否已初始化
    bool bPaused;         // 是否暂停
    int32 Width;          // 网格宽度
    int32 Height;         // 网格高度
    int32 TimeStep;       // 当前时间步
    int32 CycleCount;     // 周期计数
};
```

### FWorldStatistics

```cpp
struct FWorldStatistics
{
    int32 TotalCells;              // 总单元格数
    int32 TerrainCells;            // 地形单元格数
    int32 AlphaCrystals;           // Alpha晶石数量
    int32 BetaCrystals;            // Beta晶石数量
    int32 HumanSettlements;        // 人类聚落数量
    int32 ThunderstormCells;       // 雷暴区域数量
    float AverageMantleEnergy;     // 平均地幔能量
    float AverageTemperature;      // 平均温度
};
```

### FCellState

```cpp
struct FCellState
{
    bool bExists;              // 是否存在地形
    float MantleEnergy;        // 地幔能量
    float Temperature;         // 温度
    bool bHasThunderstorm;     // 是否有雷暴
    ECrystalType CrystalType;  // 晶石类型
    float StoredEnergy;        // 储存能量
    bool bIsAbsorbing;         // 是否正在吸收能量
    float Prosperity;          // 繁荣度
};
```

### FSimulationParams

包含30+个参数，分为4个类别：

#### 地幔层参数
- `MantleTimeScale`: 地幔时间缩放
- `ExpansionThreshold`: 扩张阈值
- `ShrinkThreshold`: 收缩阈值
- `MantleEnergyLevel`: 地幔能量水平
- `MaxRadius`, `MinRadius`: 噪声半径范围
- `DistortionSpeed`: 扭曲速度
- `EdgeGenerationWidth`: 边缘生成宽度
- `EdgeGenerationEnergy`: 边缘生成能量
- 等等...

#### 气候层参数
- `DiffusionRate`: 扩散速率
- `AdvectionRate`: 对流速率
- `ThunderstormThreshold`: 雷暴阈值
- `SeasonalAmplitude`: 季节振幅

#### 晶石层参数
- `AlphaEnergyDemand`: Alpha能量需求
- `BetaEnergyDemand`: Beta能量需求
- `MantleAbsorption`: 地幔吸收率
- `ThunderstormEnergy`: 雷暴能量
- `ExpansionCost`: 扩张成本
- `MaxCrystalEnergy`: 最大晶石能量
- 等等...

#### 人类层参数
- `HumanMinTemp`, `HumanMaxTemp`: 适宜温度范围
- `HumanSurvivalMinTemp`, `HumanSurvivalMaxTemp`: 生存温度范围
- `HumanProsperityGrowth`: 繁荣增长率
- `HumanExpansionThreshold`: 扩张阈值
- 等等...

完整参数列表请参考 `WorldMorphingTypes.h`。

---

## 枚举参考

### ECrystalType

```cpp
enum class ECrystalType
{
    Empty,    // 空白
    Alpha,    // Alpha晶石（活体）
    Beta,     // Beta晶石（硬化）
    Human     // 人类聚落
};
```

### EHeatmapDataType

```cpp
enum class EHeatmapDataType
{
    MantleEnergy,     // 地幔能量
    Temperature,      // 温度
    CrystalDensity,   // 晶石密度
    HumanDensity      // 人类密度
};
```

### ESimulationPreset

```cpp
enum class ESimulationPreset
{
    Default,          // 默认
    FastGrowth,       // 快速生长
    SlowEvolution,    // 缓慢演化
    HighEnergy,       // 高能量
    Stable            // 稳定
};
```

---

## 最佳实践

### 1. 初始化

```
Event BeginPlay
  → Configuration.MakeDefault (或 MakePreset)
    → Simulation.Initialize
```

### 2. 更新

```
Event Tick
  → Simulation.Tick (Delta Seconds)
```

### 3. 渲染

```
Event Tick
  → Visualization.GetRegionStates (批量获取)
    → For Each Loop
      → Update Visual
```

### 4. 调试

```
Event Tick (每秒一次)
  → Visualization.GetStatistics
    → Print Stats to Screen
```

### 5. 参数调整

```
On UI Slider Changed
  → Configuration.GetCurrent
    → Modify Parameter
      → Configuration.Apply
```

---

## 性能建议

1. **批量操作**: 使用 `GetRegionStates` 而不是多次 `GetCellState`
2. **统计信息**: 使用 `GetStatistics` 而不是手动遍历
3. **合理网格**: 建议网格大小在 30x30 到 80x80 之间
4. **更新频率**: 可以降低更新频率（例如每2帧更新一次）

---

## 相关文档

- [API迁移指南](WorldMorphingSystem_API_Migration.md)
- [使用指南](WorldMorphingSystem.md)
- [实现总结](WorldMorphingSystem_Implementation.md)

---

**版本**: 1.0  
**更新日期**: 2025-11-29  
**相关任务**: VOI-6
