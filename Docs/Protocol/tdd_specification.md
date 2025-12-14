# 技术设计文档（TDD）规范

**版本**: 1.0  
**作者**: Manus AI - 项目策略师  
**日期**: 2025-11-29  
**状态**: Draft

---

## 1. 概述

技术设计文档（Technical Design Document, TDD）是人机协同开发流程中的核心文档，用于明确定义系统的技术实现方案。TDD作为"架构师Manus"和"开发者ManusAI"之间的契约，确保模块解耦和接口清晰。

### 1.1 TDD的作用

1. **明确架构设计**：定义系统的四层架构（数据层、模拟层、粘合层、视觉层）
2. **定义接口规范**：明确模块的输入输出接口
3. **指导实现**：为开发者提供详细的实现指南
4. **验证设计**：通过算法对比和模拟器验证设计的可行性
5. **文档化决策**：记录设计决策和权衡

### 1.2 TDD的时机

TDD应该在以下阶段创建：

- **设计阶段**：在Linear任务进入`Workflow::Design`状态时创建TDD
- **架构审查前**：在开始实现前，由"架构师Manus"审查TDD
- **实现前**：TDD审查通过后，任务才能进入`Workflow::Implementation`状态

---

## 2. TDD模板

### 2.1 文档结构

TDD应该包含以下章节：

```markdown
# [系统名称]技术设计文档（TDD）

**版本**: 1.0
**作者**: [作者名称]
**日期**: [创建日期]
**状态**: [Draft | Review | Approved]
**关联任务**: [Linear任务ID和标题]

---

## 1. 概述

[系统的简要描述，包括目标和核心功能]

### 1.1 设计目标

[列出系统的设计目标，例如：性能、可维护性、可扩展性等]

### 1.2 核心设计原则

[列出系统遵循的核心设计原则，例如：模块解耦、数据驱动等]

---

## 2. 系统架构

### 2.1 四层架构

[明确定义系统的四层架构]

\`\`\`
数据层：[数据结构和数据表]
  ↓
模拟层：[纯逻辑引擎]
  ↓
粘合层：[接口层]
  ↓
视觉层：[视觉表现]
\`\`\`

### 2.2 系统组件

[列出系统的核心组件，使用树状结构]

\`\`\`
SystemName
├── DataLayer（数据层）
│   ├── DataStructure1
│   └── DataStructure2
├── SimulationLayer（模拟层）
│   ├── CoreEngine
│   └── Algorithm
├── GlueLayer（粘合层）
│   └── BlueprintLibrary
└── VisualLayer（视觉层）
    └── UIComponents
\`\`\`

### 2.3 数据流

[使用流程图描述数据如何在各层之间流动]

---

## 3. 数据层设计

### 3.1 数据结构定义

[定义所有数据结构，使用UE5的C++语法]

\`\`\`cpp
USTRUCT(BlueprintType)
struct FDataStructureName
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
    Type FieldName;
};
\`\`\`

### 3.2 数据表定义

[定义所有数据表，说明数据表的用途和字段]

---

## 4. 模拟层设计

### 4.1 核心算法

[详细描述核心算法的原理和实现]

### 4.2 接口设计

[定义模拟层的公共接口]

\`\`\`cpp
UCLASS(BlueprintType)
class USimulationSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "System")
    void Initialize(const FConfigData& Config);

    UFUNCTION(BlueprintCallable, Category = "System")
    void Tick(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "System")
    FOutputData GetState() const;
};
\`\`\`

### 4.3 更新循环

[描述模拟层的更新循环，包括更新顺序和逻辑]

---

## 5. 粘合层设计

### 5.1 蓝图函数库

[定义蓝图函数库的接口]

\`\`\`cpp
UCLASS()
class USystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "System")
    static void DoSomething(UObject* WorldContextObject);
};
\`\`\`

---

## 6. 视觉层设计

### 6.1 视觉表现

[描述视觉层如何呈现模拟层的状态]

### 6.2 UI设计

[描述UI的布局和交互]

---

## 7. 算法选型对比分析（可选）

[如果有多种算法选择，应该对比分析各种算法的优缺点]

### 7.1 方案A：[算法名称]

- **原理**: [算法原理]
- **优点**: [优点列表]
- **缺点**: [缺点列表]
- **结论**: [是否采用]

### 7.2 方案B：[算法名称]

[同上]

---

## 8. 实现细节

### 8.1 关键实现要点

[列出实现时需要注意的关键要点]

### 8.2 性能优化

[描述性能优化的策略]

### 8.3 测试策略

[描述如何测试系统]

---

## 9. 附录：可视化模拟器（可选）

[如果有可视化模拟器，应该说明模拟器的路径和使用方法]

---

**参考资料**:
[列出参考资料]
```

---

## 3. TDD最佳实践

### 3.1 参考VOI-6的TDD

VOI-6的TDD（`docs/tech/world_morphing_system_tdd.md`）是最佳实践的典范，包含：

1. **清晰的四层架构定义**
   - 数据层：SimulationParams
   - 模拟层：engine.ts
   - 粘合层：React状态管理
   - 视觉层：React组件

2. **完整的数据结构定义**
   - Cell数据结构
   - SimulationParams参数配置

3. **详细的算法描述**
   - 地幔层更新逻辑
   - 气候层更新逻辑
   - 晶石层更新逻辑

4. **算法选型对比分析**
   - 经典元胞自动机 vs 纯物理模拟 vs 混合算法
   - 明确说明选择混合算法的理由

5. **可视化模拟器**
   - TypeScript实现的Web模拟器
   - 实时查看三层状态
   - 动态调整参数

### 3.2 参考VOI-11的TDD

VOI-11的TDD（`docs/tech/monster_generation_system_tdd.md`）也是最佳实践，包含：

1. **清晰的模块解耦**
   - 属性系统与外观系统解耦
   - 生态属性和战斗属性分离

2. **完整的接口设计**
   - AttributeGenerator输入输出
   - AppearanceComposer输入输出

3. **详细的实现指南**
   - UE5 Paper2D实现步骤
   - 调色板替换材质实现
   - 蓝图伪代码

4. **示例和用例**
   - 战斗词条示例
   - 生态属性与基础模型的映射

---

## 4. TDD与MHP的关系

TDD和MHP（Manus Handover Protocol）是两个不同的文档：

### 4.1 TDD（技术设计文档）

- **创建时机**：设计阶段
- **创建者**：架构师Manus
- **目的**：定义技术实现方案
- **内容**：架构设计、接口定义、算法描述
- **受众**：开发者ManusAI

### 4.2 MHP（交接文档）

- **创建时机**：实现完成后
- **创建者**：开发者ManusAI
- **目的**：交接实现成果
- **内容**：实现总结、代码位置、测试结果、已知问题
- **受众**：人类开发者

### 4.3 工作流

```
设计阶段：
  架构师Manus创建TDD
    ↓
  TDD审查通过
    ↓
实现阶段：
  开发者ManusAI根据TDD实现
    ↓
  实现完成
    ↓
交接阶段：
  开发者ManusAI创建MHP
    ↓
  人类开发者审查MHP
```

---

## 5. TDD的存储位置

### 5.1 协议仓库（ue5-human-ai-collab）

TDD应该存储在协议仓库的`docs/tech/`目录下：

```
docs/
├── tech/
│   ├── world_morphing_system_tdd.md
│   ├── monster_generation_system_tdd.md
│   ├── combat_system_tdd.md
│   └── ...
```

### 5.2 Game仓库（ue5-human-ai-collab-game）

Game仓库的`Docs/CoreModules/`目录下应该存储**实现文档**，而不是TDD：

```
Docs/
├── CoreModules/
│   ├── PhysicsSystem.md（实现文档）
│   ├── WorldMorphingSystem.md（实现文档）
│   └── ...
```

**区别**：
- **TDD**：设计阶段的技术方案
- **实现文档**：实现完成后的使用指南

---

## 6. TDD的审查流程

### 6.1 审查检查清单

在审查TDD时，应该检查以下内容：

- [ ] 是否明确定义了四层架构？
- [ ] 是否定义了所有数据结构？
- [ ] 是否定义了所有接口？
- [ ] 是否描述了核心算法？
- [ ] 是否进行了算法选型对比分析？
- [ ] 是否考虑了性能优化？
- [ ] 是否定义了测试策略？
- [ ] 是否遵循了模块解耦原则？
- [ ] 是否与其他系统的接口清晰？

### 6.2 审查流程

1. **架构师Manus创建TDD**
2. **提交TDD到协议仓库**
3. **通知人类开发者审查**
4. **人类开发者提出修改意见**
5. **架构师Manus修改TDD**
6. **TDD审查通过，任务进入实现阶段**

---

## 7. TDD的版本管理

### 7.1 版本号规则

TDD应该使用语义化版本号：

- **1.0**：初始版本
- **1.1**：小修改（修复错误、补充细节）
- **2.0**：大修改（架构变更、接口变更）

### 7.2 版本历史

TDD应该在文档末尾记录版本历史：

```markdown
## 版本历史

| 版本 | 日期 | 作者 | 变更说明 |
|------|------|------|----------|
| 1.0 | 2025-11-28 | 架构师Manus | 初始版本 |
| 1.1 | 2025-11-29 | 架构师Manus | 补充性能优化章节 |
```

---

## 8. TDD的命名规范

### 8.1 文件命名

TDD文件应该使用以下命名规范：

```
[系统名称]_tdd.md
```

例如：
- `world_morphing_system_tdd.md`
- `monster_generation_system_tdd.md`
- `combat_system_tdd.md`

### 8.2 文档标题

TDD的文档标题应该使用以下格式：

```
# [系统名称]技术设计文档（TDD）
```

例如：
- `# 世界变迁系统技术设计文档（TDD）`
- `# 怪兽生成系统技术设计文档（TDD）`

---

## 9. TDD的更新和维护

### 9.1 何时更新TDD

TDD应该在以下情况下更新：

- **设计变更**：架构或接口发生变更
- **算法优化**：核心算法发生变化
- **需求变更**：功能需求发生变化
- **实现反馈**：实现过程中发现设计问题

### 9.2 更新流程

1. **识别变更需求**
2. **评估变更影响**
3. **更新TDD**
4. **更新版本号和版本历史**
5. **重新审查TDD**
6. **通知相关人员**

---

## 10. 总结

TDD是人机协同开发流程中的核心文档，确保了：

1. **架构清晰**：四层架构明确定义
2. **接口规范**：输入输出接口清晰
3. **实现指导**：为开发者提供详细指南
4. **设计验证**：通过算法对比和模拟器验证
5. **文档化决策**：记录设计决策和权衡

**所有新任务都必须先创建TDD，TDD审查通过后才能进入实现阶段！**

---

**参考资料**:
1. [VOI-6: 世界变迁系统TDD](tech/world_morphing_system_tdd.md)
2. [VOI-11: 怪兽生成系统TDD](tech/monster_generation_system_tdd.md)
3. [协作规范](collaboration_specification.md)
4. [工作流规范](workflow_specification.md)
