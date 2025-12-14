# MonsterVisualizer 使用文档

**模块：** PCG | Visualizer  
**C++类：** `AMonsterVisualizer`

## 1. 功能说明

`MonsterVisualizer` 是一个临时视觉模块，用于在UE5编辑器中预览程序化生成的怪兽。它将怪兽属性（生态属性和战斗属性）可视化为简单的彩色精灵，帮助开发者和设计师快速验证怪兽生成系统的效果。

这是一个**测试和预览工具**，不用于最终游戏。在正式开发中，应该使用完整的3D模型或2D精灵资源替换这个临时可视化系统。

## 2. 快速开始

### 2.1 在编辑器中使用

1. 在内容浏览器中找到 `MonsterVisualizer` 蓝图类（或C++类）
2. 将其拖放到关卡中
3. 在Details面板中设置 `Monster Attributes`
4. 点击 `Regenerate Visual` 按钮（或启用 `Auto Regenerate On Change`）
5. 观察怪兽的可视化效果

### 2.2 在蓝图中使用

```
// 生成怪兽属性
Generate Monster Attributes(Seed: 12345, Climate Value: 0.7, Terrain Value: 0.5, Num Traits: 2) → Monster Attributes

// 创建可视化Actor
Spawn Actor(Monster Visualizer, Transform) → Visualizer

// 设置属性并显示
Visualizer → Set Monster Attributes(Monster Attributes)
```

## 3. 可视化规则

### 3.1 身体颜色（基于栖息地）

`MonsterVisualizer` 使用不同的颜色表示不同的栖息地类型：

| 栖息地 | 颜色 | RGB值 |
| :--- | :--- | :--- |
| **森林** | 绿色 | (0.2, 0.8, 0.2) |
| **沙漠** | 沙黄色 | (0.9, 0.7, 0.3) |
| **冰原** | 冰蓝色 | (0.7, 0.9, 1.0) |
| **洞穴** | 深灰色 | (0.4, 0.4, 0.5) |
| **火山** | 岩浆红 | (1.0, 0.3, 0.1) |
| **沼泽** | 浑浊绿 | (0.4, 0.6, 0.3) |

### 3.2 身体大小（基于体型）

`MonsterVisualizer` 使用不同的缩放比例表示不同的体型等级：

| 体型 | 缩放比例 | 相对大小 |
| :--- | :--- | :--- |
| **微小** | 0.5× | 50% |
| **小型** | 0.75× | 75% |
| **中型** | 1.0× | 100% |
| **大型** | 1.5× | 150% |
| **巨型** | 2.5× | 250% |

### 3.3 部件颜色（基于战斗词条稀有度）

战斗词条以额外的精灵组件显示，使用不同的颜色表示稀有度：

| 稀有度 | 颜色 | RGB值 |
| :--- | :--- | :--- |
| **普通** | 灰色 | (0.8, 0.8, 0.8) |
| **稀有** | 蓝色 | (0.3, 0.5, 1.0) |
| **史诗** | 紫色 | (0.8, 0.2, 1.0) |

### 3.4 部件位置

战斗词条的精灵组件会在身体上方水平排列，每个词条偏移20单位，便于区分。

## 4. 蓝图函数

### 4.1 Set Monster Attributes

设置怪兽属性并重新生成可视化。

**参数：**
- `Attributes` (FMonsterAttributes)：要可视化的怪兽属性

**使用示例：**
```
Generate Monster Attributes(...) → Monster Attributes
Visualizer → Set Monster Attributes(Monster Attributes)
```

### 4.2 Regenerate Visual

根据当前的怪兽属性重新生成可视化。

**使用示例：**
```
// 修改属性后重新生成
Visualizer.Monster Attributes.Ecology Attributes.Size Class = Large
Visualizer → Regenerate Visual()
```

### 4.3 Clear Visual

清除所有可视化组件。

**使用示例：**
```
// 清除当前可视化
Visualizer → Clear Visual()
```

## 5. 属性说明

### 5.1 Monster Attributes

**类型：** `FMonsterAttributes`  
**可编辑：** 是  
**描述：** 要可视化的怪兽属性，包含生态属性和战斗属性。

### 5.2 Auto Regenerate On Change

**类型：** `bool`  
**可编辑：** 是  
**默认值：** `true`  
**描述：** 是否在编辑器中修改属性时自动重新生成可视化。

## 6. 使用场景

### 6.1 测试属性生成器

快速验证 `AttributeGenerator` 生成的属性是否符合预期。

```
// 测试不同气候和地势的组合
For Climate Value from 0.0 to 1.0 step 0.2
  For Terrain Value from 0.0 to 1.0 step 0.2
    Generate Monster Attributes(Seed, Climate Value, Terrain Value, 2) → Monster Attributes
    Spawn Actor(Monster Visualizer, Location) → Visualizer
    Visualizer → Set Monster Attributes(Monster Attributes)
    Location.X += 200
```

### 6.2 预览世界怪兽分布

在关卡中可视化不同位置的怪兽分布。

```
// 在网格上生成怪兽预览
For X from 0 to 10
  For Y from 0 to 10
    Location = (X × 200, Y × 200, 0)
    Seed = X + Y × 100
    Sample World State At Location(Location) → Climate, Terrain
    Generate Monster Attributes(Seed, Climate, Terrain, 2) → Monster Attributes
    Spawn Actor(Monster Visualizer, Location) → Visualizer
    Visualizer → Set Monster Attributes(Monster Attributes)
```

### 6.3 调试战斗词条

验证战斗词条的生成和可视化。

```
// 测试不同稀有度的词条
For Min Rarity in [Common, Rare, Epic]
  Generate Combat Attributes(Seed, 3, Min Rarity) → Combat Attributes
  Monster Attributes.Combat Attributes = Combat Attributes
  Visualizer → Set Monster Attributes(Monster Attributes)
  Wait 2 seconds
```

## 7. 限制和注意事项

### 7.1 临时工具

`MonsterVisualizer` 是一个**临时测试工具**，不应该用于最终游戏。它的主要目的是：
- 快速验证属性生成逻辑
- 预览怪兽分布
- 调试战斗词条

在正式开发中，应该使用完整的3D模型或2D精灵资源。

### 7.2 精灵资源

当前实现中，`MonsterVisualizer` 不加载实际的精灵资源，只使用颜色和缩放进行可视化。如果需要显示真实的精灵，需要：
1. 创建精灵资源（身体、部件）
2. 在 `CreateBodySprite()` 和 `CreatePartSprites()` 中加载精灵
3. 根据属性选择合适的精灵

### 7.3 性能考虑

`MonsterVisualizer` 会为每个战斗词条创建一个精灵组件，如果大量生成可视化Actor，可能会影响编辑器性能。建议：
- 限制同时显示的可视化Actor数量
- 使用后及时销毁不需要的可视化Actor
- 在运行时不要使用此工具

### 7.4 编辑器专用

`PostEditChangeProperty` 函数只在编辑器中有效，运行时不会自动重新生成可视化。如果需要运行时动态更新，应该手动调用 `RegenerateVisual()`。

## 8. 扩展建议

### 8.1 添加真实精灵

在 `CreateBodySprite()` 中加载真实的精灵资源：

```cpp
// 根据栖息地和体型选择精灵
FString SpritePath = FString::Printf(TEXT("/Game/Sprites/Monsters/%s_%s"),
    *UEnum::GetValueAsString(MonsterAttributes.EcologyAttributes.Habitat),
    *UEnum::GetValueAsString(MonsterAttributes.EcologyAttributes.SizeClass));
UPaperSprite* Sprite = LoadObject<UPaperSprite>(nullptr, *SpritePath);
BodySprite->SetSprite(Sprite);
```

### 8.2 添加动画

使用 `ProceduralAnimator` 生成的关键帧创建简单的动画：

```cpp
// 生成待机动画
TArray<FVector2D> IdleKeyframes = UProceduralAnimator::GenerateIdleAnimation(10, 5.0f, 1.0f);

// 应用动画（需要实现动画播放逻辑）
PlayIdleAnimation(IdleKeyframes);
```

### 8.3 添加属性显示

在可视化Actor上方显示属性文本：

```cpp
// 创建文本渲染组件
UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(this);
TextComponent->SetText(FText::FromString(FString::Printf(TEXT("HP: %.0f\nATK: %.0f"), HP, ATK)));
TextComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
TextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
```

## 9. 完整示例

### 示例1：在编辑器中预览怪兽

1. 在关卡中放置一个 `MonsterVisualizer` Actor
2. 在Details面板中设置属性：
   - Ecology Attributes → Habitat: Desert
   - Ecology Attributes → Size Class: Large
   - Combat Attributes → Traits: 添加2个战斗词条
3. 启用 `Auto Regenerate On Change`
4. 观察可视化效果：大型沙黄色身体 + 2个彩色部件

### 示例2：批量生成预览

```
// 在网格上生成100个怪兽预览
For X from 0 to 9
  For Y from 0 to 9
    Location = (X × 300, Y × 300, 0)
    Seed = X + Y × 10
    Climate = X / 9.0
    Terrain = Y / 9.0
    
    Generate Monster Attributes(Seed, Climate, Terrain, 2) → Monster Attributes
    Spawn Actor(Monster Visualizer, Location) → Visualizer
    Visualizer → Set Monster Attributes(Monster Attributes)
```

### 示例3：动态更新可视化

```
// 每秒生成一个新的怪兽
Event Tick
  Accumulator += Delta Time
  If Accumulator >= 1.0
    Accumulator = 0
    Seed = Random Integer In Range(0, 10000)
    Generate Monster Attributes(Seed, 0.5, 0.5, 2) → Monster Attributes
    Visualizer → Set Monster Attributes(Monster Attributes)
```

## 10. 调试技巧

1. **颜色检查**：确认身体颜色是否与栖息地匹配
2. **大小检查**：确认身体大小是否与体型等级匹配
3. **部件数量**：确认部件数量是否与战斗词条数量匹配
4. **部件颜色**：确认部件颜色是否与词条稀有度匹配
5. **日志输出**：在 `RegenerateVisual()` 中添加日志，输出属性信息

## 11. 未来改进

1. **3D可视化**：支持3D模型的程序化组装
2. **动画播放**：集成 `ProceduralAnimator` 的动画系统
3. **属性面板**：在可视化Actor上方显示详细的属性信息
4. **交互功能**：点击可视化Actor查看详细属性
5. **导出功能**：将可视化结果导出为图片或数据表
