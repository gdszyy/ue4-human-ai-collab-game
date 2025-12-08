# 物理系统测试场景配置指南

本文档描述了VOI-28物理系统集成测试所需的测试场景配置。这些场景需要在UE4编辑器中创建。

---

## 测试场景概览

| 场景名称 | 用途 | 优先级 | 状态 |
|---------|------|--------|------|
| TestMap_Combat | 战斗场景测试 | High | 待创建 |
| TestMap_Workbench | 炼金工作台测试 | High | 待创建 |
| TestMap_Effects | 特殊效果测试 | Medium | 待创建 |
| TestMap_Performance | 性能压力测试 | Medium | 待创建 |

---

## 1. 战斗场景测试 (TestMap_Combat)

### 场景描述
模拟完整的战斗场景，包含魔力露珠发射、敌人碰撞、特殊效果等。

### 场景配置

#### 基础设置
- **地图尺寸**: 2000x2000x2000 cm
- **边界**: 启用
- **重力**: 启用 (0, 0, -980)
- **场景类型**: Combat

#### 蓝图Actor配置

##### 1. BP_PhysicsTestManager
**类型**: Actor  
**位置**: (0, 0, 0)  
**功能**: 管理测试流程

**组件**:
- `PhysicsSystem` (UMarblePhysicsSystem)
- `CollisionManager` (UCollisionManager)
- `EffectManager` (UPhysicsEffectManager)

**事件图**:
```
Event BeginPlay
  ├─ Initialize PhysicsSystem (Combat Config)
  ├─ Initialize CollisionManager
  └─ Start Test Sequence

Event Tick
  ├─ PhysicsSystem->Tick(DeltaTime)
  ├─ Update Collision Bodies
  ├─ Detect Collisions
  └─ Handle Collision Events
```

##### 2. BP_MarbleLauncher
**类型**: Actor  
**位置**: (0, 0, 100)  
**功能**: 发射魔力露珠

**参数**:
- Launch Speed: 500-1000 cm/s
- Launch Angle: 0-90度
- Launch Interval: 1.0秒
- Max Marbles: 20

##### 3. BP_EnemySpawner
**类型**: Actor  
**位置**: (0, 0, 800)  
**功能**: 生成测试敌人

**参数**:
- Enemy Count: 10-30
- Spawn Pattern: Grid / Random
- Enemy Size: 80x50 cm
- Enemy Health: 10.0

#### 测试流程

1. **初始化阶段** (0-2秒)
   - 初始化物理系统
   - 生成敌人
   - 设置边界

2. **发射阶段** (2-10秒)
   - 每秒发射2-3个魔力露珠
   - 记录发射参数

3. **战斗阶段** (10-30秒)
   - 持续检测碰撞
   - 处理伤害
   - 记录碰撞事件

4. **结果阶段** (30-32秒)
   - 统计碰撞次数
   - 计算平均FPS
   - 输出测试报告

#### 验收标准
- ✅ 场景可正常运行30秒
- ✅ 魔力露珠发射正常
- ✅ 碰撞检测正确
- ✅ FPS > 30 (20个魔力露珠 + 20个敌人)
- ✅ 无崩溃或错误

---

## 2. 炼金工作台测试 (TestMap_Workbench)

### 场景描述
模拟炼金工作台场景，测试精确的物理模拟和注入点交互。

### 场景配置

#### 基础设置
- **地图尺寸**: 1000x1000x1000 cm
- **边界**: 启用
- **重力**: 启用 (0, 0, -980)
- **场景类型**: Workbench

#### 蓝图Actor配置

##### 1. BP_WorkbenchTestManager
**类型**: Actor  
**位置**: (0, 0, 0)  
**功能**: 管理工作台测试

**组件**:
- `PhysicsSystem` (UMarblePhysicsSystem)
- `CollisionManager` (UCollisionManager)

**事件图**:
```
Event BeginPlay
  ├─ Initialize PhysicsSystem (Workbench Config)
  ├─ Initialize CollisionManager
  ├─ Create Injection Points
  └─ Start Test

Event Tick
  ├─ PhysicsSystem->Tick(DeltaTime)
  ├─ Update Collision Bodies
  ├─ Detect Collisions with Injection Points
  └─ Handle Injection Events
```

##### 2. BP_InjectionPoint
**类型**: Actor  
**位置**: 5个，均匀分布  
**功能**: 魔力露珠注入点

**参数**:
- Radius: 20 cm
- Color: 根据类型变化
- Injection Type: Fire / Water / Earth / Air / Neutral

**位置列表**:
- Point 1: (-200, 0, 200) - Fire
- Point 2: (-100, 0, 200) - Water
- Point 3: (0, 0, 200) - Earth
- Point 4: (100, 0, 200) - Air
- Point 5: (200, 0, 200) - Neutral

##### 3. BP_WorkbenchLauncher
**类型**: Actor  
**位置**: (0, 0, 500)  
**功能**: 发射魔力露珠到注入点

**参数**:
- Launch Speed: 200 cm/s
- Launch Direction: 向下 (0, 0, -1)
- Launch Count: 10
- Launch Interval: 0.5秒

#### 测试流程

1. **初始化阶段** (0-1秒)
   - 初始化物理系统
   - 创建注入点
   - 设置边界

2. **发射阶段** (1-6秒)
   - 每0.5秒发射1个魔力露珠
   - 随机选择目标注入点

3. **注入阶段** (6-10秒)
   - 检测魔力露珠与注入点碰撞
   - 记录注入事件
   - 统计注入成功率

4. **结果阶段** (10-12秒)
   - 输出注入统计
   - 验证精度

#### 验收标准
- ✅ 场景可正常运行12秒
- ✅ 魔力露珠可准确到达注入点
- ✅ 注入成功率 > 80%
- ✅ 物理模拟精确
- ✅ 无崩溃或错误

---

## 3. 特殊效果测试 (TestMap_Effects)

### 场景描述
测试所有特殊物理效果（引力场、虫洞、速度修改）的功能和视觉表现。

### 场景配置

#### 基础设置
- **地图尺寸**: 4000x4000x2000 cm
- **边界**: 启用
- **重力**: 禁用（便于观察效果）
- **场景类型**: Combat

#### 蓝图Actor配置

##### 1. BP_EffectsTestManager
**类型**: Actor  
**位置**: (0, 0, 0)  
**功能**: 管理效果测试

**组件**:
- `PhysicsSystem` (UMarblePhysicsSystem)
- `EffectManager` (UPhysicsEffectManager)

**事件图**:
```
Event BeginPlay
  ├─ Initialize PhysicsSystem (No Gravity)
  ├─ Create Test Effects
  │   ├─ Gravity Field (Center)
  │   ├─ Wormhole (Left to Right)
  │   └─ Velocity Modifier (Accelerator)
  └─ Start Test

Event Tick
  ├─ PhysicsSystem->Tick(DeltaTime)
  └─ Update Visual Indicators
```

##### 2. BP_GravityFieldVisualizer
**类型**: Actor  
**位置**: (0, 0, 500)  
**功能**: 可视化引力场

**参数**:
- Field Radius: 1000 cm
- Gravity Strength: 2000 cm/s²
- Visual: 半透明球体 + 箭头指示

##### 3. BP_WormholeVisualizer
**类型**: Actor  
**位置**: 入口(-1500, 0, 500), 出口(1500, 0, 500)  
**功能**: 可视化虫洞

**参数**:
- Entrance Radius: 100 cm
- Exit Velocity Multiplier: 1.5x
- Visual: 旋涡特效 + 连接线

##### 4. BP_AcceleratorVisualizer
**类型**: Actor  
**位置**: (0, 1500, 500)  
**功能**: 可视化加速区域

**参数**:
- Area Type: Sphere
- Area Radius: 300 cm
- Velocity Multiplier: 2.0x
- Visual: 发光区域 + 粒子流

##### 5. BP_EffectTestLauncher
**类型**: Actor  
**位置**: 多个位置  
**功能**: 发射测试魔力露珠

**测试用例**:
1. **引力场测试**: 从四周发射魔力露珠，观察被吸引
2. **虫洞测试**: 从左侧发射，观察传送到右侧
3. **加速器测试**: 穿过加速区域，观察速度变化
4. **组合测试**: 同时激活多个效果

#### 测试流程

1. **引力场测试** (0-10秒)
   - 从四周发射20个魔力露珠
   - 观察被吸引到中心
   - 验证引力强度

2. **虫洞测试** (10-20秒)
   - 从左侧发射10个魔力露珠
   - 观察传送到右侧
   - 验证速度变化

3. **加速器测试** (20-30秒)
   - 发射魔力露珠穿过加速区域
   - 观察速度增加
   - 验证倍数正确

4. **组合测试** (30-60秒)
   - 同时激活所有效果
   - 发射50个魔力露珠
   - 观察复杂交互

#### 验收标准
- ✅ 所有效果可正常工作
- ✅ 视觉表现清晰
- ✅ 效果组合无冲突
- ✅ FPS > 30 (50个魔力露珠 + 3个效果)
- ✅ 无崩溃或错误

---

## 4. 性能压力测试 (TestMap_Performance)

### 场景描述
测试物理系统在高负载下的性能表现。

### 场景配置

#### 基础设置
- **地图尺寸**: 4000x4000x2000 cm
- **边界**: 启用
- **重力**: 启用 (0, 0, -980)
- **场景类型**: Combat

#### 蓝图Actor配置

##### 1. BP_PerformanceTestManager
**类型**: Actor  
**位置**: (0, 0, 0)  
**功能**: 管理性能测试

**组件**:
- `PhysicsSystem` (UMarblePhysicsSystem)
- `CollisionManager` (UCollisionManager)
- `EffectManager` (UPhysicsEffectManager)
- `PerformanceMonitor` (自定义组件)

**事件图**:
```
Event BeginPlay
  ├─ Initialize All Systems
  ├─ Start Performance Monitor
  └─ Begin Stress Test

Event Tick
  ├─ Update All Systems
  ├─ Record Performance Metrics
  │   ├─ FPS
  │   ├─ Physics Tick Time
  │   ├─ Collision Detection Time
  │   └─ Memory Usage
  └─ Display Real-time Stats
```

##### 2. BP_StressTestLauncher
**类型**: Actor  
**位置**: (0, 0, 1000)  
**功能**: 大量发射魔力露珠

**参数**:
- Max Marbles: 200
- Launch Rate: 10/秒
- Launch Pattern: Random

#### 测试场景

##### 场景1: 基础性能测试
- **魔力露珠数量**: 100
- **敌人数量**: 50
- **特殊效果**: 0
- **目标FPS**: > 30

##### 场景2: 效果性能测试
- **魔力露珠数量**: 100
- **敌人数量**: 50
- **特殊效果**: 5 (2个引力场 + 2个虫洞 + 1个加速器)
- **目标FPS**: > 25

##### 场景3: 极限压力测试
- **魔力露珠数量**: 200
- **敌人数量**: 100
- **特殊效果**: 10
- **目标FPS**: > 20

#### 性能指标

| 指标 | 目标值 | 测量方法 |
|------|--------|---------|
| FPS | > 30 | UE4性能分析器 |
| Physics Tick Time | < 10ms | 自定义计时器 |
| Collision Detection Time | < 5ms | 自定义计时器 |
| Memory Usage | < 500MB | UE4内存分析器 |
| Marble Count | 100-200 | PhysicsSystem->GetMarbleCount() |

#### 测试流程

1. **预热阶段** (0-5秒)
   - 初始化系统
   - 发射初始魔力露珠
   - 稳定FPS

2. **渐进加载** (5-30秒)
   - 每秒增加10个魔力露珠
   - 记录FPS变化
   - 监控性能指标

3. **稳定测试** (30-60秒)
   - 维持最大负载
   - 持续记录性能
   - 检测性能下降

4. **结果分析** (60-65秒)
   - 输出性能报告
   - 生成性能曲线图
   - 标记性能瓶颈

#### 验收标准
- ✅ 100个魔力露珠场景 FPS > 30
- ✅ 200个魔力露珠场景 FPS > 20
- ✅ Physics Tick Time < 10ms
- ✅ Collision Detection Time < 5ms
- ✅ 无内存泄漏
- ✅ 系统稳定运行60秒

---

## 测试执行指南

### 1. 创建测试场景

#### 步骤1: 创建地图
1. 在UE4编辑器中，`File` → `New Level` → `Empty Level`
2. 保存为对应的测试场景名称（如 `TestMap_Combat`）
3. 添加基础光照和后期处理

#### 步骤2: 创建蓝图Actor
1. `Content Browser` → 右键 → `Blueprint Class` → `Actor`
2. 命名为对应的蓝图名称（如 `BP_PhysicsTestManager`）
3. 按照配置文档添加组件和事件图

#### 步骤3: 配置场景
1. 将蓝图Actor拖入场景
2. 设置位置和参数
3. 配置物理系统和碰撞管理器

#### 步骤4: 测试运行
1. 点击 `Play` 按钮运行场景
2. 观察测试流程
3. 记录测试结果

### 2. 运行自动化测试

#### 在编辑器中运行
1. `Window` → `Test Automation`
2. 搜索 `EchoAlchemist.Physics`
3. 选择要运行的测试
4. 点击 `Run Tests`

#### 命令行运行
```bash
# 运行所有物理测试
UnrealEditor.exe "ProjectPath.uproject" -ExecCmds="Automation RunTests EchoAlchemist.Physics" -unattended -nopause -testexit="Automation Test Queue Empty"

# 运行集成测试
UnrealEditor.exe "ProjectPath.uproject" -ExecCmds="Automation RunTests EchoAlchemist.Physics.Integration" -unattended -nopause -testexit="Automation Test Queue Empty"

# 运行性能测试
UnrealEditor.exe "ProjectPath.uproject" -ExecCmds="Automation RunTests EchoAlchemist.Physics.EffectsIntegration" -unattended -nopause -testexit="Automation Test Queue Empty"
```

### 3. 生成测试报告

测试完成后，UE4会生成测试报告：
- **位置**: `Saved/Automation/Reports/`
- **格式**: HTML + JSON
- **内容**: 测试结果、性能数据、错误日志

---

## 故障排查

### 常见问题

#### 1. 编译错误
**问题**: 测试代码无法编译  
**解决方案**:
- 检查UE4版本（需要4.27+）
- 验证所有头文件路径正确
- 确保 `WITH_DEV_AUTOMATION_TESTS` 宏已定义

#### 2. 测试失败
**问题**: 测试运行但失败  
**解决方案**:
- 查看测试日志（`Output Log`）
- 检查物理系统初始化
- 验证测试参数正确

#### 3. 性能不达标
**问题**: FPS低于目标值  
**解决方案**:
- 使用UE4 Profiler分析瓶颈
- 优化空间网格尺寸
- 减少碰撞检测频率
- 启用对象池

#### 4. 视觉效果异常
**问题**: 特殊效果不可见  
**解决方案**:
- 检查材质和粒子系统
- 验证效果位置和范围
- 确保摄像机视角正确

---

## 下一步

1. **创建测试场景**: 按照本文档在UE4编辑器中创建4个测试场景
2. **运行测试**: 执行自动化测试并记录结果
3. **性能优化**: 根据测试结果优化性能瓶颈
4. **文档更新**: 更新测试结果到任务资源映射表

---

## 参考资料

- [UE4自动化测试文档](https://docs.unrealengine.com/4.27/en-US/TestingAndOptimization/Automation/)
- [物理系统设计文档](../CoreModules/PhysicsSystem.md)
- [特殊效果设计文档](../CoreModules/SpecialPhysicsEffects.md)
- [快速入门指南](../QUICKSTART.md)
