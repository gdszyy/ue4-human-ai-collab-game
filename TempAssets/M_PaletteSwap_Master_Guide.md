# M_PaletteSwap_Master 调色板材质创建指南

本文档提供详细的步骤指南，帮助您在UE5中创建调色板替换主材质。

---

## 1. 材质原理

### 1.1 什么是调色板替换？

调色板替换是一种技术，通过在运行时替换精灵中的特定颜色，实现动态改变精灵外观的效果。

**工作原理**：
1. 精灵使用特定的"标记颜色"（如纯红、纯绿、纯蓝）
2. 材质在运行时检测这些标记颜色
3. 将标记颜色替换为目标颜色
4. 其他颜色保持不变

**优势**：
- 一套精灵可以生成多种颜色变体
- 运行时动态改变颜色
- 节省美术资源

### 1.2 颜色通道定义

| 标记颜色 | RGB值 | 用途 | 示例 |
|---------|-------|------|------|
| **纯红** | (1, 0, 0) | 主色（身体） | 怪兽身体颜色 |
| **纯绿** | (0, 1, 0) | 次色（细节） | 怪兽细节颜色 |
| **纯蓝** | (0, 0, 1) | 强调色（高光） | 怪兽高光颜色 |

---

## 2. 精灵准备

### 2.1 精灵设计规范

在创建精灵时，需要使用标记颜色：

**示例：森林怪兽精灵**
```
- 身体部分：使用纯红色 (255, 0, 0)
- 细节部分：使用纯绿色 (0, 255, 0)
- 高光部分：使用纯蓝色 (0, 0, 255)
- 其他部分：使用黑色或其他颜色（不会被替换）
```

### 2.2 精灵创建步骤（Photoshop示例）

1. **创建新文档**
   - 尺寸：256×256 或 512×512
   - 背景：透明

2. **绘制身体**
   - 使用纯红色 (R=255, G=0, B=0)
   - 绘制怪兽的主体部分

3. **绘制细节**
   - 使用纯绿色 (R=0, G=255, B=0)
   - 绘制怪兽的细节部分（如斑点、条纹）

4. **绘制高光**
   - 使用纯蓝色 (R=0, G=0, B=255)
   - 绘制怪兽的高光部分（如眼睛、光泽）

5. **绘制其他部分**
   - 使用黑色或其他颜色
   - 绘制不需要替换的部分（如眼睛、轮廓）

6. **导出**
   - 格式：PNG
   - 保留透明通道

### 2.3 临时测试精灵

如果您暂时没有美术资源，可以使用我生成的临时测试精灵。这些精灵已经使用了纯色，可以直接测试调色板替换功能。

**注意**：临时测试精灵是纯色矩形，仅用于功能测试。正式版本需要替换为美术设计的精灵。

---

## 3. 材质创建步骤

### 3.1 创建材质

1. 在内容浏览器中右键 → Material
2. 命名为 `M_PaletteSwap_Master`
3. 保存到 `Content/Materials/Monsters/` 目录
4. 双击打开材质编辑器

### 3.2 材质设置

在材质编辑器的Details面板中设置：

```
Material Domain: Surface
Blend Mode: Translucent
Shading Model: Unlit
Two Sided: True
```

**说明**：
- **Translucent**：支持透明通道
- **Unlit**：不受光照影响（2D精灵）
- **Two Sided**：双面渲染

### 3.3 创建参数

在材质编辑器中创建以下参数：

#### 3.3.1 PrimaryColor（主色）

1. 右键 → Parameters → Vector Parameter
2. 命名为 `PrimaryColor`
3. 设置默认值：(0.2, 0.8, 0.2, 1.0)（森林绿）
4. 在Details面板中设置：
   - Parameter Group: Colors
   - Sort Priority: 0

#### 3.3.2 SecondaryColor（次色）

1. 右键 → Parameters → Vector Parameter
2. 命名为 `SecondaryColor`
3. 设置默认值：(0.4, 0.6, 0.3, 1.0)（深绿）
4. 在Details面板中设置：
   - Parameter Group: Colors
   - Sort Priority: 1

#### 3.3.3 AccentColor（强调色）

1. 右键 → Parameters → Vector Parameter
2. 命名为 `AccentColor`
3. 设置默认值：(0.8, 0.7, 0.3, 1.0)（黄色）
4. 在Details面板中设置：
   - Parameter Group: Colors
   - Sort Priority: 2

### 3.4 创建纹理采样

1. 右键 → Texture → Texture Sample
2. 在Details面板中设置：
   - Texture: None（将在运行时设置）
   - Sampler Type: Color
3. 将Texture Sample节点命名为 `BaseTexture`

### 3.5 创建颜色替换逻辑

#### 步骤1：提取RGB通道

```
[BaseTexture] → RGB
  ↓
[Component Mask]
  R: True
  G: False
  B: False
  → Red Channel

[BaseTexture] → RGB
  ↓
[Component Mask]
  R: False
  G: True
  B: False
  → Green Channel

[BaseTexture] → RGB
  ↓
[Component Mask]
  R: False
  G: False
  B: True
  → Blue Channel
```

#### 步骤2：检测标记颜色

**检测纯红色**：
```
[Red Channel] → Greater (>)
                 ↓ (0.9)
                 ↓
[Green Channel] → Less (<)
                  ↓ (0.1)
                  ↓
[Blue Channel] → Less (<)
                 ↓ (0.1)
                 ↓
[Multiply (AND)] → Is Red
```

**检测纯绿色**：
```
[Red Channel] → Less (<)
                ↓ (0.1)
                ↓
[Green Channel] → Greater (>)
                  ↓ (0.9)
                  ↓
[Blue Channel] → Less (<)
                 ↓ (0.1)
                 ↓
[Multiply (AND)] → Is Green
```

**检测纯蓝色**：
```
[Red Channel] → Less (<)
                ↓ (0.1)
                ↓
[Green Channel] → Less (<)
                  ↓ (0.1)
                  ↓
[Blue Channel] → Greater (>)
                 ↓ (0.9)
                 ↓
[Multiply (AND)] → Is Blue
```

#### 步骤3：替换颜色

```
[If]
  A: Is Red
  B: PrimaryColor
  C: [If]
       A: Is Green
       B: SecondaryColor
       C: [If]
            A: Is Blue
            B: AccentColor
            C: BaseTexture RGB
  → Final Color
```

**简化版本（使用Lerp）**：
```
[Lerp]
  A: BaseTexture RGB
  B: PrimaryColor
  Alpha: Is Red
  → Color After Red

[Lerp]
  A: Color After Red
  B: SecondaryColor
  Alpha: Is Green
  → Color After Green

[Lerp]
  A: Color After Green
  B: AccentColor
  Alpha: Is Blue
  → Final Color
```

#### 步骤4：保留透明通道

```
[BaseTexture] → A (Alpha)
  ↓
[Append]
  A: Final Color (RGB)
  B: BaseTexture Alpha
  → Final Color with Alpha
```

#### 步骤5：连接到输出

```
[Final Color with Alpha] → Emissive Color
[BaseTexture Alpha] → Opacity
```

---

## 4. 完整节点图

由于文本限制，这里提供完整的节点连接伪代码：

```
// 1. 纹理采样
TextureSample BaseTexture
  ↓ RGB → Extract RGB
  ↓ A → Extract Alpha

// 2. 提取通道
Extract RGB → ComponentMask(R) → Red Channel
Extract RGB → ComponentMask(G) → Green Channel
Extract RGB → ComponentMask(B) → Blue Channel

// 3. 检测标记颜色
Red Channel > 0.9 → Red High
Green Channel < 0.1 → Red Green Low
Blue Channel < 0.1 → Red Blue Low
Red High × Red Green Low × Red Blue Low → Is Red

Red Channel < 0.1 → Green Red Low
Green Channel > 0.9 → Green High
Blue Channel < 0.1 → Green Blue Low
Green Red Low × Green High × Green Blue Low → Is Green

Red Channel < 0.1 → Blue Red Low
Green Channel < 0.1 → Blue Green Low
Blue Channel > 0.9 → Blue High
Blue Red Low × Blue Green Low × Blue High → Is Blue

// 4. 替换颜色
Lerp(Extract RGB, PrimaryColor, Is Red) → Color1
Lerp(Color1, SecondaryColor, Is Green) → Color2
Lerp(Color2, AccentColor, Is Blue) → Final RGB

// 5. 保留透明通道
Append(Final RGB, Extract Alpha) → Final Color

// 6. 连接输出
Final Color → Emissive Color
Extract Alpha → Opacity
```

---

## 5. 测试材质

### 5.1 创建材质实例

1. 右键 M_PaletteSwap_Master → Create Material Instance
2. 命名为 `MI_PaletteSwap_Forest`
3. 打开材质实例
4. 在Parameters面板中设置：
   - PrimaryColor: (0.2, 0.8, 0.2, 1.0)（森林绿）
   - SecondaryColor: (0.4, 0.6, 0.3, 1.0)（深绿）
   - AccentColor: (0.8, 0.7, 0.3, 1.0)（黄色）

### 5.2 应用到精灵

1. 导入一个测试精灵到UE5
2. 创建Paper2D Sprite
3. 打开Sprite编辑器
4. 在Details面板中设置：
   - Material: MI_PaletteSwap_Forest
5. 保存Sprite

### 5.3 在关卡中测试

1. 将Sprite拖入关卡
2. 运行游戏（Play）
3. 检查颜色是否正确替换

---

## 6. 动态材质实例

在蓝图中创建动态材质实例：

```
[Create Dynamic Material Instance]
  Parent: M_PaletteSwap_Master
  → Dynamic Material

[Set Vector Parameter Value]
  Target: Dynamic Material
  Parameter Name: "PrimaryColor"
  Value: (0.9, 0.7, 0.3, 1.0)

[Set Vector Parameter Value]
  Target: Dynamic Material
  Parameter Name: "SecondaryColor"
  Value: (0.7, 0.5, 0.2, 1.0)

[Set Vector Parameter Value]
  Target: Dynamic Material
  Parameter Name: "AccentColor"
  Value: (1.0, 0.9, 0.7, 1.0)

[Set Material]
  Target: Sprite Component
  Element Index: 0
  Material: Dynamic Material
```

---

## 7. 常见问题

### 7.1 颜色没有被替换

**可能原因**：
- 精灵中的颜色不是纯色（RGB值不精确）
- 阈值设置不正确

**解决方案**：
- 确保精灵使用纯红(255,0,0)、纯绿(0,255,0)、纯蓝(0,0,255)
- 调整阈值（将0.9改为0.8，将0.1改为0.2）

### 7.2 透明通道丢失

**可能原因**：
- 没有正确连接Alpha通道
- Blend Mode设置不正确

**解决方案**：
- 确保BaseTexture的Alpha连接到Opacity
- 确保Blend Mode设置为Translucent

### 7.3 颜色不准确

**可能原因**：
- 参数值设置不正确
- 颜色空间问题

**解决方案**：
- 使用线性颜色空间（UE5默认）
- 确保参数值在0-1范围内

### 7.4 性能问题

**可能原因**：
- 材质过于复杂
- 使用了过多的If节点

**解决方案**：
- 使用Lerp代替If节点
- 简化材质逻辑

---

## 8. 优化建议

### 8.1 使用Lerp代替If

If节点会导致分支，影响性能。使用Lerp可以避免分支：

```
// 不推荐
If(Is Red, PrimaryColor, BaseTexture RGB)

// 推荐
Lerp(BaseTexture RGB, PrimaryColor, Is Red)
```

### 8.2 使用材质函数

将颜色替换逻辑封装为材质函数，方便复用：

1. 创建Material Function：`MF_ColorReplace`
2. 输入：BaseColor, TargetColor, Threshold
3. 输出：ReplacedColor
4. 在主材质中调用材质函数

### 8.3 使用材质参数集合

如果有多个怪兽使用相同的调色板，可以使用Material Parameter Collection：

1. 创建Material Parameter Collection：`MPC_MonsterPalettes`
2. 添加参数：Forest_Primary, Forest_Secondary等
3. 在材质中引用MPC参数
4. 在蓝图中统一修改MPC参数

---

## 9. 高级功能

### 9.1 支持更多颜色通道

如果需要支持更多颜色通道（如4个、5个），可以使用以下方法：

**方法1：使用Alpha通道**
- 红色 (1, 0, 0, 1)：主色
- 绿色 (0, 1, 0, 1)：次色
- 蓝色 (0, 0, 1, 1)：强调色
- 黄色 (1, 1, 0, 1)：第四色

**方法2：使用多张纹理**
- BaseTexture：RGB三个通道
- MaskTexture：额外的颜色通道

### 9.2 支持渐变色

如果需要支持渐变色，可以使用以下方法：

```
[BaseTexture Red Channel] → Lerp
  A: PrimaryColor
  B: SecondaryColor
  Alpha: BaseTexture Red Channel
  → Gradient Color
```

### 9.3 支持色相偏移

如果需要支持色相偏移（Hue Shift），可以使用以下方法：

```
[BaseTexture RGB] → RGB to HSV
  ↓ H, S, V

[H + Hue Offset] → New H

[HSV to RGB]
  H: New H
  S: S
  V: V
  → Shifted Color
```

---

## 10. 下一步

完成材质创建后，您可以：

1. **测试材质**
   - 创建测试精灵
   - 应用材质
   - 调整参数

2. **创建材质实例**
   - 为每个栖息地创建材质实例
   - 设置不同的颜色参数

3. **集成到蓝图**
   - 在MonsterActor中使用动态材质实例
   - 根据栖息地动态改变颜色

4. **优化性能**
   - 使用材质函数
   - 使用材质参数集合

祝您创建顺利！
