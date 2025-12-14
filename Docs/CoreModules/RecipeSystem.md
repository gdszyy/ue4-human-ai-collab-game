## 炼金配方系统使用指南

炼金配方系统是《回声炼金师》的核心玩法系统，负责管理魔药配方的组合、发现、存储和应用。通过配方系统，玩家可以将基础药剂和魔法添加剂组合成强大的魔药，提升策略深度和解谜乐趣。

---

## 系统概述

炼金配方系统提供了完整的配方管理功能，包括配方评估、发现机制、配方库管理和持久化存储。系统采用模块化设计，与其他游戏系统解耦，易于扩展和维护。

### 核心功能

配方系统实现了以下核心功能：

**配方评估**：根据玩家选择的基础药剂和添加剂组合，系统会自动计算配方效果，包括基础伤害、特殊效果和元素伤害等。评估过程支持世界状态影响，不同的环境条件会调整配方的效果倍率。

**配方发现**：当玩家首次使用某个配方组合时，系统会自动记录为新发现的配方，并保存到配方库中。已发现的配方可以在配方库UI中查看和快速应用。

**配方库管理**：系统维护一个完整的配方库，记录所有已发现的配方、发现时间和使用次数。配方库支持分类浏览、效果对比等功能。

**持久化存储**：配方库可以保存到玩家存档中，即使在Roguelike循环失败后，已发现的配方仍然保留，作为玩家的知识积累。

**世界状态联动**：配方效果会受到世界状态的影响。例如，在高温气候区，火属性魔药的效果会增强；在晶石密集区，所有配方的效果都会得到提升。

---

## 数据结构

### 基础药剂类型

系统定义了三种基础弹射药剂类型，每种药剂都有独特的战斗特性：

**弹射药剂（Ricochet）**：最基础的魔药类型，每次碰撞后伤害递增。适合对付单个强敌或需要持续输出的场景。基础伤害为10，速度为500 cm/s。

**穿透药剂（Piercing）**：可以穿透敌人，对路径上的所有敌人造成伤害。适合对付排成一线的敌群。基础伤害为15，速度为600 cm/s。

**爆炸药剂（Explosive）**：碰撞后产生爆炸，对范围内的敌人造成伤害。适合对付密集的敌群。基础伤害为20，速度为400 cm/s，爆炸半径为200 cm。

### 魔法添加剂类型

系统提供了丰富的魔法添加剂，分为持续类、特殊效果类和元素类三大类别：

**持续类添加剂**包括持续药剂（增加药效强度20%）和强化药剂（增加伤害50%）。这些添加剂主要用于提升魔药的基础属性。

**特殊效果添加剂**包括分裂药剂（碰撞后分裂成3个子代）、连锁药剂（触发5次连锁反应）、引力药剂（产生1000 cm/s²的引力场）、传送药剂（创建500 cm范围的虫洞）、加速药剂（速度提升50%）和减速药剂（速度降低50%）。这些添加剂为魔药添加独特的战术效果。

**元素类添加剂**包括火焰、冰霜、雷电和毒素四种元素，每种元素都会附加15点对应的元素伤害。元素伤害会受到世界状态的影响，在特定环境下效果更强。

### 配方效果键

系统使用标准化的效果键来表示配方效果，所有效果都存储在`TMap<FString, float>`中。主要的效果键包括：

**基础属性**：`BaseDamage`（基础伤害）、`Potency`（药效强度）、`Speed`（速度）、`Radius`（半径）。

**特殊效果**：`SplitCount`（分裂数量）、`ChainCount`（连锁数量）、`GravityStrength`（引力强度）、`TeleportRange`（传送距离）。

**元素伤害**：`FireDamage`（火焰伤害）、`IceDamage`（冰霜伤害）、`LightningDamage`（雷电伤害）、`PoisonDamage`（毒素伤害）。

**效果倍率**：`DamageMultiplier`（伤害倍率）、`DurationMultiplier`（持续时间倍率）、`RangeMultiplier`（范围倍率）。

### 配方稀有度

配方的稀有度由添加剂数量和类型决定，范围从0到5：

- **0级（普通）**：无添加剂的基础药剂，灰色显示
- **1级（常见）**：单个添加剂，白色显示
- **2-3级（稀有/精良）**：两个添加剂，绿色或蓝色显示
- **4-5级（史诗/传说）**：三个添加剂或包含特殊添加剂（引力、传送），紫色或橙色显示

---

## C++使用示例

### 获取配方系统实例

配方系统采用单例模式，通过`URecipeSystem::Get()`获取实例：

```cpp
URecipeSystem* RecipeSystem = URecipeSystem::Get(GetWorld());
if (!RecipeSystem)
{
    UE_LOG(LogTemp, Error, TEXT("Failed to get RecipeSystem"));
    return;
}
```

### 评估配方

创建配方输入并评估配方效果：

```cpp
// 创建配方输入
FRecipeInput Input;
Input.BasePotion = EBasePotionType::Ricochet;
Input.Additives.Add(EAdditiveType::Split);
Input.Additives.Add(EAdditiveType::Fire);

// 添加世界状态参数（可选）
Input.WorldState.Add(TEXT("Temperature"), 35.0f);
Input.WorldState.Add(TEXT("CrystalDensity"), 2.0f);

// 评估配方
FRecipeOutput Output = RecipeSystem->EvaluateRecipe(Input);

// 检查是否为新配方
if (Output.bIsNewRecipe)
{
    UE_LOG(LogTemp, Log, TEXT("发现新配方: %s"), *Output.DisplayName);
}

// 获取配方效果
float BaseDamage = Output.Effects.FindRef(RecipeEffectKeys::BaseDamage);
float SplitCount = Output.Effects.FindRef(RecipeEffectKeys::SplitCount);
float FireDamage = Output.Effects.FindRef(RecipeEffectKeys::FireDamage);

UE_LOG(LogTemp, Log, TEXT("配方效果 - 基础伤害: %.1f, 分裂数量: %.0f, 火焰伤害: %.1f"),
    BaseDamage, SplitCount, FireDamage);
```

### 查询配方库

获取已发现的配方列表：

```cpp
// 获取所有已发现的配方ID
TArray<FString> DiscoveredRecipeIDs = RecipeSystem->GetDiscoveredRecipes();

// 获取所有已发现的配方定义
TArray<FRecipeDefinition> DiscoveredRecipes = RecipeSystem->GetDiscoveredRecipeDefinitions();

for (const FRecipeDefinition& Recipe : DiscoveredRecipes)
{
    UE_LOG(LogTemp, Log, TEXT("配方: %s (稀有度: %d)"), *Recipe.DisplayName, Recipe.Rarity);
}

// 检查特定配方是否已发现
bool bDiscovered = RecipeSystem->IsRecipeDiscovered(TEXT("Recipe_0_3_4"));

// 获取配方使用次数
int32 UsageCount = RecipeSystem->GetRecipeUsageCount(TEXT("Recipe_0_3_4"));
```

### 保存和加载配方库

配方库支持持久化存储：

```cpp
// 保存配方库
bool bSaveSuccess = RecipeSystem->SaveRecipeLibrary(TEXT("MyRecipeLibrary"));

// 加载配方库
bool bLoadSuccess = RecipeSystem->LoadRecipeLibrary(TEXT("MyRecipeLibrary"));

// 重置配方库（清除所有已发现的配方）
RecipeSystem->ResetRecipeLibrary();
```

---

## 蓝图使用示例

### 评估配方

在蓝图中使用配方系统的步骤：

1. 调用`Make Recipe Input`节点创建配方输入
2. 设置`Base Potion`和`Additives`参数
3. （可选）调用`Add World State Parameter`添加世界状态参数
4. 调用`Evaluate Recipe`节点评估配方
5. 从`Recipe Output`中提取效果值

蓝图示例流程：

```
Make Recipe Input (Base Potion = Ricochet, Additives = [Split, Fire])
    ↓
Add World State Parameter (Key = "Temperature", Value = 35.0)
    ↓
Evaluate Recipe
    ↓
Get Effect Value (Effect Key = "BaseDamage")
```

### 查询配方库

获取已发现的配方列表并显示在UI中：

```
Get Discovered Recipes
    ↓
For Each Loop (Recipe Definition)
    ↓
Get Rarity Color (Rarity)
    ↓
Create Widget (Recipe Item UI)
```

### 保存和加载

在游戏开始和结束时保存/加载配方库：

```
Event Begin Play
    ↓
Load Recipe Library (Slot Name = "RecipeLibrary")

Event End Play
    ↓
Save Recipe Library (Slot Name = "RecipeLibrary")
```

---

## 配方组合建议

### 基础组合

以下是一些推荐的基础配方组合：

**弹射 + 分裂**：适合对付多个敌人，分裂后的子代可以攻击不同目标。稀有度1，易于获得。

**穿透 + 火焰**：适合对付排成一线的敌群，附加火焰伤害提升总输出。稀有度1，在高温环境下效果更强。

**爆炸 + 强化**：适合对付密集敌群，强化后的爆炸伤害非常可观。稀有度1，简单实用。

### 进阶组合

**弹射 + 分裂 + 火焰**：炎爆分裂药剂，分裂后的每个子代都附带火焰伤害，形成火焰风暴。稀有度2，适合中期使用。

**爆炸 + 连锁 + 雷电**：雷霆连锁爆破，爆炸后触发连锁雷电，对大范围敌人造成持续伤害。稀有度3，适合对付大量敌人。

**穿透 + 引力 + 冰霜**：冰霜引力陷阱，创造引力场吸引敌人，同时释放冰霜伤害和减速效果。稀有度4，适合控制战场。

### 传说组合

**穿透 + 引力 + 冰霜 + 毒素**：极寒毒素黑洞，创造引力场吸引敌人，同时释放冰霜和毒素，造成持续伤害和减速。稀有度5，最强配方之一。

**爆炸 + 分裂 + 连锁 + 雷电**：雷霆分裂连锁，爆炸后分裂成多个子代，每个子代都能触发连锁雷电。稀有度5，适合终极战斗。

---

## 世界状态影响

### 温度影响

温度参数会影响火焰和冰霜元素的效果：

- **高温环境（>30°C）**：火焰伤害增强20%
- **低温环境（<10°C）**：冰霜伤害增强20%

示例：在火山区域使用火焰配方，效果倍率为1.2；在雪山区域使用冰霜配方，效果倍率为1.2。

### 晶石密度影响

晶石密度参数会影响所有配方的效果：

- **晶石密度每增加1**：所有效果增强10%

示例：在晶石密度为2的区域，所有配方的效果倍率为1.2；在晶石密度为5的区域，效果倍率为1.5。

### 自定义世界状态

开发者可以添加自定义的世界状态参数，系统会自动传递给配方评估逻辑：

```cpp
Input.WorldState.Add(TEXT("MagicPower"), 3.0f);
Input.WorldState.Add(TEXT("Corruption"), 0.5f);
```

然后在`CalculateWorldStateMultiplier`方法中实现自定义的倍率计算逻辑。

---

## 性能优化建议

### 配方缓存

配方评估结果会自动缓存在`RecipeDefinitions`中，相同的配方组合不会重复计算。建议在游戏初始化时预先评估常用配方，以减少运行时开销。

### 效果查询优化

使用`TMap::FindRef`而不是`TMap::Find`来查询效果值，可以避免空指针检查：

```cpp
// 推荐
float BaseDamage = Output.Effects.FindRef(RecipeEffectKeys::BaseDamage);

// 不推荐
const float* BaseDamagePtr = Output.Effects.Find(RecipeEffectKeys::BaseDamage);
float BaseDamage = BaseDamagePtr ? *BaseDamagePtr : 0.0f;
```

### 配方库大小

配方库的大小取决于基础药剂和添加剂的组合数量。当前配置下，理论上最多可以有`3 * C(13, 3) = 858`种配方（3种基础药剂 × 13种添加剂的3元组合）。实际游戏中，玩家可能只会发现其中的一部分。

---

## 扩展开发指南

### 添加新的基础药剂

在`EBasePotionType`枚举中添加新类型，然后在`CalculateBaseEffects`方法中实现对应的效果计算逻辑：

```cpp
// 在RecipeData.h中添加
UENUM(BlueprintType)
enum class EBasePotionType : uint8
{
    // ... 现有类型 ...
    
    /** 追踪药剂 - 自动追踪敌人 */
    Homing UMETA(DisplayName = "Homing")
};

// 在RecipeSystem.cpp的CalculateBaseEffects中添加
case EBasePotionType::Homing:
    Effects.Add(RecipeEffectKeys::BaseDamage, 12.0f);
    Effects.Add(RecipeEffectKeys::Speed, 450.0f);
    Effects.Add(TEXT("HomingStrength"), 800.0f);
    break;
```

### 添加新的魔法添加剂

在`EAdditiveType`枚举中添加新类型，然后在`CalculateBaseEffects`方法中实现对应的效果：

```cpp
// 在RecipeData.h中添加
UENUM(BlueprintType)
enum class EAdditiveType : uint8
{
    // ... 现有类型 ...
    
    /** 反弹药剂 - 增加反弹次数 */
    Bounce UMETA(DisplayName = "Bounce")
};

// 在RecipeSystem.cpp的CalculateBaseEffects中添加
case EAdditiveType::Bounce:
    Effects.Add(TEXT("BounceCount"), 5.0f);
    break;
```

### 自定义配方发现逻辑

可以重写`EvaluateRecipe`方法来实现自定义的配方发现逻辑，例如：

- 只有在特定条件下才能发现某些配方
- 配方发现需要消耗资源
- 配方发现触发特殊事件或奖励

### 集成到炼金循环

配方系统设计为独立模块，可以轻松集成到炼金循环中：

1. 在炼金工作台UI中，玩家选择基础药剂和添加剂
2. 调用`EvaluateRecipe`评估配方
3. 将配方效果应用到魔力露珠的物理状态
4. 在战斗场景中，魔力露珠根据配方效果执行对应的行为

---

## 单元测试

配方系统包含完整的单元测试，覆盖所有核心功能：

- **FRecipeSystemBasicTest**：测试基础配方评估和发现
- **FRecipeSystemAdditiveTest**：测试单添加剂配方
- **FRecipeSystemMultiAdditiveTest**：测试多添加剂配方
- **FRecipeSystemDiscoveryTest**：测试配方发现机制
- **FRecipeSystemWorldStateTest**：测试世界状态影响
- **FRecipeSystemLibraryTest**：测试配方库管理
- **FRecipeBlueprintLibraryTest**：测试蓝图函数库
- **FRecipeEffectExtractionTest**：测试效果提取

运行测试：

```
// 在UE编辑器中
Window -> Developer Tools -> Session Frontend -> Automation -> 运行所有测试

// 或在命令行中
UE5Editor.exe ProjectName -ExecCmds="Automation RunTests EchoAlchemist.Alchemy"
```

---

## 常见问题

### Q: 如何获取配方的显示名称？

A: 配方的显示名称会自动生成，格式为"基础药剂名称 + 添加剂名称 + 药剂"。也可以使用`URecipeBlueprintLibrary::GetBasePotionDisplayName`和`GetAdditiveDisplayName`获取单个组件的名称。

### Q: 配方效果如何应用到魔力露珠？

A: 配方效果存储在`TMap<FString, float>`中，调用方需要根据效果键提取对应的值，并应用到魔力露珠的物理状态。例如，`BaseDamage`应用到`FMarbleState::BaseDamage`，`SplitCount`应用到特殊效果系统的分裂参数。

### Q: 如何实现配方的持久化保存？

A: 当前版本的`SaveRecipeLibrary`和`LoadRecipeLibrary`方法是占位实现，需要创建一个`USaveGame`子类来存储`FRecipeLibrarySaveData`，然后使用`UGameplayStatics::SaveGameToSlot`和`LoadGameFromSlot`实现实际的保存和加载逻辑。

### Q: 配方稀有度如何影响游戏玩法？

A: 配方稀有度主要用于UI显示和玩家引导。高稀有度的配方通常效果更强，但也更难发现。可以在游戏中设置稀有度限制，例如只有在特定条件下才能使用高稀有度配方。

### Q: 如何平衡不同配方的强度？

A: 配方强度由基础效果和添加剂效果共同决定。可以通过调整`CalculateBaseEffects`中的数值来平衡配方强度。建议通过游戏测试和数据分析来调整参数，确保所有配方都有使用价值。

---

## 技术架构

### 模块解耦

配方系统采用纯数据管理和组合逻辑设计，不直接依赖其他游戏模块。所有接口都使用通用的键值对（`TMap<FString, float>`），由调用方解释和应用。

### 单例模式

配方系统使用单例模式，通过`URecipeSystem::Get()`获取全局唯一实例。实例存储在`UGameInstance`的子系统中，确保在整个游戏会话期间持久存在。

### 数据驱动

所有配方定义都存储在`RecipeDefinitions`中，支持运行时动态添加和修改。未来可以将配方定义存储在数据表（DataTable）中，实现完全的数据驱动。

### 蓝图友好

所有核心功能都通过`URecipeBlueprintLibrary`暴露给蓝图，支持蓝图开发者无需编写C++代码即可使用配方系统。

---

## 版本历史

### v1.0.0 (2025-12-08)

初始版本，实现以下功能：

- 基础配方评估和发现机制
- 配方库管理和查询
- 世界状态影响计算
- 蓝图函数库
- 完整的单元测试
- 使用文档

---

## 作者

**Manus AI** - 自主开发者模式

**项目**: 《回声炼金师》UE5游戏开发

**任务**: VOI-12 - 炼金配方系统

**日期**: 2025-12-08
