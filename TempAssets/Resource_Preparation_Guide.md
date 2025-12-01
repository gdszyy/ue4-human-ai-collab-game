# VOI-11 资源准备完整指南

本文档提供VOI-11怪兽生成系统的完整资源准备指南，从零开始帮助您准备所有必需的资源。

---

## 📋 目录

1. [快速开始](#1-快速开始)
2. [精灵资源准备](#2-精灵资源准备)
3. [Flipbook动画创建](#3-flipbook动画创建)
4. [调色板材质创建](#4-调色板材质创建)
5. [数据表配置](#5-数据表配置)
6. [蓝图创建](#6-蓝图创建)
7. [测试验证](#7-测试验证)
8. [常见问题](#8-常见问题)
9. [资源清单](#9-资源清单)

---

## 1. 快速开始

### 1.1 两种实现路径

您可以选择以下两种路径之一：

#### 路径A：快速原型测试（推荐新手）

使用临时测试资源快速验证系统功能，然后逐步替换为正式资源。

**优势**：
- ✅ 快速验证系统功能
- ✅ 无需美术资源
- ✅ 降低学习曲线

**步骤**：
1. 使用我生成的临时测试精灵（30个骨架 + 8个部件）
2. 导入CSV数据表模板
3. 创建简单的Flipbook（单帧）
4. 创建调色板材质
5. 创建BP_MonsterActor蓝图
6. 测试系统功能
7. 逐步替换为正式资源

**预计时间**：2-4小时

#### 路径B：直接准备正式资源（推荐有经验的开发者）

直接准备正式的美术资源和完整的动画。

**优势**：
- ✅ 一步到位
- ✅ 最终效果更好
- ✅ 无需二次替换

**步骤**：
1. 设计或委托美术设计精灵资源
2. 创建完整的Flipbook动画（多帧）
3. 创建调色板材质
4. 配置数据表
5. 创建BP_MonsterActor蓝图
6. 测试系统功能

**预计时间**：1-2周（取决于美术资源准备时间）

### 1.2 推荐路径

**如果您是第一次使用VOI-11系统**，强烈推荐选择**路径A（快速原型测试）**：

1. 先使用临时资源验证系统功能
2. 熟悉整个工作流程
3. 确认系统符合需求
4. 再投入时间准备正式资源

---

## 2. 精灵资源准备

### 2.1 精灵规格

#### 2.1.1 技术规格

| 属性 | 要求 | 说明 |
|------|------|------|
| **格式** | PNG | 支持透明通道 |
| **分辨率** | 256×256 或 512×512 | 根据游戏需求选择 |
| **背景** | 透明 | Alpha通道 |
| **枢轴点** | 居中或底部中心 | 统一所有精灵 |
| **颜色空间** | sRGB | UE4默认 |
| **位深度** | 32位（RGBA） | 包含Alpha通道 |

#### 2.1.2 命名规范

**骨架精灵**：
```
S_<Habitat>_<SizeClass>_Body.png

示例：
S_Forest_Medium_Body.png
S_Desert_Large_Body.png
S_Tundra_Small_Body.png
```

**部件精灵**：
```
S_<PartName>.png

示例：
S_FireWings.png
S_RockArmor.png
S_IceHorns.png
```

### 2.2 骨架精灵设计

#### 2.2.1 设计要求

每个骨架精灵需要满足以下要求：

1. **清晰的轮廓**：怪兽的整体形状清晰可辨
2. **统一的风格**：所有骨架使用相同的美术风格
3. **适当的细节**：细节不要过多，避免影响调色板替换
4. **标记颜色**：使用纯红、纯绿、纯蓝作为标记颜色

#### 2.2.2 栖息地特征

每个栖息地的骨架应该有独特的特征：

| 栖息地 | 特征 | 示例 |
|--------|------|------|
| **Forest** | 植物特征、柔和曲线 | 树叶状耳朵、藤蔓尾巴 |
| **Desert** | 爬行动物特征、尖锐线条 | 鳞片、尖刺 |
| **Tundra** | 厚重皮毛、圆润形状 | 毛茸茸、圆耳朵 |
| **Swamp** | 两栖动物特征、湿润质感 | 光滑皮肤、蹼足 |
| **Mountain** | 岩石质感、坚硬线条 | 棱角分明、厚重 |
| **Ocean** | 水生动物特征、流线型 | 鳍、流线型身体 |

#### 2.2.3 体型差异

不同体型的骨架应该有明显的大小和比例差异：

| 体型 | 相对大小 | 比例特征 |
|------|----------|----------|
| **Tiny** | 40% | 头大身小、可爱 |
| **Small** | 60% | 匀称、灵活 |
| **Medium** | 80% | 标准比例 |
| **Large** | 100% | 强壮、厚重 |
| **Giant** | 100% | 巨大、威猛 |

**注意**：体型的实际大小由代码控制，精灵本身的大小差异主要体现在比例上。

### 2.3 部件精灵设计

#### 2.3.1 设计要求

每个部件精灵需要满足以下要求：

1. **独立性**：可以单独显示，不依赖骨架
2. **兼容性**：可以叠加到任何骨架上
3. **清晰的功能**：一眼就能看出部件的作用
4. **适当的大小**：不要过大或过小

#### 2.3.2 战斗词条对应

每个战斗词条应该有对应的视觉部件：

| 战斗词条 | 部件名称 | 视觉特征 | 示例 |
|---------|---------|---------|------|
| **FireAffinity** | FireWings, FireTail | 火焰形状、橙红色 | 燃烧的翅膀 |
| **StoneShell** | RockArmor | 岩石质感、灰色 | 岩石外壳 |
| **IceBreath** | IceHorns | 冰晶形状、蓝色 | 冰霜之角 |
| **PoisonSpit** | PoisonSpikes | 尖刺、绿色 | 毒刺 |
| **LightningStrike** | LightningBolt | 闪电形状、黄色 | 闪电标记 |
| **ShadowMeld** | ShadowCloak | 半透明、深灰色 | 暗影斗篷 |
| **Regeneration** | HealingAura | 光环、粉色 | 治愈光环 |
| **Berserk** | BerserkMark | 狂暴符号、红色 | 狂暴标记 |

### 2.4 调色板颜色设计

#### 2.4.1 精灵中的标记颜色

在精灵中使用以下标记颜色：

| 标记颜色 | RGB值 | 用途 | 占比建议 |
|---------|-------|------|---------|
| **纯红** | (255, 0, 0) | 主色（身体） | 60-70% |
| **纯绿** | (0, 255, 0) | 次色（细节） | 20-30% |
| **纯蓝** | (0, 0, 255) | 强调色（高光） | 5-10% |
| **其他** | 任意颜色 | 不替换的部分 | 5-10% |

#### 2.4.2 栖息地调色板

每个栖息地应该有独特的调色板：

| 栖息地 | 主色 | 次色 | 强调色 | 整体感觉 |
|--------|------|------|--------|---------|
| **Forest** | 森林绿 | 深绿 | 黄绿 | 自然、生机 |
| **Desert** | 沙黄 | 土黄 | 浅黄 | 干燥、炎热 |
| **Tundra** | 冰蓝 | 深蓝 | 白色 | 寒冷、纯净 |
| **Swamp** | 沼泽绿 | 褐绿 | 黄绿 | 潮湿、阴暗 |
| **Mountain** | 岩石灰 | 深灰 | 浅灰 | 坚硬、稳重 |
| **Ocean** | 海蓝 | 深蓝 | 浅蓝 | 深邃、神秘 |

### 2.5 临时测试精灵使用

如果您选择快速原型路径，可以直接使用我生成的临时测试精灵。

#### 2.5.1 导入步骤

1. 在UE4中打开项目
2. 在内容浏览器中创建文件夹：`Content/Textures/Monsters/Temp/`
3. 将临时精灵拖入UE4编辑器
4. 确认导入设置：
   - Texture Group: 2D Pixels (Unfiltered)
   - Compression Settings: UserInterface2D (RGBA)
   - MipGen Settings: NoMipmaps

#### 2.5.2 创建Paper2D精灵

1. 右键纹理 → Sprite Actions → Create Sprite
2. 打开Sprite编辑器
3. 在Details面板中设置：
   - Pixels Per Unit: 1.0
   - Pivot Mode: Center_Center（或Bottom_Center）
4. 保存Sprite

---

## 3. Flipbook动画创建

### 3.1 动画类型

每个骨架需要创建4种动画：

| 动画类型 | 用途 | 帧数建议 | 循环 | 帧率建议 |
|---------|------|---------|------|---------|
| **Idle** | 待机 | 4-8帧 | 是 | 12 FPS |
| **Walk** | 行走 | 6-12帧 | 是 | 18 FPS |
| **Attack** | 攻击 | 8-16帧 | 否 | 24 FPS |
| **Death** | 死亡 | 8-12帧 | 否 | 18 FPS |

### 3.2 创建步骤

#### 3.2.1 准备序列帧

1. 在图像编辑软件中创建动画序列帧
2. 每帧保存为独立的PNG文件
3. 命名规范：
   ```
   S_Forest_Medium_Idle_001.png
   S_Forest_Medium_Idle_002.png
   S_Forest_Medium_Idle_003.png
   ...
   ```

#### 3.2.2 导入到UE4

1. 将所有序列帧拖入UE4
2. 为每帧创建Sprite（右键 → Create Sprite）

#### 3.2.3 创建Flipbook

1. 选中所有Sprite
2. 右键 → Create Flipbook
3. 命名为 `FB_Forest_Medium_Idle`
4. 打开Flipbook编辑器
5. 在Details面板中设置：
   - Frames Per Second: 12（根据动画类型调整）
   - Loop: True（Idle和Walk）或False（Attack和Death）

### 3.3 快速原型：单帧Flipbook

如果您选择快速原型路径，可以创建单帧Flipbook：

1. 为每个骨架创建一个Sprite
2. 右键Sprite → Create Flipbook
3. Flipbook只包含一帧
4. 所有动画类型使用同一个Flipbook

**优势**：
- 快速创建
- 无需动画制作
- 可以验证系统功能

**劣势**：
- 没有动画效果
- 需要后续替换

---

## 4. 调色板材质创建

详细步骤请参考 `M_PaletteSwap_Master_Guide.md`。

### 4.1 快速步骤

1. 创建材质：`M_PaletteSwap_Master`
2. 设置材质属性：
   - Blend Mode: Translucent
   - Shading Model: Unlit
   - Two Sided: True
3. 创建参数：
   - PrimaryColor (Vector)
   - SecondaryColor (Vector)
   - AccentColor (Vector)
4. 创建纹理采样：BaseTexture
5. 实现颜色替换逻辑
6. 连接到输出节点

### 4.2 测试材质

1. 创建材质实例：`MI_PaletteSwap_Forest`
2. 设置颜色参数
3. 应用到测试精灵
4. 在关卡中验证效果

---

## 5. 数据表配置

### 5.1 导入CSV模板

我已经为您创建了3个CSV模板：

1. `DT_Skeletons.csv`（骨架数据表）
2. `DT_Parts.csv`（部件数据表）
3. `DT_Palettes.csv`（调色板数据表）

#### 5.1.1 导入步骤

1. 在内容浏览器中右键 → Miscellaneous → Data Table
2. 选择行结构：
   - DT_Skeletons → FSkeletonData
   - DT_Parts → FPartData
   - DT_Palettes → FPaletteData
3. 点击"Import from CSV"
4. 选择对应的CSV文件
5. 保存数据表

### 5.2 填写资源引用

导入后，需要手动填写资源引用：

#### 5.2.1 DT_Skeletons

打开DT_Skeletons，为每一行填写：

```
Idle Flipbook: FB_Forest_Medium_Idle
Walk Flipbook: FB_Forest_Medium_Walk
Attack Flipbook: FB_Forest_Medium_Attack
Death Flipbook: FB_Forest_Medium_Death
```

**快速原型**：所有Flipbook字段都填写同一个单帧Flipbook。

#### 5.2.2 DT_Parts

打开DT_Parts，为每一行填写：

```
Part Sprite: S_FireWings
```

#### 5.2.3 DT_Palettes

DT_Palettes不需要填写资源引用，颜色值已经在CSV中设置。

### 5.3 验证数据表

1. 打开每个数据表
2. 检查所有行是否正确填写
3. 检查资源引用是否有效（没有红色感叹号）

---

## 6. 蓝图创建

详细步骤请参考 `BP_MonsterActor_Pseudocode.md`。

### 6.1 创建BP_MonsterActor

1. 在内容浏览器中右键 → Blueprint Class
2. 选择 `MonsterActor` 作为父类
3. 命名为 `BP_MonsterActor`
4. 保存到 `Content/Blueprints/Monsters/`

### 6.2 设置Class Defaults

打开BP_MonsterActor，在Class Defaults面板中设置：

```
Skeleton Data Table: DT_Skeletons
Part Data Table: DT_Parts
Palette Data Table: DT_Palettes
Palette Swap Material: M_PaletteSwap_Master
```

### 6.3 添加测试函数

在Event Graph中添加测试函数：

1. Test Generate Monster
2. Change Habitat
3. Play Random Animation

详细实现请参考 `BP_MonsterActor_Pseudocode.md`。

---

## 7. 测试验证

### 7.1 单元测试

1. 打开UE4编辑器
2. 菜单栏 → Window → Test Automation
3. 找到PCG类别
4. 运行所有测试
5. 确保所有测试通过

### 7.2 集成测试

#### 7.2.1 测试1：生成单个怪兽

1. 将BP_MonsterActor拖入关卡
2. 在Details面板中调用"Test Generate Monster"
3. 检查怪兽是否正确生成

#### 7.2.2 测试2：批量生成怪兽

1. 创建BP_MonsterSpawnerTest（参考伪代码）
2. 将其拖入关卡
3. 运行游戏
4. 检查是否生成100个怪兽

#### 7.2.3 测试3：动态调整

1. 创建BP_MonsterDynamicTest（参考伪代码）
2. 将其拖入关卡
3. 运行游戏
4. 检查怪兽颜色是否每2秒变化

#### 7.2.4 测试4：动画播放

1. 创建BP_MonsterAnimationTest（参考伪代码）
2. 将其拖入关卡
3. 运行游戏
4. 检查怪兽动画是否每1.5秒变化

### 7.3 性能测试

1. 生成100个怪兽
2. 打开Stat FPS
3. 检查帧率是否稳定
4. 如果帧率过低，考虑优化

---

## 8. 常见问题

### 8.1 精灵相关

#### Q1: 精灵导入后显示模糊

**解决方案**：
1. 选中纹理
2. 在Details面板中设置：
   - Texture Group: 2D Pixels (Unfiltered)
   - Filter: Nearest
3. 保存并重新编译

#### Q2: 精灵的枢轴点不统一

**解决方案**：
1. 打开Sprite编辑器
2. 在Details面板中设置：
   - Pivot Mode: Custom
   - Custom Pivot Point: (0.5, 0.5)（居中）或(0.5, 1.0)（底部中心）
3. 对所有Sprite应用相同设置

### 8.2 Flipbook相关

#### Q3: Flipbook不播放

**解决方案**：
1. 打开Flipbook编辑器
2. 检查Frames Per Second是否大于0
3. 检查是否有多帧（至少2帧才能看到动画效果）
4. 在蓝图中调用Play()函数

#### Q4: Flipbook播放速度不对

**解决方案**：
1. 打开Flipbook编辑器
2. 调整Frames Per Second
3. 建议值：Idle=12, Walk=18, Attack=24, Death=18

### 8.3 材质相关

#### Q5: 调色板材质不工作

**解决方案**：
1. 检查精灵是否使用了纯红、纯绿、纯蓝
2. 检查材质的Blend Mode是否为Translucent
3. 检查材质参数是否正确设置
4. 使用Print String输出调试信息

#### Q6: 透明通道丢失

**解决方案**：
1. 检查材质的Opacity是否连接了Alpha通道
2. 检查Blend Mode是否为Translucent
3. 检查纹理的Compression Settings是否为UserInterface2D (RGBA)

### 8.4 数据表相关

#### Q7: 数据表导入失败

**解决方案**：
1. 检查CSV文件的编码是否为UTF-8
2. 检查CSV文件的格式是否正确（逗号分隔）
3. 检查行结构是否选择正确

#### Q8: 资源引用无效

**解决方案**：
1. 检查资源路径是否正确
2. 确保资源已经导入到项目中
3. 尝试手动选择资源（而不是输入路径）

### 8.5 蓝图相关

#### Q9: 怪兽没有显示

**解决方案**：
1. 检查BP_MonsterActor的Class Defaults是否设置了数据表引用
2. 检查是否调用了Set Monster Attributes
3. 使用Print String输出调试信息
4. 检查怪兽是否在相机视野内

#### Q10: 怪兽颜色不对

**解决方案**：
1. 检查DT_Palettes中的颜色值
2. 检查材质是否正确应用
3. 使用Print String输出当前调色板数据

---

## 9. 资源清单

### 9.1 快速原型路径

#### 阶段1：基础功能验证（2-4小时）

- [ ] 导入临时测试精灵（38个）
- [ ] 创建Paper2D Sprite（38个）
- [ ] 创建单帧Flipbook（30个）
- [ ] 创建调色板材质（1个）
- [ ] 导入数据表CSV（3个）
- [ ] 填写数据表资源引用
- [ ] 创建BP_MonsterActor蓝图
- [ ] 测试单个怪兽生成
- [ ] 测试批量生成
- [ ] 测试动态调整

#### 阶段2：替换正式资源（1-2周）

- [ ] 设计或委托美术设计精灵（38个）
- [ ] 创建多帧Flipbook动画（120个）
- [ ] 更新数据表资源引用
- [ ] 测试所有功能
- [ ] 优化性能

### 9.2 直接准备正式资源路径

#### 阶段1：美术资源准备（1-2周）

- [ ] 设计骨架精灵（30个）
  - [ ] Forest（6个体型）
  - [ ] Desert（6个体型）
  - [ ] Tundra（6个体型）
  - [ ] Swamp（6个体型）
  - [ ] Mountain（6个体型）
  - [ ] Ocean（6个体型）
- [ ] 设计部件精灵（8个）
  - [ ] FireWings, FireTail
  - [ ] RockArmor
  - [ ] IceHorns
  - [ ] PoisonSpikes
  - [ ] LightningBolt
  - [ ] ShadowCloak
  - [ ] HealingAura

#### 阶段2：动画创建（3-5天）

- [ ] 创建Idle动画（30个）
- [ ] 创建Walk动画（30个）
- [ ] 创建Attack动画（30个）
- [ ] 创建Death动画（30个）

#### 阶段3：UE4集成（1-2天）

- [ ] 导入所有精灵到UE4
- [ ] 创建所有Flipbook
- [ ] 创建调色板材质
- [ ] 配置数据表
- [ ] 创建BP_MonsterActor蓝图
- [ ] 测试所有功能

---

## 10. 时间估算

### 10.1 快速原型路径

| 任务 | 预计时间 | 说明 |
|------|---------|------|
| 导入临时精灵 | 30分钟 | 批量导入 |
| 创建单帧Flipbook | 1小时 | 30个Flipbook |
| 创建调色板材质 | 1小时 | 按照指南创建 |
| 配置数据表 | 30分钟 | 导入CSV并填写引用 |
| 创建BP_MonsterActor | 30分钟 | 设置Class Defaults |
| 测试验证 | 30分钟 | 运行所有测试 |
| **总计** | **4小时** | 一个下午即可完成 |

### 10.2 正式资源路径

| 任务 | 预计时间 | 说明 |
|------|---------|------|
| 设计骨架精灵 | 3-5天 | 取决于美术能力 |
| 设计部件精灵 | 1-2天 | 取决于美术能力 |
| 创建动画 | 3-5天 | 120个Flipbook |
| 创建调色板材质 | 1小时 | 按照指南创建 |
| 配置数据表 | 1小时 | 填写所有资源引用 |
| 创建BP_MonsterActor | 30分钟 | 设置Class Defaults |
| 测试验证 | 1小时 | 运行所有测试 |
| **总计** | **1-2周** | 取决于美术资源准备时间 |

---

## 11. 下一步

完成资源准备后，您可以：

1. **集成世界变迁系统**
   - 从VOI-6获取世界状态
   - 根据气候和地势生成怪兽

2. **实现怪兽AI**
   - 创建AI控制器
   - 实现巡逻、追击、攻击行为

3. **集成战斗系统**
   - 与VOI-18战斗系统集成
   - 实现伤害处理和死亡逻辑

4. **优化性能**
   - 实现对象池
   - 优化渲染
   - 限制同时显示的怪兽数量

---

## 12. 支持与帮助

如果在资源准备过程中遇到问题，可以：

1. **查阅文档**
   - MHP_VOI-11_Monster_Generation_System.md
   - BP_MonsterActor_Pseudocode.md
   - M_PaletteSwap_Master_Guide.md

2. **查看代码注释**
   - 所有C++代码都有完整的注释

3. **运行单元测试**
   - 确保核心模块正常工作

4. **使用临时资源**
   - 先用临时资源验证功能
   - 再逐步替换为正式资源

---

祝您资源准备顺利！🎉
