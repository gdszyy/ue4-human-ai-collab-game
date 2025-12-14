# 蓝图使用指南

**版本**: 1.0  
**日期**: 2025-11-29  
**适用项目**: 《回声炼金师》(EchoAlchemist)

---

## 概述

本文档说明如何在蓝图中使用 Manus 实现的 C++ 模块。

**核心理念**：您只需要拖拽**一个节点**即可完成功能，无需手动连接复杂的蓝图逻辑。

---

## 快速开始

### 第一步：拉取并编译代码

```bash
cd ue5-human-ai-collab-game
git pull origin main
```

然后在 Visual Studio 中编译项目（或让 UE5 自动编译）。

### 第二步：创建测试蓝图

1. 在 UE5 编辑器中，打开 Content Browser
2. 右键 -> Blueprint Class -> Actor
3. 命名为 `BP_AlchemyTest`
4. 双击打开蓝图编辑器

### 第三步：添加测试节点

在 Event Graph 中：

1. 找到 `Event BeginPlay` 节点
2. 右键 -> 搜索 "Test Alchemy System"
3. 拖拽 `Test Alchemy System` 节点
4. 连接 `Event BeginPlay` 到 `Test Alchemy System`
5. 将 `Self` 连接到 `World Context Object` 引脚

完整的蓝图应该是这样的：

```
Event BeginPlay
  ↓
Test Alchemy System
  - World Context Object: Self
```

**就这么简单！只需要一个节点！**

### 第四步：测试

1. 将 `BP_AlchemyTest` 拖入关卡
2. 点击 Play 按钮
3. 打开 Output Log（Window -> Developer Tools -> Output Log）
4. 查看测试结果

您应该会看到类似这样的输出：

```
LogTemp: ========================================
LogTemp: === 炼金术系统测试开始 ===
LogTemp: ========================================
LogTemp: AlchemySystem: Successfully combined Fire + Water = Steam (Power: 15)
LogTemp: ✅ 测试 1 通过: Fire + Water = Steam (Type: Hybrid, Power: 15)
LogTemp: AlchemySystem: Successfully combined Earth + Water = Mud (Power: 12)
LogTemp: ✅ 测试 2 通过: Earth + Water = Mud (Type: Hybrid, Power: 12)
LogTemp: AlchemySystem: Successfully combined Fire + Earth = Lava (Power: 18)
LogTemp: ✅ 测试 3 通过: Fire + Earth = Lava (Type: Hybrid, Power: 18)
LogTemp: AlchemySystem: Successfully combined Air + Water = Cloud (Power: 13)
LogTemp: ✅ 测试 4 通过: Air + Water = Cloud (Type: Hybrid, Power: 13)
LogTemp: AlchemySystem: No recipe found for Fire + Fire
LogTemp: ✅ 测试 5 通过: Fire + Fire 正确返回失败（无配方）
LogTemp: ========================================
LogTemp: === 炼金术系统测试结束 ===
LogTemp: ========================================
```

---

## 可用的蓝图函数

### 1. Test Alchemy System（一键测试）

**功能**：自动运行所有测试用例并输出结果

**蓝图使用**：
```
Event BeginPlay
  ↓
Test Alchemy System
  - World Context Object: Self
```

**输出**：在 Output Log 中显示所有测试结果

---

### 2. Quick Combine（快速合成）

**功能**：合成两个元素并输出详细日志

**蓝图使用**：
```
Event BeginPlay
  ↓
Quick Combine
  - World Context Object: Self
  - Element1: "Fire"
  - Element2: "Water"
  ↓
Print String: {Result.Name}
```

**返回值**：`FElement` 结构体（包含 Name、Type、Power）

**示例输出**：
```
LogTemp: ✅ Fire + Water = Steam (Type: Hybrid, Power: 15)
```

---

### 3. Get All Elements With Log（获取所有元素）

**功能**：获取所有可用元素并输出到日志

**蓝图使用**：
```
Event BeginPlay
  ↓
Get All Elements With Log
  - World Context Object: Self
  ↓
ForEachLoop
  ↓
Print String: {Element.Name}
```

**返回值**：`TArray<FElement>`

**示例输出**：
```
LogTemp: ========================================
LogTemp: === 所有元素 (4) ===
LogTemp: - Fire (Type: Fire, Power: 10)
LogTemp: - Water (Type: Water, Power: 10)
LogTemp: - Earth (Type: Earth, Power: 10)
LogTemp: - Air (Type: Air, Power: 10)
LogTemp: ========================================
```

---

### 4. Get All Recipes With Log（获取所有配方）

**功能**：获取所有配方并输出到日志

**蓝图使用**：
```
Event BeginPlay
  ↓
Get All Recipes With Log
  - World Context Object: Self
  ↓
ForEachLoop
  ↓
Print String: {Recipe.Result.Name}
```

**返回值**：`TArray<FRecipe>`

**示例输出**：
```
LogTemp: ========================================
LogTemp: === 所有配方 (4) ===
LogTemp: - Fire + Water = Steam (Power: 15)
LogTemp: - Earth + Water = Mud (Power: 12)
LogTemp: - Fire + Earth = Lava (Power: 18)
LogTemp: - Air + Water = Cloud (Power: 13)
LogTemp: ========================================
```

---

## 常见问题

### Q: 为什么我找不到这些节点？

**A**: 确保您已经编译了 C++ 代码。如果还是找不到，尝试：
1. 关闭 UE5 编辑器
2. 在 Visual Studio 中重新编译
3. 重新打开 UE5 编辑器

### Q: 为什么 Output Log 没有输出？

**A**: 确保您已经：
1. 将 `BP_AlchemyTest` 拖入关卡
2. 点击了 Play 按钮
3. 打开了 Output Log 窗口（Window -> Developer Tools -> Output Log）

### Q: 我可以在蓝图中直接调用 UAlchemySystem 吗？

**A**: 可以，但不推荐。使用蓝图函数库封装的函数更简单：

❌ **不推荐**（需要 5+ 个节点）：
```
Event BeginPlay
  ↓
Get Game Instance
  ↓
Get Subsystem (AlchemySystem)
  ↓
Combine Elements
  - Element1: "Fire"
  - Element2: "Water"
  ↓
Branch (Success?)
  ↓
Print String
```

✅ **推荐**（只需要 1 个节点）：
```
Event BeginPlay
  ↓
Quick Combine
  - World Context Object: Self
  - Element1: "Fire"
  - Element2: "Water"
```

### Q: 如何添加新的元素或配方？

**A**: 告诉 Manus 您需要的元素和配方，Manus 会在 C++ 中添加并提交到 Git。您只需要 `git pull` 并重新编译即可。

---

## 反馈

如果您发现任何问题或有改进建议，请：

1. 复制 Output Log 中的错误信息
2. 在 Linear 任务中添加评论
3. 或者直接在 GitHub 上创建 Issue

Manus 会尽快修复问题并更新代码。

---

## 下一步

现在您已经知道如何使用炼金术系统，可以尝试：

1. **创建自己的测试蓝图**
2. **在游戏中集成炼金术系统**
3. **添加 UI 来显示元素和配方**
4. **等待 Manus 实现更多核心模块**（如弹珠物理系统、砖块系统等）

所有新模块都会遵循相同的封装模式，您只需要拖拽一个节点即可使用！

---

**开始创建您的炼金术游戏吧！** 🎮✨
