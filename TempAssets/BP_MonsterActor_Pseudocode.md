# BP_MonsterActor 蓝图伪代码

本文档提供BP_MonsterActor蓝图的详细实现伪代码，您可以按照这些伪代码在UE4蓝图编辑器中实现。

---

## 1. 蓝图创建步骤

### 1.1 创建蓝图类

1. 在内容浏览器中右键 → Blueprint Class
2. 选择 `MonsterActor` 作为父类
3. 命名为 `BP_MonsterActor`
4. 保存到 `Content/Blueprints/Monsters/` 目录

### 1.2 设置Class Defaults

打开BP_MonsterActor，在Class Defaults面板中设置以下属性：

```
Skeleton Data Table: DT_Skeletons
Part Data Table: DT_Parts
Palette Data Table: DT_Palettes
Palette Swap Material: M_PaletteSwap_Master
```

---

## 2. 事件图（Event Graph）

### 2.1 Event BeginPlay

```
Event BeginPlay
  ↓
[Branch]
  Condition: Monster Attributes.Ecology Attributes.Habitat != Forest
           OR Monster Attributes.Combat Attributes.Traits.Num() > 0
  ↓
  True → Call "Reconstruct Appearance"
  False → (Do nothing)
```

**说明**：
- 如果在编辑器中设置了怪兽属性，则在游戏开始时自动重建外观
- 这允许在编辑器中预览怪兽

---

## 3. 自定义函数

### 3.1 Test Generate Monster（测试函数）

创建一个测试函数，用于快速生成怪兽进行测试。

```
Function: Test Generate Monster
  Inputs:
    - Test Seed (Integer) = 12345
    - Test Climate (Float) = 0.7
    - Test Terrain (Float) = 0.5
    - Test Num Traits (Integer) = 2
  
  Body:
    [Generate Monster Attributes]
      Seed: Test Seed
      Climate Value: Test Climate
      Terrain Value: Test Terrain
      Num Traits: Test Num Traits
      → Monster Attributes
    
    [Set Monster Attributes]
      Monster Attributes: Monster Attributes
```

**使用方法**：
- 在编辑器中选中BP_MonsterActor实例
- 在Details面板中找到"Test Generate Monster"函数
- 点击"Call in Editor"按钮
- 怪兽会立即生成并显示

### 3.2 Change Habitat（测试函数）

创建一个测试函数，用于动态改变怪兽的栖息地。

```
Function: Change Habitat
  Inputs:
    - New Habitat (EHabitatType)
  
  Body:
    [Set Ecology Attributes.Habitat]
      Monster Attributes → Ecology Attributes → Habitat = New Habitat
    
    [Reconstruct Appearance]
```

**使用方法**：
- 在编辑器中选中BP_MonsterActor实例
- 调用"Change Habitat"函数
- 选择新的栖息地
- 怪兽颜色会立即改变

### 3.3 Play Random Animation（测试函数）

创建一个测试函数，用于播放随机动画。

```
Function: Play Random Animation
  
  Body:
    [Random Integer in Range]
      Min: 0
      Max: 3
      → Random Index
    
    [Switch on Int]
      Selection: Random Index
      
      Case 0:
        [Play Animation]
          Animation Type: Idle
      
      Case 1:
        [Play Animation]
          Animation Type: Walk
      
      Case 2:
        [Play Animation]
          Animation Type: Attack
      
      Case 3:
        [Play Animation]
          Animation Type: Death
```

**使用方法**：
- 在编辑器中选中BP_MonsterActor实例
- 调用"Play Random Animation"函数
- 怪兽会播放随机动画

---

## 4. 蓝图示例：批量生成怪兽

创建一个测试Actor，用于批量生成怪兽。

### 4.1 创建BP_MonsterSpawnerTest

1. 在内容浏览器中右键 → Blueprint Class
2. 选择 `Actor` 作为父类
3. 命名为 `BP_MonsterSpawnerTest`
4. 保存到 `Content/Blueprints/Monsters/` 目录

### 4.2 Event Graph

```
Event BeginPlay
  ↓
[For Loop]
  First Index: 0
  Last Index: 9
  Loop Body:
    Current X Index → X
    
    [For Loop (Nested)]
      First Index: 0
      Last Index: 9
      Loop Body:
        Current Y Index → Y
        
        [Calculate Location]
          Location X = X × 300
          Location Y = Y × 300
          Location Z = 0
          → Spawn Location
        
        [Calculate Seed]
          Seed = X + Y × 10
          → Monster Seed
        
        [Generate Monster Attributes]
          Seed: Monster Seed
          Climate Value: 0.7
          Terrain Value: 0.5
          Num Traits: 2
          → Monster Attributes
        
        [Spawn Actor from Class]
          Class: BP_MonsterActor
          Spawn Transform: (Location: Spawn Location, Rotation: (0,0,0), Scale: (1,1,1))
          → Spawned Actor
        
        [Cast to MonsterActor]
          Object: Spawned Actor
          → Monster Actor
        
        [Set Monster Attributes]
          Target: Monster Actor
          Monster Attributes: Monster Attributes
```

**说明**：
- 这个蓝图会在10×10的网格上生成100个怪兽
- 每个怪兽使用不同的种子，因此外观各不相同
- 间距为300单位

### 4.3 使用方法

1. 将BP_MonsterSpawnerTest拖入关卡
2. 运行游戏（Play）
3. 观察100个怪兽的生成效果

---

## 5. 蓝图示例：动态调整怪兽

创建一个测试Actor，用于动态调整怪兽属性。

### 5.1 创建BP_MonsterDynamicTest

1. 在内容浏览器中右键 → Blueprint Class
2. 选择 `Actor` 作为父类
3. 命名为 `BP_MonsterDynamicTest`
4. 保存到 `Content/Blueprints/Monsters/` 目录

### 5.2 添加变量

```
Variables:
  - Monster Actor Reference (MonsterActor, Instance Editable)
  - Current Habitat Index (Integer) = 0
```

### 5.3 Event Graph

```
Event BeginPlay
  ↓
[Generate Monster Attributes]
  Seed: 12345
  Climate Value: 0.7
  Terrain Value: 0.5
  Num Traits: 2
  → Monster Attributes

[Spawn Actor from Class]
  Class: BP_MonsterActor
  Spawn Transform: (Location: (0,0,100), Rotation: (0,0,0), Scale: (1,1,1))
  → Spawned Actor

[Cast to MonsterActor]
  Object: Spawned Actor
  → Monster Actor

[Set Monster Actor Reference]
  Value: Monster Actor

[Set Monster Attributes]
  Target: Monster Actor
  Monster Attributes: Monster Attributes

[Set Timer by Event]
  Event: Change Habitat Timer
  Time: 2.0
  Looping: True
```

```
Custom Event: Change Habitat Timer
  ↓
[Branch]
  Condition: Monster Actor Reference != None
  ↓
  True:
    [Increment Current Habitat Index]
      Current Habitat Index = (Current Habitat Index + 1) % 6
    
    [Switch on Int]
      Selection: Current Habitat Index
      
      Case 0:
        New Habitat = Forest
      Case 1:
        New Habitat = Desert
      Case 2:
        New Habitat = Tundra
      Case 3:
        New Habitat = Swamp
      Case 4:
        New Habitat = Mountain
      Case 5:
        New Habitat = Ocean
    
    [Get Monster Attributes]
      Target: Monster Actor Reference
      → Current Attributes
    
    [Set Ecology Attributes.Habitat]
      Current Attributes → Ecology Attributes → Habitat = New Habitat
    
    [Set Monster Attributes]
      Target: Monster Actor Reference
      Monster Attributes: Current Attributes
    
    [Print String]
      In String: "Changed habitat to: " + New Habitat
      Duration: 2.0
  
  False:
    (Do nothing)
```

**说明**：
- 这个蓝图会生成一个怪兽
- 每2秒自动切换栖息地
- 怪兽颜色会随着栖息地改变而改变

### 5.4 使用方法

1. 将BP_MonsterDynamicTest拖入关卡
2. 运行游戏（Play）
3. 观察怪兽颜色每2秒变化一次

---

## 6. 蓝图示例：怪兽动画测试

创建一个测试Actor，用于测试怪兽动画。

### 6.1 创建BP_MonsterAnimationTest

1. 在内容浏览器中右键 → Blueprint Class
2. 选择 `Actor` 作为父类
3. 命名为 `BP_MonsterAnimationTest`
4. 保存到 `Content/Blueprints/Monsters/` 目录

### 6.2 添加变量

```
Variables:
  - Monster Actor Reference (MonsterActor, Instance Editable)
  - Current Animation Index (Integer) = 0
```

### 6.3 Event Graph

```
Event BeginPlay
  ↓
[Generate Monster Attributes]
  Seed: 12345
  Climate Value: 0.7
  Terrain Value: 0.5
  Num Traits: 2
  → Monster Attributes

[Spawn Actor from Class]
  Class: BP_MonsterActor
  Spawn Transform: (Location: (0,0,100), Rotation: (0,0,0), Scale: (1,1,1))
  → Spawned Actor

[Cast to MonsterActor]
  Object: Spawned Actor
  → Monster Actor

[Set Monster Actor Reference]
  Value: Monster Actor

[Set Monster Attributes]
  Target: Monster Actor
  Monster Attributes: Monster Attributes

[Set Timer by Event]
  Event: Change Animation Timer
  Time: 1.5
  Looping: True
```

```
Custom Event: Change Animation Timer
  ↓
[Branch]
  Condition: Monster Actor Reference != None
  ↓
  True:
    [Increment Current Animation Index]
      Current Animation Index = (Current Animation Index + 1) % 4
    
    [Switch on Int]
      Selection: Current Animation Index
      
      Case 0:
        [Play Animation]
          Target: Monster Actor Reference
          Animation Type: Idle
        [Print String]
          In String: "Playing: Idle"
      
      Case 1:
        [Play Animation]
          Target: Monster Actor Reference
          Animation Type: Walk
        [Print String]
          In String: "Playing: Walk"
      
      Case 2:
        [Play Animation]
          Target: Monster Actor Reference
          Animation Type: Attack
        [Print String]
          In String: "Playing: Attack"
      
      Case 3:
        [Play Animation]
          Target: Monster Actor Reference
          Animation Type: Death
        [Print String]
          In String: "Playing: Death"
  
  False:
    (Do nothing)
```

**说明**：
- 这个蓝图会生成一个怪兽
- 每1.5秒自动切换动画
- 按照Idle → Walk → Attack → Death的顺序循环

### 6.4 使用方法

1. 将BP_MonsterAnimationTest拖入关卡
2. 运行游戏（Play）
3. 观察怪兽动画每1.5秒变化一次

---

## 7. 调试技巧

### 7.1 在编辑器中预览怪兽

1. 将BP_MonsterActor拖入关卡
2. 在Details面板中设置Monster Attributes
3. 调用"Reconstruct Appearance"函数
4. 怪兽会立即显示

### 7.2 使用Print String调试

在关键节点添加Print String节点，输出调试信息：

```
[Print String]
  In String: "Monster Seed: " + Monster Seed
  Text Color: (R=0,G=1,B=0,A=1)
  Duration: 5.0
```

### 7.3 使用Draw Debug节点

在生成怪兽时绘制调试信息：

```
[Draw Debug String]
  World Context Object: Self
  Text: "Monster: " + Monster Attributes.Ecology Attributes.Habitat
  Location: Spawn Location + (0,0,100)
  Text Color: (R=1,G=1,B=1,A=1)
  Duration: 10.0
```

---

## 8. 常见问题

### 8.1 怪兽没有显示

**检查清单**：
- [ ] BP_MonsterActor的Class Defaults中是否设置了数据表引用？
- [ ] 数据表中是否填写了Flipbook引用？
- [ ] Flipbook是否正确创建？
- [ ] 是否调用了Set Monster Attributes？

**调试方法**：
```
[Print String]
  In String: "Base Flipbook Component: " + Base Flipbook Component
  
[Print String]
  In String: "Current Skeleton Data: " + Current Skeleton Data.Skeleton ID
```

### 8.2 怪兽颜色不正确

**检查清单**：
- [ ] DT_Palettes中是否填写了正确的颜色值？
- [ ] M_PaletteSwap_Master材质是否正确创建？
- [ ] 材质参数名称是否正确（PrimaryColor、SecondaryColor、AccentColor）？

**调试方法**：
```
[Print String]
  In String: "Current Palette Data: " + Current Palette Data.Habitat
  
[Print String]
  In String: "Primary Color: " + Current Palette Data.Primary Color
```

### 8.3 怪兽部件位置不正确

**检查清单**：
- [ ] DT_Parts中的Relative Location是否正确？
- [ ] 锚点名称是否正确（Head、Back、Tail等）？
- [ ] Z Order是否正确设置？

**调试方法**：
```
[For Each Loop]
  Array: Part Components
  Loop Body:
    [Print String]
      In String: "Part Location: " + Part Component.Relative Location
```

### 8.4 动画不播放

**检查清单**：
- [ ] DT_Skeletons中是否填写了Flipbook引用？
- [ ] Flipbook是否设置了正确的帧率？
- [ ] Flipbook是否设置了正确的循环模式？

**调试方法**：
```
[Print String]
  In String: "Current Flipbook: " + Base Flipbook Component.Flipbook
  
[Print String]
  In String: "Is Playing: " + Base Flipbook Component.Is Playing
```

---

## 9. 性能优化建议

### 9.1 使用对象池

如果需要生成大量怪兽，建议使用对象池：

```
Function: Spawn Monster from Pool
  Inputs:
    - Monster Attributes (FMonsterAttributes)
    - Spawn Location (Vector)
  
  Body:
    [Branch]
      Condition: Monster Pool.Num() > 0
      
      True:
        [Array Get]
          Target Array: Monster Pool
          Index: 0
          → Pooled Monster
        
        [Array Remove Index]
          Target Array: Monster Pool
          Index: 0
        
        [Set Actor Location]
          Target: Pooled Monster
          New Location: Spawn Location
        
        [Set Actor Hidden in Game]
          Target: Pooled Monster
          New Hidden: False
        
        [Set Monster Attributes]
          Target: Pooled Monster
          Monster Attributes: Monster Attributes
        
        Return: Pooled Monster
      
      False:
        [Spawn Actor from Class]
          Class: BP_MonsterActor
          Spawn Transform: (Location: Spawn Location)
          → New Monster
        
        [Set Monster Attributes]
          Target: New Monster
          Monster Attributes: Monster Attributes
        
        Return: New Monster
```

### 9.2 限制同时显示的怪兽数量

```
Function: Spawn Monster with Limit
  Inputs:
    - Monster Attributes (FMonsterAttributes)
    - Spawn Location (Vector)
    - Max Monsters (Integer) = 100
  
  Body:
    [Branch]
      Condition: Active Monsters.Num() >= Max Monsters
      
      True:
        [Print String]
          In String: "Monster limit reached!"
        Return: None
      
      False:
        [Spawn Monster from Pool]
          Monster Attributes: Monster Attributes
          Spawn Location: Spawn Location
          → New Monster
        
        [Array Add]
          Target Array: Active Monsters
          New Item: New Monster
        
        Return: New Monster
```

---

## 10. 下一步

完成蓝图实现后，您可以：

1. **测试基础功能**
   - 使用BP_MonsterSpawnerTest测试批量生成
   - 使用BP_MonsterDynamicTest测试动态调整
   - 使用BP_MonsterAnimationTest测试动画播放

2. **集成世界变迁系统**
   - 从世界变迁系统获取气候和地势值
   - 根据世界状态动态生成怪兽

3. **实现怪兽AI**
   - 创建AI控制器
   - 实现巡逻、追击、攻击行为

4. **集成战斗系统**
   - 将怪兽与VOI-18战斗系统集成
   - 实现伤害处理和死亡逻辑

祝您开发顺利！
