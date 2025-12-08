# PaletteGenerator 使用文档

**模块：** PCG | Palette  
**C++类：** `UPaletteGenerator`  
**版本：** 2.0  
**最后更新：** 2025-12-08

## 1. 功能说明

`PaletteGenerator` 是一个蓝图函数库，用于程序化地生成颜色调色板。它提供了多种算法，可以根据给定的颜色、色谱或怪兽属性生成和谐的配色方案。这对于实现怪兽、环境和特效的动态颜色变化非常有用。

### 核心特性

- **栖息地色谱系统**：为6种栖息地（森林、沙漠、冰原、洞穴、火山、沼泽）定义了专属的基础色谱
- **生态属性驱动**：根据怪兽的生态属性（栖息地、耐受值）自动生成调色板
- **战斗属性增强**：根据战斗词条的稀有度调整颜色的饱和度和亮度
- **确定性生成**：支持随机种子，确保相同输入产生相同输出
- **完整集成**：与怪兽生成系统深度集成，自动生成符合属性的配色方案

## 2. 蓝图使用示例

### 2.1 从色谱生成调色板

此函数可以从一个预定义的颜色数组（色谱）中，通过插值生成一个新的调色板。

**节点：** `Generate Palette From Spectrum`

**使用方法：**
1. 创建一个`LinearColor`数组，作为你的基础色谱。
2. 调用`Generate Palette From Spectrum`函数，传入色谱和希望生成的颜色数量。
3. 函数将返回一个`Palette`结构体，其中包含了生成的颜色数组。

**示例：**
```
Spectrum: [Red, Orange, Yellow]
Num Colors: 5

结果：
Palette.Colors: [Red, Red-Orange, Orange, Orange-Yellow, Yellow]
```

### 2.2 生成单色调色板

此函数可以从一个基础颜色生成一个单色调色板，通过调整饱和度和亮度来创建变化。

**节点：** `Generate Monochromatic Palette`

**使用方法：**
1. 提供一个基础颜色。
2. 调用`Generate Monochromatic Palette`函数，可以调整饱和度和亮度的变化范围。
3. 函数将返回一个`Palette`结构体。

**示例：**
```
Base Color: Blue (0.0, 0.0, 1.0)
Num Colors: 5
Saturation Range: (0.2, 1.0)
Value Range: (0.3, 1.0)

结果：
Palette.Colors: [Dark desaturated blue, ..., Bright saturated blue]
```

### 2.3 根据生态属性生成调色板（新功能）

此函数根据怪兽的生态属性（栖息地、耐受值）自动生成适合的调色板。

**节点：** `Generate Palette From Ecology`

**使用方法：**
1. 提供一个生态属性结构体（`FEcologyAttributes`）
2. 可选：提供随机种子用于确定性生成
3. 函数将返回一个包含5种颜色的调色板（主色、辅色、第三色、高光色、阴影色）

**示例：**
```
Ecology Attributes:
  - Habitat: Desert (沙漠)
  - Tolerance: 75.0

结果：
Palette.Colors:
  - Color 0: Sand (主色)
  - Color 1: Light sand (辅色)
  - Color 2: Dark orange (第三色)
  - Color 3: Orange (高光色)
  - Color 4: Deep orange (阴影色)
  
颜色饱和度根据Tolerance调整（75%耐受值 → 较高饱和度）
```

### 2.4 根据完整怪兽属性生成调色板（新功能）

这是最常用的函数，综合考虑生态属性和战斗属性，生成完整的怪兽配色方案。

**节点：** `Generate Monster Palette`

**使用方法：**
1. 提供完整的怪兽属性结构体（`FMonsterAttributes`）
2. 可选：提供随机种子
3. 函数将返回一个调色板，颜色反映了怪兽的栖息地、耐受值和战斗词条稀有度

**示例：**
```
Monster Attributes:
  Ecology Attributes:
    - Habitat: Volcano (火山)
    - Tolerance: 80.0
  Combat Attributes:
    - Traits: [Fire Affinity (Epic), Stone Shell (Rare)]

结果：
Palette.Colors:
  - 基于火山栖息地的红橙色系
  - 高饱和度（耐受值80%）
  - 高亮度（Epic稀有度）
  - 颜色更加鲜艳夺目
```

### 2.5 获取栖息地色谱（新功能）

此函数返回特定栖息地的基础色谱，可用于自定义调色板生成。

**节点：** `Get Habitat Spectrum`

**使用方法：**
1. 提供一个栖息地类型（`EHabitatType`）
2. 函数将返回该栖息地的基础颜色数组

**示例：**
```
Habitat: Forest

结果：
Spectrum:
  - Dark green (0.2, 0.4, 0.1)
  - Medium green (0.3, 0.6, 0.2)
  - Brown (0.4, 0.3, 0.1)
  - Light green (0.5, 0.7, 0.3)
```

### 2.6 根据稀有度调整调色板（新功能）

此函数根据战斗词条的稀有度调整现有调色板的饱和度和亮度。

**节点：** `Adjust Palette For Rarity`

**使用方法：**
1. 提供一个基础调色板
2. 提供稀有度等级（Common、Rare、Epic）
3. 函数将返回调整后的调色板

**示例：**
```
Base Palette: [Green colors from Forest habitat]
Rarity: Epic

结果：
Adjusted Palette:
  - 饱和度提升30%
  - 亮度提升20%
  - 颜色更加鲜艳夺目
```

## 3. 参数说明

### GeneratePaletteFromSpectrum
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Spectrum** | `TArray<FLinearColor>` | 输入的基础颜色数组（色谱）。 |
| **NumColors** | `int32` | 希望在调色板中生成的颜色数量。 |
| **Return Value** | `FPalette` | 输出的调色板结构体。 |

### GenerateMonochromaticPalette
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **BaseColor** | `FLinearColor` | 输入的基础颜色。 |
| **NumColors** | `int32` | 希望在调色板中生成的颜色数量。 |
| **SaturationRange** | `FVector2D` | 饱和度变化范围（X=Min, Y=Max）。 |
| **ValueRange** | `FVector2D` | 亮度变化范围（X=Min, Y=Max）。 |
| **Return Value** | `FPalette` | 输出的调色板结构体。 |

### GeneratePaletteFromEcology（新功能）
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **EcologyAttributes** | `FEcologyAttributes` | 怪兽的生态属性。 |
| **Seed** | `int32` | 随机种子（默认为0）。 |
| **Return Value** | `FPalette` | 输出的调色板结构体（5种颜色）。 |

### GenerateMonsterPalette（新功能）
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **MonsterAttributes** | `FMonsterAttributes` | 完整的怪兽属性。 |
| **Seed** | `int32` | 随机种子（默认为0）。 |
| **Return Value** | `FPalette` | 输出的调色板结构体（5种颜色）。 |

### GetHabitatSpectrum（新功能）
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Habitat** | `EHabitatType` | 栖息地类型。 |
| **Return Value** | `TArray<FLinearColor>` | 该栖息地的基础色谱。 |

### AdjustPaletteForRarity（新功能）
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **BasePalette** | `FPalette` | 要调整的基础调色板。 |
| **Rarity** | `ERarity` | 稀有度等级。 |
| **Return Value** | `FPalette` | 调整后的调色板。 |

## 4. 栖息地色谱详解

### 森林（Forest）
- **主题**：绿色、棕色、大地色调
- **色谱**：深绿 → 中绿 → 棕色 → 浅绿
- **适用场景**：森林怪兽、植物类生物

### 沙漠（Desert）
- **主题**：黄色、橙色、沙色
- **色谱**：沙色 → 浅沙色 → 深橙 → 橙色
- **适用场景**：沙漠怪兽、干旱环境生物

### 冰原（Tundra）
- **主题**：白色、蓝色、冰雪色调
- **色谱**：冰蓝 → 白色 → 深蓝 → 浅蓝
- **适用场景**：冰雪怪兽、寒冷环境生物

### 洞穴（Cave）
- **主题**：灰色、暗色、石头色调
- **色谱**：深灰 → 中灰 → 蓝灰 → 浅灰
- **适用场景**：洞穴怪兽、地下生物

### 火山（Volcano）
- **主题**：红色、橙色、火焰色调
- **色谱**：深红 → 橙红 → 橙色 → 黄橙
- **适用场景**：火山怪兽、火属性生物

### 沼泽（Swamp）
- **主题**：深绿、浑浊、毒性色调
- **色谱**：暗绿 → 沼泽绿 → 苔藓绿 → 毒绿
- **适用场景**：沼泽怪兽、毒属性生物

## 5. 稀有度对颜色的影响

### Common（普通）
- **饱和度**：降低20%（0.8倍）
- **亮度**：降低10%（0.9倍）
- **效果**：颜色较为暗淡，不太显眼

### Rare（稀有）
- **饱和度**：提升10%（1.1倍）
- **亮度**：提升10%（1.1倍）
- **效果**：颜色较为鲜艳，引人注目

### Epic（史诗）
- **饱和度**：提升30%（1.3倍）
- **亮度**：提升20%（1.2倍）
- **效果**：颜色非常鲜艳夺目，极具视觉冲击力

## 6. 与怪兽生成系统的集成

`PaletteGenerator` 设计为与怪兽生成系统深度集成：

1. **生态属性影响**：
   - 栖息地决定基础色谱
   - 耐受值影响颜色饱和度（高耐受 = 高饱和）

2. **战斗属性影响**：
   - 战斗词条的稀有度影响颜色的饱和度和亮度
   - 史诗级怪兽拥有最鲜艳的颜色

3. **确定性生成**：
   - 使用随机种子确保相同属性生成相同颜色
   - 便于调试和复现

## 7. 完整蓝图示例

### 示例1：为怪兽生成配色方案

```
// 获取怪兽属性
Get Monster Attributes() → Monster Attributes

// 生成调色板
Generate Monster Palette(Monster Attributes, Seed: 12345) → Palette

// 应用到怪兽材质
For Each(Palette.Colors)
  Set Material Parameter(Material, "Color" + Index, Color)
```

### 示例2：根据世界状态生成环境配色

```
// 获取世界变迁系统的当前状态
Get World Morphing Subsystem()
Get Current Climate Value() → Climate Value

// 根据气候值选择栖息地
Select Habitat From Climate(Climate Value) → Habitat

// 获取栖息地色谱
Get Habitat Spectrum(Habitat) → Spectrum

// 生成环境调色板
Generate Palette From Spectrum(Spectrum, 10) → Palette

// 应用到环境材质
Apply Palette To Environment(Palette)
```

### 示例3：动态调整怪兽颜色

```
// 获取基础调色板
Generate Palette From Ecology(Ecology Attributes) → Base Palette

// 根据战斗状态调整
If (Is Enraged)
  Adjust Palette For Rarity(Base Palette, Epic) → Adjusted Palette
Else
  Adjusted Palette = Base Palette

// 应用到怪兽
Set Monster Palette(Adjusted Palette)
```

## 8. 注意事项

1. **HSV色彩空间**：所有插值和调整都在HSV色彩空间进行，确保颜色过渡自然
2. **颜色数量**：`GeneratePaletteFromEcology`和`GenerateMonsterPalette`固定生成5种颜色
3. **饱和度范围**：调整后的饱和度和亮度会被限制在0-1范围内
4. **性能考虑**：调色板生成是轻量级操作，可以在运行时频繁调用
5. **扩展性**：栖息地色谱可以通过修改私有函数进行调整

## 9. 性能考虑

- **轻量级生成**：调色板生成是纯计算，无需加载资源，性能开销极小
- **批量生成**：可以在一帧内生成数百个调色板
- **缓存建议**：如果需要频繁访问相同怪兽的调色板，建议缓存结果

## 10. 调试技巧

1. **固定种子测试**：使用固定的种子值（如12345）进行测试，确保结果可重现
2. **打印颜色**：使用 `Print String` 节点打印生成的颜色值，验证生成逻辑
3. **可视化预览**：使用 `MonsterVisualizer` Actor 在编辑器中预览生成的调色板
4. **栖息地对比**：生成不同栖息地的调色板，观察色谱差异
5. **稀有度对比**：对同一调色板应用不同稀有度，观察颜色变化

## 11. 未来扩展方向

1. **LUT纹理生成**：将调色板转换为LUT纹理，用于材质着色
2. **渐变色支持**：支持生成渐变色调色板
3. **互补色算法**：根据主色自动生成互补色或对比色
4. **色彩和谐度检测**：检测生成的调色板是否和谐
5. **数据表驱动**：将栖息地色谱从代码移到数据表，便于美术调整

---

**版本历史**：
- **v2.0 (2025-12-08)**：添加怪兽属性驱动的调色板生成功能
- **v1.0 (2025-11-29)**：初始版本，基础色谱插值和单色调色板生成
