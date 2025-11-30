# 文档目录结构说明

本目录包含《回声炼金师》项目的所有规范文档、技术设计文档和交付物。

## 目录结构

```
docs/
├── README.md                             # 本文件
├── collaboration_specification.md        # 人机协同开发规范
├── workflow_specification.md             # 两阶段工作流规范
├── tdd_specification.md                  # TDD规范
├── self_check_specification.md           # 自检流程规范
├── first_conversation_checklist.md       # 首次对话检查清单
├── task_resource_mapping.md              # Linear任务与资源映射表（索引表）
├── three_stage_prompt_templates.md       # 三阶段提示词模板体系
├── autonomous_mode_prompt.md             # 自主模式提示词
├── cpp_blueprint_workflow.md             # C++核心模块+蓝图连接开发模式
├── mhp_template.md                       # MHP模板
├── tool_comparison_report.md             # 工具对比报告
├── tech/                                 # 技术设计文档目录
│   └── (TDD文档存放于此)
├── mhp/                                  # MHP存档目录
│   ├── VOI-5_git_setup.md                # VOI-5的MHP
│   └── VOI-6_world_morphing.md           # VOI-6的MHP
├── reports/                              # 测试报告和分析文档
│   └── (性能测试报告等存放于此)
└── handover/                             # 交接文档目录
    └── (交接文档存放于此)
```

## 文档分类说明

### 根目录文档
- **规范类文档**: 定义人机协同开发的核心规则和流程
  - `collaboration_specification.md`: 人机协同开发规范
  - `workflow_specification.md`: 两阶段工作流规范
  - `tdd_specification.md`: 技术设计文档（TDD）规范
  - `self_check_specification.md`: 自检流程规范
  - `first_conversation_checklist.md`: 首次对话检查清单
  - `cpp_blueprint_workflow.md`: C++核心模块+蓝图连接开发模式
- **工作流文档**: 引导Manus在不同场景下的工作模式
  - `three_stage_prompt_templates.md`: 三阶段提示词模板体系（需要指定任务）
  - `autonomous_mode_prompt.md`: 自主模式提示词（无需指定任务）
- **模板文档**: 提供标准化的文档格式
  - `mhp_template.md`: MHP模板
- **追踪文档**: 记录项目资源和任务映射
  - `task_resource_mapping.md`: Linear任务与资源映射表（索引表）

### tech/ 目录
存放所有技术设计文档（TDD），包括：
- 系统架构设计
- 算法设计文档
- 技术方案对比分析

**命名规范**: `{系统名称}_tdd.md`  
**示例**: `world_morphing_system_tdd.md`

### mhp/ 目录
存放所有Manus工作交接协议（MHP），用于记录每个任务的完整决策过程。

**命名规范**: `VOI-{任务ID}_{任务简称}.md`  
**示例**: `VOI-6_world_morphing.md`

### reports/ 目录
存放所有测试报告、性能分析和评估文档，包括：
- 性能测试报告
- 算法对比分析
- 集成测试报告

**命名规范**: `{报告类型}_{日期}.md`  
**示例**: `performance_test_report_20251128.md`

### handover/ 目录
存放所有交接文档，用于确保跨对话的任务交接能够顺畅进行。

**命名规范**: `VOI-{任务ID}_handover.md`  
**示例**: `VOI-6_handover.md`

## 交付物沉淀规则

1. **所有交付物必须提交到GitHub仓库**，而不是仅保存在本地或Notion
2. **MHP必须按任务ID归档**到`mhp/`目录，便于后续查找和追溯
3. **技术文档必须分类存放**，保持清晰的信息架构
4. **文件命名必须遵循规范**，使用小写字母和下划线分隔
5. **每个目录应包含README.md**（如有必要），说明该目录的用途和内容

## 文档维护

- 所有文档应使用Markdown格式
- 文档应包含版本号和最后更新时间
- 重大更新应在文档开头添加更新日志
- 过时的文档应移动到`archive/`目录（如需创建）

---

## 重要更新

### 2025-11-29: 开发模式确定

✅ **采用 C++ 核心模块 + 蓝图连接开发模式**

- **Manus 职责**: 实现 C++ 核心逻辑（算法、系统架构、性能优化）
- **用户职责**: 在蓝图中连接核心模块（游戏玩法、UI、关卡设计）
- **同步方式**: 完全通过 Git 同步，无需 Remote Control API
- **优势**: 职责清晰、性能最优、易于测试、符合 UE4 最佳实践

详细工作流程请参考 [C++ 核心模块 + 蓝图连接开发模式](cpp_blueprint_workflow.md)。

### 2025-11-29: TDD规范加入人机协议

✅ **技术设计文档（TDD）规范正式加入人机协议**

- **TDD作用**: 明确定义系统的技术实现方案，作为架构师和开发者之间的契约
- **四层架构**: 所有系统都必须遵循数据层、模拟层、粘合层、视觉层的架构
- **最佳实践**: 参考VOI-6（世界变迁系统）和VOI-11（怪兽生成系统）的TDD
- **工作流**: 设计阶段创建TDD → TDD审查通过 → 进入实现阶段

详细规范请参考 [TDD规范](tdd_specification.md)。

### 2025-11-29: 自检流程和索引表加入人机协议

✅ **自检流程规范和索引表正式加入人机协议**

- **自检流程**: 对话结束前必须执行自检，验证所有交付物符合标准
- **索引表**: 记录Linear任务与Git仓库资源的映射关系，便于快速定位和追溯
- **首次对话检查清单**: 每个对话开始时必须执行，确保规范理解和资源确认
- **工作流集成**: 自检流程和索引表更新已集成到两阶段工作流中

详细规范请参考：
- [自检流程规范](self_check_specification.md)
- [索引表](task_resource_mapping.md)
- [首次对话检查清单](first_conversation_checklist.md)

---

**最后更新**: 2025-11-29  
**维护者**: Manus AI
