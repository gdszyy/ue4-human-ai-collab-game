# C++ 核心模块 + 蓝图连接开发模式

**版本**: 1.0  
**日期**: 2025-11-29  
**适用项目**: 《回声炼金师》(EchoAlchemist)

---

## 概述

本文档定义了《回声炼金师》项目采用的核心开发模式：**Manus 实现 C++ 核心模块 + 用户通过蓝图连接**。

这是 UE4 开发的最佳实践之一，也是最适合人机协同开发的模式。

---

## 核心原则

### 职责分工

| 角色 | 职责 | 工具 |
|------|------|------|
| **Manus** | C++ 核心逻辑实现 | Git, C++, Python |
| **用户** | 蓝图连接和可视化内容 | UE4 Editor, Blueprint, Git |

### 工作流程

```
Manus (远程)                    GitHub                    用户 (本地)
    │                             │                          │
    ├─ 1. 实现 C++ 核心模块 ────────>│                          │
    ├─ 2. 编写单元测试 ──────────────>│                          │
    ├─ 3. 编写使用文档 ──────────────>│                          │
    ├─ 4. 提交到 Git ───────────────>│                          │
    │                             │                          │
    │                             │<──── 5. Git pull ────────┤
    │                             │                          │
    │                             │      6. 编译 C++ 代码 ───┤
    │                             │                          │
    │                             │      7. 在蓝图中连接 ────┤
    │                             │                          │
    │                             │      8. 测试功能 ────────┤
    │                             │                          │
    │                             │<──── 9. 反馈结果 ────────┤
    │<──────────────────────────────── 10. 迭代优化 ─────────┤
```

---

## Manus 的工作方式

### 1. C++ 核心模块实现

#### 设计原则

- ✅ **暴露接口给蓝图**：使用 `UFUNCTION(BlueprintCallable)` 和 `UPROPERTY(BlueprintReadWrite)`
- ✅ **清晰的命名**：函数和变量名应该语义明确
- ✅ **完整的注释**：每个公开接口都应该有详细注释
- ✅ **性能优先**：核心逻辑用 C++ 实现以保证性能
- ✅ **模块化设计**：每个系统独立，便于测试和维护

#### 示例：炼金术系统

```cpp
// Source/EchoAlchemist/Public/AlchemySystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AlchemySystem.generated.h"

/**
 * 元素结构体
 * 用于表示炼金术系统中的基础元素
 */
USTRUCT(BlueprintType)
struct FElement
{
    GENERATED_BODY()
    
    /** 元素名称 */
    UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
    FString Name;
    
    /** 元素类型 (Fire, Water, Earth, Air) */
    UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
    FString Type;
    
    /** 元素强度 */
    UPROPERTY(BlueprintReadWrite, Category = "Alchemy")
    int32 Power;
};

/**
 * 炼金术系统
 * 负责管理元素合成逻辑
 */
UCLASS()
class ECHOALCHEMIST_API UAlchemySystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
public:
    /**
     * 合成两个元素
     * @param Element1 第一个元素的名称
     * @param Element2 第二个元素的名称
     * @param OutResult 合成结果（如果成功）
     * @return 是否合成成功
     */
    UFUNCTION(BlueprintCallable, Category = "Alchemy")
    bool CombineElements(const FString& Element1, const FString& Element2, FElement& OutResult);
    
    /**
     * 获取所有可用元素
     * @return 元素数组
     */
    UFUNCTION(BlueprintPure, Category = "Alchemy")
    TArray<FElement> GetAllElements() const;
    
protected:
    /** 元素数据库（内部使用，不暴露给蓝图） */
    UPROPERTY()
    TMap<FString, FElement> ElementDatabase;
};
```

### 2. 提交规范

#### Commit Message 格式

```
<type>: <subject>

<body>

Blueprint Usage:
<蓝图使用说明>

Ref: <任务ID>
```

**示例**：

```
feat: Implement Alchemy System core module

- Add FElement and FRecipe structs
- Add UAlchemySystem subsystem
- Implement element combination logic
- Expose functions to Blueprint

Blueprint Usage:
1. Get Alchemy System: GetGameInstance -> GetSubsystem(AlchemySystem)
2. Combine Elements: CombineElements(Element1, Element2, Result)
3. Get All Elements: GetAllElements()

Ref: VOI-18
```

### 3. 文档编写

每个核心模块都应该包含：

#### 模块文档

创建 `Docs/{ModuleName}_module.md`，包含：
- 模块概述
- 核心功能
- 蓝图使用示例
- API 参考
- 注意事项

#### 代码注释

```cpp
/**
 * 合成两个元素
 * 
 * 根据配方数据库查找对应的合成结果。
 * 如果找不到配方，返回 false。
 * 
 * @param Element1 第一个元素的名称（如 "Fire"）
 * @param Element2 第二个元素的名称（如 "Water"）
 * @param OutResult 合成结果，只有在返回 true 时有效
 * @return 是否合成成功
 * 
 * 蓝图示例：
 * CombineElements("Fire", "Water", Result) -> Result.Name = "Steam"
 */
UFUNCTION(BlueprintCallable, Category = "Alchemy")
bool CombineElements(const FString& Element1, const FString& Element2, FElement& OutResult);
```

---

## 用户的工作方式

### 1. 拉取并编译 C++ 代码

#### 步骤

```bash
# 1. 拉取最新代码
git pull origin main

# 2. 关闭 UE4 编辑器（如果打开）

# 3. 右键 EchoAlchemist.uproject -> Generate Visual Studio project files

# 4. 打开 EchoAlchemist.sln

# 5. 在 Visual Studio 中编译（F7）

# 6. 编译成功后，打开 UE4 编辑器
```

### 2. 在蓝图中使用 C++ 模块

#### 示例：使用炼金术系统

**创建蓝图**：`BP_AlchemyTest`

**Event Graph**：

```
Event BeginPlay
  ↓
Get Game Instance
  ↓
Get Subsystem (Class: AlchemySystem)
  ↓
Combine Elements
  - Element1: "Fire"
  - Element2: "Water"
  - Result: (output variable)
  ↓
Branch (Success?)
  ↓ True
  Print String: "Combined: {Result.Name}, Power: {Result.Power}"
  ↓ False
  Print String: "Combination failed"
```

### 3. 测试并反馈

#### 测试清单

- ✅ 功能是否正常工作？
- ✅ 性能是否满足要求？
- ✅ 是否有 bug 或异常？
- ✅ 接口是否易于使用？
- ✅ 文档是否清晰？

#### 反馈方式

1. **GitHub Issues**：报告 bug 或提出改进建议
2. **Linear 任务评论**：在对应任务下反馈
3. **Git Commit**：如果您修改了蓝图或配置，提交并说明

---

## 典型开发场景

### 场景 1：实现弹珠发射系统

#### 第 1 天：Manus 实现核心逻辑

**创建文件**：
- `Source/EchoAlchemist/Public/BallLauncher.h`
- `Source/EchoAlchemist/Private/BallLauncher.cpp`

**核心功能**：
```cpp
UCLASS()
class ABallLauncher : public AActor
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Ball")
    void LaunchBall(FVector Direction, float Speed);
    
    UFUNCTION(BlueprintPure, Category = "Ball")
    bool CanLaunch() const;
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    float DefaultLaunchSpeed = 1500.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    float LaunchCooldown = 1.0f;
};
```

**提交**：
```bash
git add Source/EchoAlchemist/Public/BallLauncher.h
git add Source/EchoAlchemist/Private/BallLauncher.cpp
git commit -m "feat: Implement Ball Launcher core module"
git push origin main
```

#### 第 2 天：用户创建蓝图

1. **拉取代码并编译**
2. **创建 `BP_BallLauncher`**，继承自 `ABallLauncher`
3. **添加组件**：
   - Static Mesh Component（发射器模型）
   - Arrow Component（显示发射方向）
4. **在 Event Graph 中**：
   - 监听鼠标点击
   - 检查 `CanLaunch()`
   - 调用 `LaunchBall(Direction, Speed)`
5. **测试并反馈**

#### 第 3 天：Manus 根据反馈优化

**用户反馈**：
- "发射速度太快，建议降低到 1200"
- "需要添加发射音效接口"

**Manus 修改**：
```cpp
// 调整默认速度
float DefaultLaunchSpeed = 1200.0f;

// 添加音效接口
UFUNCTION(BlueprintImplementableEvent, Category = "Ball")
void OnBallLaunched();
```

**提交并通知用户**

#### 第 4 天：用户完善蓝图

1. **拉取更新并编译**
2. **在蓝图中实现 `OnBallLaunched` 事件**：
   - 播放发射音效
   - 触发粒子效果
3. **测试并确认完成**

---

## 技术约定

### 1. 命名规范

| 类型 | 前缀 | 示例 |
|------|------|------|
| Actor 类 | A | `ABallLauncher` |
| Object 类 | U | `UAlchemySystem` |
| Struct | F | `FElement` |
| Enum | E | `EElementType` |
| Interface | I | `ICombinable` |

### 2. 暴露给蓝图的宏

| 宏 | 用途 | 示例 |
|-----|------|------|
| `UFUNCTION(BlueprintCallable)` | 可在蓝图中调用的函数 | `LaunchBall()` |
| `UFUNCTION(BlueprintPure)` | 纯函数（无副作用） | `GetAllElements()` |
| `UFUNCTION(BlueprintImplementableEvent)` | 由蓝图实现的事件 | `OnBallLaunched()` |
| `UPROPERTY(BlueprintReadWrite)` | 可读写的属性 | `LaunchSpeed` |
| `UPROPERTY(BlueprintReadOnly)` | 只读属性 | `CurrentBall` |
| `USTRUCT(BlueprintType)` | 可在蓝图中使用的结构体 | `FElement` |

### 3. 模块组织

```
Source/EchoAlchemist/
├── Public/
│   ├── Core/                    # 核心系统
│   │   ├── AlchemySystem.h
│   │   └── SaveGameSystem.h
│   ├── Gameplay/                # 游戏玩法
│   │   ├── BallLauncher.h
│   │   └── BrickActor.h
│   └── Utils/                   # 工具函数
│       └── MathUtils.h
└── Private/
    ├── Core/
    │   ├── AlchemySystem.cpp
    │   └── SaveGameSystem.cpp
    ├── Gameplay/
    │   ├── BallLauncher.cpp
    │   └── BrickActor.cpp
    └── Utils/
        └── MathUtils.cpp
```

---

## 优势总结

### 为什么这个模式最适合人机协同？

#### 1. 职责清晰

- **Manus**：专注于算法和逻辑，不需要关心可视化
- **用户**：专注于游戏体验，不需要深入 C++ 细节

#### 2. 完全通过 Git 同步

- ✅ C++ 代码是纯文本，Git 管理完美
- ✅ 不需要 Remote Control API
- ✅ 不需要内网穿透
- ✅ 离线也可以工作

#### 3. 性能最优

- ✅ 核心逻辑用 C++ 实现，性能高
- ✅ 蓝图只做简单连接，开销小

#### 4. 易于测试

- ✅ C++ 代码可以写单元测试
- ✅ 蓝图逻辑简单，不容易出错

#### 5. 符合 UE4 最佳实践

- ✅ Epic Games 官方推荐
- ✅ 大型 UE4 项目都是这样做的

---

## 常见问题

### Q1: Manus 能否直接创建蓝图？

**A**: 不能。蓝图是二进制文件（`.uasset`），无法通过纯文本方式创建。但 Manus 可以：
- 创建 C++ 基类，用户在蓝图中继承
- 生成 Python 脚本，用户在 UE4 中执行
- 提供详细的蓝图设计文档

### Q2: 如果 C++ 代码编译失败怎么办？

**A**: 
1. 查看编译错误信息
2. 在 GitHub Issues 或 Linear 任务中反馈给 Manus
3. Manus 会修复并重新提交

### Q3: 如何处理蓝图和 C++ 的版本冲突？

**A**: 
- 蓝图文件（`.uasset`）是二进制，Git 无法合并
- 建议：不要同时在多个地方修改同一个蓝图
- 如果冲突，使用 UE4 的 "Diff" 工具手动合并

### Q4: Manus 如何知道蓝图的使用效果？

**A**: 
- 用户录制屏幕视频或截图
- 用户将 Output Log 复制给 Manus
- 定期进行视频会议，用户共享屏幕

### Q5: 可以用 Python 代替蓝图吗？

**A**: 
- Python 可以用于编辑器自动化（如批量创建资产）
- 但游戏运行时逻辑建议用 C++ + 蓝图
- Python 性能不如 C++，且 UE4 的 Python API 有限

---

## 最佳实践

### Manus 的最佳实践

1. ✅ **接口优先**：先设计接口，再实现细节
2. ✅ **文档完整**：每个模块都应该有使用文档
3. ✅ **测试充分**：编写单元测试确保代码质量
4. ✅ **提交清晰**：Commit Message 应该说明如何使用
5. ✅ **响应及时**：及时响应用户的反馈和问题

### 用户的最佳实践

1. ✅ **及时拉取**：定期 `git pull` 获取最新代码
2. ✅ **测试充分**：测试 Manus 的代码并反馈
3. ✅ **反馈清晰**：提供详细的测试结果和截图
4. ✅ **提交规范**：蓝图修改也应该遵循 Git 规范
5. ✅ **保持沟通**：遇到问题及时沟通

---

## 相关文档

- [人机协同开发规范](collaboration_specification.md)
- [两阶段工作流规范](workflow_specification.md)
- [MHP 模板](mhp_template.md)
- [项目结构说明](https://github.com/gdszyy/ue4-human-ai-collab-game/blob/main/Docs/PROJECT_STRUCTURE.md)

---

## 更新日志

| 版本 | 日期 | 说明 |
|------|------|------|
| 1.0 | 2025-11-29 | 初始版本，定义 C++ + 蓝图开发模式 |

---

**最后更新**: 2025-11-29  
**维护者**: Manus AI
