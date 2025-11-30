// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAttributes.generated.h"

/**
 * @enum EHabitatType
 * @brief Defines the habitat type for monster ecology attributes.
 * 
 * The habitat type determines the monster's base skeleton, color palette, and base stats.
 */
UENUM(BlueprintType)
enum class EHabitatType : uint8
{
    Forest      UMETA(DisplayName = "森林"),
    Desert      UMETA(DisplayName = "沙漠"),
    Tundra      UMETA(DisplayName = "冰原"),
    Cave        UMETA(DisplayName = "洞穴"),
    Volcano     UMETA(DisplayName = "火山"),
    Swamp       UMETA(DisplayName = "沼泽")
};

/**
 * @enum ESizeClass
 * @brief Defines the size class for monster ecology attributes.
 * 
 * The size class affects the monster's base skeleton scale and stats.
 */
UENUM(BlueprintType)
enum class ESizeClass : uint8
{
    Tiny        UMETA(DisplayName = "微小"),
    Small       UMETA(DisplayName = "小型"),
    Medium      UMETA(DisplayName = "中型"),
    Large       UMETA(DisplayName = "大型"),
    Giant       UMETA(DisplayName = "巨型")
};

/**
 * @enum ERarity
 * @brief Defines the rarity level for combat traits.
 * 
 * Rarity affects the probability of trait generation and its power level.
 */
UENUM(BlueprintType)
enum class ERarity : uint8
{
    Common      UMETA(DisplayName = "普通"),
    Rare        UMETA(DisplayName = "稀有"),
    Epic        UMETA(DisplayName = "史诗")
};

/**
 * @struct FEcologyAttributes
 * @brief Represents the ecology attributes of a monster.
 * 
 * Ecology attributes define the monster's base survival environment, physiological structure,
 * and base stats. They are stable characteristics evolved in a specific ecological niche.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // In Blueprint, create a variable of type "Ecology Attributes"
 * // Set Habitat to "Desert", Size Class to "Medium", Tolerance to 75.0
 * // This will generate a desert-adapted, medium-sized monster with high tolerance
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FEcologyAttributes
{
    GENERATED_BODY()

    /** Habitat type - determines base skeleton and color palette */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecology")
    EHabitatType Habitat = EHabitatType::Forest;

    /** Tolerance value (0-100) - resistance to environmental changes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecology", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float Tolerance = 50.0f;

    /** Size class - affects base skeleton scale and stats */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecology")
    ESizeClass SizeClass = ESizeClass::Medium;

    /** Migration rate (0-100) - likelihood of appearing in different regions */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecology", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float MigrationRate = 30.0f;

    /** Fecundity (0-100) - reproduction rate and population density */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecology", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float Fecundity = 50.0f;

    /** Default constructor */
    FEcologyAttributes()
        : Habitat(EHabitatType::Forest)
        , Tolerance(50.0f)
        , SizeClass(ESizeClass::Medium)
        , MigrationRate(30.0f)
        , Fecundity(50.0f)
    {
    }
};

/**
 * @struct FCombatTrait
 * @brief Represents a single combat trait (词条) for a monster.
 * 
 * Combat traits are special abilities or mutations acquired through survival competition.
 * They directly affect combat behavior and significant appearance features.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // In Blueprint, create a Combat Trait with:
 * // - Trait ID: "FireAffinity"
 * // - Trait Name: "火属性亲和"
 * // - Rarity: "Rare"
 * // - Associated Part IDs: ["FireWings", "FireTail"]
 * // - Effect Parameters: {"DamageBonus": 1.5, "BurnChance": 0.3}
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FCombatTrait
{
    GENERATED_BODY()

    /** Unique trait identifier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FName TraitID;

    /** Display name of the trait */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FText TraitName;

    /** Description of the trait's effects */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FText TraitDescription;

    /** Rarity level - affects generation probability and power */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    ERarity Rarity = ERarity::Common;

    /** List of associated visual part IDs (for appearance modification) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FName> AssociatedPartIDs;

    /** Combat effect parameters (e.g., damage bonus, movement distance) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TMap<FName, float> EffectParameters;

    /** Default constructor */
    FCombatTrait()
        : TraitID(NAME_None)
        , TraitName(FText::GetEmpty())
        , TraitDescription(FText::GetEmpty())
        , Rarity(ERarity::Common)
    {
    }
};

/**
 * @struct FCombatAttributes
 * @brief Represents the combat attributes of a monster.
 * 
 * Combat attributes consist of 1-3 combat traits that define the monster's
 * combat behavior and significant visual features.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // In Blueprint, create a Combat Attributes variable
 * // Add 1-3 Combat Traits to the Traits array
 * // Each trait will modify the monster's appearance and combat behavior
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FCombatAttributes
{
    GENERATED_BODY()

    /** List of combat traits (1-3 traits per monster) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FCombatTrait> Traits;

    /** Default constructor */
    FCombatAttributes()
    {
    }
};

/**
 * @struct FMonsterAttributes
 * @brief Complete attributes for a procedurally generated monster.
 * 
 * Combines ecology attributes (base characteristics) and combat attributes (special abilities)
 * to define a unique monster.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // In Blueprint, create a Monster Attributes variable
 * // Set Ecology Attributes (habitat, size, tolerance, etc.)
 * // Set Combat Attributes (1-3 combat traits)
 * // Use this to generate a complete monster with appearance and behavior
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FMonsterAttributes
{
    GENERATED_BODY()

    /** Ecology attributes - base characteristics */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
    FEcologyAttributes EcologyAttributes;

    /** Combat attributes - special abilities */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
    FCombatAttributes CombatAttributes;

    /** Default constructor */
    FMonsterAttributes()
    {
    }
};
