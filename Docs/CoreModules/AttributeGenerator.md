# AttributeGenerator 使用文档

**模块：** PCG | Attributes  
**C++类：** `UAttributeGenerator`

## 1. 功能说明

`AttributeGenerator` 是一个蓝图函数库，用于程序化地生成怪兽属性。它提供了生成生态属性、战斗属性和完整怪兽属性的功能，并能够根据世界状态（气候、地势）和随机种子生成确定性的结果。

该模块是怪兽生成系统的核心部分，负责将世界变迁系统的输出（气候值、地势值）转换为怪兽的属性特征。

## 2. 蓝图使用示例

### 2.1 生成生态属性

生态属性定义了怪兽的基础生存环境、生理结构和基础数值。

**节点：** `Generate Ecology Attributes`

**使用方法：**
1. 提供一个随机种子（Seed）用于确定性生成
2. 提供气候值（Climate Value，0-1）和地势值（Terrain Value，0-1）
3. 函数将返回一个生态属性结构体，包含栖息地、体型、耐受值等

**示例：**
```
Seed: 12345
Climate Value: 0.7 (热带气候)
Terrain Value: 0.5 (丘陵地形)

结果：
- Habitat: Desert (沙漠)
- Size Class: Medium (中型)
- Tolerance: 75.0
- Migration Rate: 45.0
- Fecundity: 55.0
```

### 2.2 生成战斗属性

战斗属性由1-3个战斗词条组成，定义了怪兽的特殊能力和显著外观特征。

**节点：** `Generate Combat Attributes`

**使用方法：**
1. 提供一个随机种子（Seed）
2. 指定要生成的词条数量（Num Traits，1-3）
3. 指定最低稀有度（Min Rarity：Common、Rare、Epic）
4. 函数将返回一个战斗属性结构体，包含1-3个战斗词条

**示例：**
```
Seed: 67890
Num Traits: 2
Min Rarity: Rare

结果：
- Trait 1: Fire Affinity (火属性亲和) - Rare
  - Associated Parts: FireWings, FireTail
  - Effect Parameters: DamageBonus = 2.0, EffectChance = 0.6
  
- Trait 2: Thunder Roar (雷鸣咆哮) - Rare
  - Associated Parts: ThunderMane
  - Effect Parameters: DamageBonus = 2.0, EffectChance = 0.6
```

### 2.3 生成完整怪兽属性

这是最常用的函数，一次性生成完整的怪兽属性（生态 + 战斗）。

**节点：** `Generate Monster Attributes`

**使用方法：**
1. 提供一个随机种子（Seed）
2. 提供气候值和地势值
3. 指定要生成的战斗词条数量
4. 函数将返回一个完整的怪兽属性结构体

**示例：**
```
Seed: 11111
Climate Value: 0.3 (寒冷气候)
Terrain Value: 0.8 (山地地形)
Num Traits: 2

结果：
Monster Attributes:
  Ecology Attributes:
    - Habitat: Tundra (冰原)
    - Size Class: Large (大型)
    - Tolerance: 65.0
    - Migration Rate: 55.0
    - Fecundity: 45.0
  
  Combat Attributes:
    - Trait 1: Ice Breath (冰霜吐息) - Epic
    - Trait 2: Stone Shell (金石外壳) - Rare
```

### 2.4 计算基础数值

根据生态属性计算怪兽的基础战斗数值（HP、ATK、DEF、SPD）。

**节点：** `Calculate Base Stats From Ecology`

**使用方法：**
1. 提供一个生态属性结构体
2. 函数将输出四个基础数值：HP、ATK、DEF、SPD

**示例：**
```
输入：
Ecology Attributes:
  - Habitat: Forest (森林)
  - Size Class: Large (大型)

输出：
- HP: 154.0 (基础100 × 1.4体型 × 1.1栖息地)
- ATK: 24.0 (基础20 × 1.2体型 × 1.0栖息地)
- DEF: 13.0 (基础10 × 1.3体型 × 1.0栖息地)
- SPD: 88.0 (基础100 × 0.8体型 × 1.1栖息地)
```

## 3. 参数说明

### GenerateEcologyAttributes

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Seed** | `int32` | 随机种子，用于确定性生成 |
| **Climate Value** | `float` | 气候值（0-1），0=寒冷，0.5=温和，1=炎热 |
| **Terrain Value** | `float` | 地势值（0-1），0=平坦，0.5=丘陵，1=山地 |
| **Return Value** | `FEcologyAttributes` | 生成的生态属性 |

### GenerateCombatAttributes

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Seed** | `int32` | 随机种子，用于确定性生成 |
| **Num Traits** | `int32` | 要生成的词条数量（1-3） |
| **Min Rarity** | `ERarity` | 最低稀有度（Common、Rare、Epic） |
| **Return Value** | `FCombatAttributes` | 生成的战斗属性 |

### GenerateMonsterAttributes

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Seed** | `int32` | 随机种子，用于确定性生成 |
| **Climate Value** | `float` | 气候值（0-1） |
| **Terrain Value** | `float` | 地势值（0-1） |
| **Num Traits** | `int32` | 要生成的战斗词条数量（1-3） |
| **Return Value** | `FMonsterAttributes` | 完整的怪兽属性 |

### CalculateBaseStatsFromEcology

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Ecology Attributes** | `FEcologyAttributes` | 输入的生态属性 |
| **Out HP** | `float` | 输出的生命值 |
| **Out ATK** | `float` | 输出的攻击力 |
| **Out DEF** | `float` | 输出的防御力 |
| **Out SPD** | `float` | 输出的速度 |

## 4. 数据结构说明

### FEcologyAttributes（生态属性）

| 字段 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Habitat** | `EHabitatType` | 栖息地类型（森林、沙漠、冰原、洞穴、火山、沼泽） |
| **Tolerance** | `float` | 耐受值（0-100），对环境变化的抵抗力 |
| **Size Class** | `ESizeClass` | 体型等级（微小、小型、中型、大型、巨型） |
| **Migration Rate** | `float` | 迁移率（0-100），出现在不同区域的可能性 |
| **Fecundity** | `float` | 繁荣度（0-100），繁殖率和种群密度 |

### FCombatTrait（战斗词条）

| 字段 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Trait ID** | `FName` | 词条唯一标识符 |
| **Trait Name** | `FText` | 词条显示名称 |
| **Trait Description** | `FText` | 词条效果描述 |
| **Rarity** | `ERarity` | 稀有度（普通、稀有、史诗） |
| **Associated Part IDs** | `TArray<FName>` | 关联的视觉部件ID列表 |
| **Effect Parameters** | `TMap<FName, float>` | 战斗效果参数（如伤害加成、效果概率） |

### FCombatAttributes（战斗属性）

| 字段 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Traits** | `TArray<FCombatTrait>` | 战斗词条列表（1-3个） |

### FMonsterAttributes（完整怪兽属性）

| 字段 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Ecology Attributes** | `FEcologyAttributes` | 生态属性 |
| **Combat Attributes** | `FCombatAttributes` | 战斗属性 |

## 5. 与世界变迁系统的集成

`AttributeGenerator` 设计为与世界变迁系统深度集成：

1. **气候层影响**：气候值影响栖息地类型和耐受值
   - 高气候值（炎热）→ 沙漠、火山栖息地
   - 低气候值（寒冷）→ 冰原、洞穴栖息地
   - 中等气候值（温和）→ 森林、沼泽栖息地

2. **地势层影响**：地势值影响栖息地类型和迁移率
   - 高地势值（山地）→ 洞穴、火山栖息地，低迁移率
   - 低地势值（平坦）→ 沙漠、冰原、沼泽栖息地，高迁移率

3. **确定性生成**：使用随机种子确保相同的世界状态生成相同的怪兽

## 6. 战斗词条库

当前内置的战斗词条包括：

| 词条ID | 名称 | 稀有度 | 关联部件 | 效果 |
| :--- | :--- | :--- | :--- | :--- |
| BerserkLeap | 狂暴跳跃 | Common | StrongLegs | 跳跃攻击，造成额外伤害 |
| FireAffinity | 火属性亲和 | Rare | FireWings, FireTail | 火焰攻击，有燃烧效果 |
| StoneShell | 金石外壳 | Rare | RockArmor | 高防御，减少受到的伤害 |
| PoisonSting | 剧毒之刺 | Common | PoisonStinger | 攻击附带毒素效果 |
| IceBreath | 冰霜吐息 | Epic | IceHorns, FrostAura | 冰冻敌人，降低移动速度 |
| Regeneration | 快速再生 | Epic | HealingGlow | 持续恢复生命值 |
| SwiftStrike | 疾风连击 | Common | SharpClaws | 快速连续攻击 |
| ThunderRoar | 雷鸣咆哮 | Rare | ThunderMane | 范围震慑，造成眩晕 |

**注意**：在实际项目中，词条库应该从数据表（Data Table）加载，以便于扩展和平衡调整。

## 7. 注意事项

1. **确定性生成**：相同的种子和世界状态将始终生成相同的怪兽属性
2. **词条数量限制**：每个怪兽最多3个战斗词条，最少1个
3. **稀有度过滤**：`MinRarity` 参数会过滤掉低于指定稀有度的词条
4. **数值平衡**：基础数值由体型和栖息地共同决定，战斗词条提供额外加成
5. **扩展性**：当前词条库是硬编码的，未来应该改为从数据表加载

## 8. 完整蓝图示例

### 示例1：根据当前世界状态生成怪兽

```
// 获取世界变迁系统的当前状态
Get World Morphing Subsystem()
Get Current Climate Value() → Climate Value
Get Current Terrain Value() → Terrain Value

// 生成随机种子（基于时间或位置）
Get Game Time In Seconds() → Time
Time × 1000 → Seed

// 生成怪兽属性
Generate Monster Attributes(Seed, Climate Value, Terrain Value, 2) → Monster Attributes

// 计算基础数值
Calculate Base Stats From Ecology(Monster Attributes.Ecology Attributes, HP, ATK, DEF, SPD)

// 应用战斗词条加成
For Each(Monster Attributes.Combat Attributes.Traits)
  Get Effect Parameters("DamageBonus") → Damage Bonus
  ATK × Damage Bonus → ATK
```

### 示例2：在特定位置生成怪兽

```
// 使用位置作为种子
Get Actor Location() → Location
Location.X + Location.Y × 1000 → Seed

// 采样世界状态
Sample World State At Location(Location) → Climate Value, Terrain Value

// 生成怪兽
Generate Monster Attributes(Seed, Climate Value, Terrain Value, 2) → Monster Attributes

// 创建怪兽Actor
Spawn Actor(Monster Actor, Location)
Set Monster Attributes(Monster Attributes)
```

## 9. 性能考虑

- **轻量级生成**：属性生成是纯计算，无需加载资源，性能开销极小
- **批量生成**：可以在一帧内生成数百个怪兽属性
- **缓存建议**：如果需要频繁访问相同位置的怪兽属性，建议缓存结果

## 10. 调试技巧

1. **固定种子测试**：使用固定的种子值（如12345）进行测试，确保结果可重现
2. **打印属性**：使用 `Print String` 节点打印生成的属性，验证生成逻辑
3. **可视化预览**：使用 `MonsterVisualizer` Actor 在编辑器中预览生成的怪兽
4. **世界状态调试**：调整气候值和地势值，观察对栖息地和属性的影响
