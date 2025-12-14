# 项目快速入门指南 (QUICKSTART)

**版本**: 2.0 | **最后更新**: 2025-11-30

你好！我是Manus,你的自主开发者伙伴。本指南是我进入《回声炼金师》项目的唯一入口,旨在让我快速理解核心规范并开始高效工作。

---

## 1. 项目概述

*   **项目名称**: 《回声炼金师》(EchoAlchemist)
*   **引擎版本**: Unreal Engine 5 (UE5)
*   **核心模式**: 人机协同开发,我(Manus)负责C++核心模块实现,您(用户)负责蓝图连接与游戏设计。

---

## 2. 核心协作原则

我们遵循**"外部化状态管理"**原则,项目的记忆和状态持久化存储在外部工具中,而非依赖临时对话。

| 工具 | 角色 | 核心职责 |
| :--- | :--- | :--- |
| **Linear** | "要做什么？" (What) | 管理所有任务、需求和项目进度,是任务状态的唯一权威来源。 |
| **GitHub** | "已经做了什么？" (How) | 管理所有代码、文档和交付物,是技术实现的唯一权威来源。 |

---

## 3. 开发模式：C++核心模块 + 蓝图连接

我们的职责分工明确,以发挥各自的最大优势。

*   **我的职责 (Manus)**:
    *   使用C++实现所有核心系统、算法和性能敏感的逻辑。
    *   通过`UFUNCTION(BlueprintCallable)`等方式将接口暴露给蓝图。
    *   编写并提交所有相关的代码、单元测试和使用文档到GitHub。
    *   **确保所有代码兼容UE5，禁止使用UE5专属特性**。

*   **您的职责 (用户)**:
    *   在UE编辑器中,通过蓝图调用我提供的C++接口,实现具体游戏玩法、UI和关卡设计。
    *   审核我提交的代码,并通过Git反馈结果。

**同步方式**: 我们之间**完全通过Git同步代码和文档**,无需其他远程控制API。

---

## 4. 两阶段工作流

我们采用**两阶段分离**的工作流,以标签区分任务类型,实现高效异步协作。

*   **`Workflow::Design` (设计阶段)**:
    *   **内容**: 规划、架构、游戏设计、文档编写。
    *   **目标**: 产出清晰的设计文档和面向开发的任务规划。

*   **`Workflow::Implementation` (实现阶段)**:
    *   **内容**: 编码、资产集成、调试、测试。
    *   **目标**: 接收明确的设计输入,产出经过验证的可合并代码。

---

## 5. UE5兼容性要求

**⚠️ 重要**: 本项目使用Unreal Engine 5,所有代码必须严格兼容UE5。

### 禁止使用的UE5专属特性

*   **渲染**: Nanite、Lumen、Virtual Shadow Maps
*   **输入**: Enhanced Input System (使用传统Input组件)
*   **音频**: MetaSounds (使用Sound Cue)
*   **世界**: World Partition (使用Level Streaming)
*   **其他**: One File Per Actor、Motion Warping等

### 推荐使用的UE5 API

*   **反射宏**: `UCLASS()`, `UPROPERTY()`, `UFUNCTION()`
*   **输入处理**: `UInputComponent::BindAction/BindAxis`
*   **音频系统**: `USoundCue`, `UAudioComponent`
*   **基础类型**: `UObject`, `AActor`, `UActorComponent`, `UGameInstanceSubsystem`

---

## 6. 常用命令

*   **列出所有待处理任务**:
    ```bash
    manus-mcp-cli tool call list_issues --server linear --input '{"project_id": "f7f494ca8063"}'
    ```

---

## 7. 文档与资源索引

所有详细的规范、技术设计和历史记录都沉淀在本仓库中。当需要深入了解特定主题时,我会按需查阅以下文档。**我不会在启动时读取所有文档**。

| 主题 | 关键文档路径 (相对于仓库根目录) | 说明 |
| :--- | :--- | :--- |
| **协议文档索引** | `Docs/Protocol/README.md` | **（优先查阅）** 协议文档的目录结构和分类说明。 |
| **任务资源总览** | `Docs/Protocol/task_resource_mapping.md` | **（优先查阅）** 任务ID与所有相关代码、文档的映射表。 |
| **协同开发规范** | `Docs/Protocol/collaboration_specification.md` | 详细定义了工具链、Git规范、交付物规则等。 |
| **工作流规范** | `Docs/Protocol/workflow_specification.md` | 两阶段工作流的详细状态定义和协作流程。 |
| **TDD规范** | `Docs/Protocol/tdd_specification.md` | 技术设计文档(TDD)的编写规范和四层架构定义。 |
| **自检流程** | `Docs/Protocol/self_check_specification.md` | 对话结束前的交付物自检清单,包含UE5兼容性检查。 |
| **首次对话检查清单** | `Docs/Protocol/first_conversation_checklist.md` | 每个对话开始时必须执行的检查清单。 |
| **C++/蓝图开发** | `Docs/Protocol/cpp_blueprint_workflow.md` | C++与蓝图协作开发的详细技术规范和示例。 |
| **游戏核心模块** | `Docs/CoreModules/` | 各个C++核心模块的设计和使用文档。 |
| **项目结构说明** | `Docs/PROJECT_STRUCTURE.md` | 项目目录结构和组织方式。 |
| **本地开发指南** | `Docs/LOCAL_SETUP_GUIDE.md` | 本地开发环境搭建指南。 |

---

## 8. 首次对话检查清单

在每个对话开始时,我会确认以下事项:

### 人机协同规范
- ✅ 我已理解《人机协同开发规范》
- ✅ 我已理解《两阶段工作流规范》
- ✅ 我已理解《TDD规范》
- ✅ 我已理解《自检流程规范》

### 项目资源
- ✅ Game仓库: `ue5-human-ai-collab-game`
- ✅ Linear项目: https://linear.app/voidzyy/project/ue4游戏开发-人机协同mvp-f7f494ca8063
- ✅ 索引表: `Docs/Protocol/task_resource_mapping.md`

### 工作要求
- ✅ 所有设计任务必须先创建TDD
- ✅ 所有实现任务必须遵循四层架构
- ✅ 所有任务完成后必须更新索引表
- ✅ 对话结束前必须执行自检流程
- ✅ 90%开发工作由ManusAI通过UE5 API完成
- ✅ **所有代码必须兼容UE5，禁止使用UE5专属特性**

---

## 9. 快速启动流程

当您启动一个新的Manus开发者实例时,我会按照以下流程工作:

1. **克隆仓库**: `gh repo clone gdszyy/ue5-human-ai-collab-game`
2. **阅读本文档**: 理解核心协作规范和工作流程
3. **获取任务列表**: 使用Linear MCP工具列出所有待处理任务
4. **选择任务**: 根据您的指示或任务优先级选择一个任务
5. **按需查阅**: 根据任务类型,从文档索引表查找并阅读相关详细规范
6. **执行任务**: 遵循规范完成设计或实现工作
7. **自检**: 执行自检流程,确保所有交付物符合标准
8. **提交**: 将所有代码和文档提交到GitHub

---

**本文档是Manus进入项目的唯一入口,包含了启动所需的所有核心信息。**

**最后更新**: 2025-11-30  
**维护者**: Manus AI  
**版本**: 2.0

---

## 更新日志

### v2.0 (2025-11-30)
- 整合协议仓库到Game仓库的`Docs/Protocol/`目录
- 更新所有文档路径为单仓库模式
- 添加UE5兼容性要求章节
- 添加首次对话检查清单章节
- 添加快速启动流程章节
- 强调禁止使用UE5专属特性

### v1.0 (2025-11-30)
- 初始版本发布
- 创建快速入门指南
