// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PCG/MonsterAttributes.h"
#include "AttributeGenerator.generated.h"

/**
 * @class UAttributeGenerator
 * @brief A Blueprint Function Library for generating monster attributes procedurally.
 * 
 * This class provides functions to generate ecology attributes, combat attributes,
 * and complete monster attributes based on world state and random seeds.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Generate random ecology attributes
 * Ecology Attributes = Generate Ecology Attributes(Seed: 12345, Climate Value: 0.7, Terrain Value: 0.5)
 * 
 * // Generate random combat attributes
 * Combat Attributes = Generate Combat Attributes(Seed: 67890, Num Traits: 2, Min Rarity: Common)
 * 
 * // Generate complete monster attributes
 * Monster Attributes = Generate Monster Attributes(Seed: 11111, Climate Value: 0.7, Terrain Value: 0.5, Num Traits: 2)
 * ```
 */
UCLASS()
class ECHOALCHEMIST_API UAttributeGenerator : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Generates ecology attributes based on world state and random seed.
     * 
     * @param Seed Random seed for deterministic generation.
     * @param ClimateValue Climate value from world morphing system (0-1).
     * @param TerrainValue Terrain value from world morphing system (0-1).
     * @return Generated ecology attributes.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate ecology attributes influenced by world state
     * Ecology Attributes = Generate Ecology Attributes(Seed: 12345, Climate Value: 0.7, Terrain Value: 0.5)
     * // Result: Habitat and stats influenced by climate and terrain
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Attributes", meta = (DisplayName = "Generate Ecology Attributes", Keywords = "monster ecology attributes generator"))
    static FEcologyAttributes GenerateEcologyAttributes(int32 Seed, float ClimateValue, float TerrainValue);

    /**
     * Generates combat attributes with a specified number of traits.
     * 
     * @param Seed Random seed for deterministic generation.
     * @param NumTraits Number of combat traits to generate (1-3).
     * @param MinRarity Minimum rarity level for generated traits.
     * @return Generated combat attributes.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate 2 combat traits with at least Rare rarity
     * Combat Attributes = Generate Combat Attributes(Seed: 67890, Num Traits: 2, Min Rarity: Rare)
     * // Result: 2 traits with Rare or Epic rarity
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Attributes", meta = (DisplayName = "Generate Combat Attributes", Keywords = "monster combat attributes traits generator"))
    static FCombatAttributes GenerateCombatAttributes(int32 Seed, int32 NumTraits = 2, ERarity MinRarity = ERarity::Common);

    /**
     * Generates complete monster attributes (ecology + combat).
     * 
     * @param Seed Random seed for deterministic generation.
     * @param ClimateValue Climate value from world morphing system (0-1).
     * @param TerrainValue Terrain value from world morphing system (0-1).
     * @param NumTraits Number of combat traits to generate (1-3).
     * @return Complete monster attributes.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate complete monster attributes
     * Monster Attributes = Generate Monster Attributes(Seed: 11111, Climate Value: 0.7, Terrain Value: 0.5, Num Traits: 2)
     * // Result: Full monster with ecology and combat attributes
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Attributes", meta = (DisplayName = "Generate Monster Attributes", Keywords = "monster attributes generator complete"))
    static FMonsterAttributes GenerateMonsterAttributes(int32 Seed, float ClimateValue, float TerrainValue, int32 NumTraits = 2);

    /**
     * Generates a random habitat type based on climate and terrain values.
     * 
     * @param Seed Random seed for deterministic generation.
     * @param ClimateValue Climate value from world morphing system (0-1).
     * @param TerrainValue Terrain value from world morphing system (0-1).
     * @return Generated habitat type.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate habitat based on world state
     * Habitat = Generate Habitat Type(Seed: 12345, Climate Value: 0.8, Terrain Value: 0.3)
     * // Result: Desert (hot climate, flat terrain)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Attributes", meta = (DisplayName = "Generate Habitat Type", Keywords = "habitat type generator"))
    static EHabitatType GenerateHabitatType(int32 Seed, float ClimateValue, float TerrainValue);

    /**
     * Generates a random size class based on ecology attributes.
     * 
     * @param Seed Random seed for deterministic generation.
     * @param Habitat Habitat type (affects size distribution).
     * @return Generated size class.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate size class based on habitat
     * Size Class = Generate Size Class(Seed: 67890, Habitat: Forest)
     * // Result: Large (forests support larger creatures)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Attributes", meta = (DisplayName = "Generate Size Class", Keywords = "size class generator"))
    static ESizeClass GenerateSizeClass(int32 Seed, EHabitatType Habitat);

    /**
     * Generates a random combat trait.
     * 
     * @param Seed Random seed for deterministic generation.
     * @param MinRarity Minimum rarity level for the trait.
     * @return Generated combat trait.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate a random combat trait
     * Trait = Generate Combat Trait(Seed: 11111, Min Rarity: Rare)
     * // Result: A random trait with at least Rare rarity
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Attributes", meta = (DisplayName = "Generate Combat Trait", Keywords = "combat trait generator"))
    static FCombatTrait GenerateCombatTrait(int32 Seed, ERarity MinRarity = ERarity::Common);

    /**
     * Calculates base stats (HP, ATK, DEF, SPD) from ecology attributes.
     * 
     * @param EcologyAttributes The ecology attributes to calculate stats from.
     * @param OutHP Output health points.
     * @param OutATK Output attack power.
     * @param OutDEF Output defense power.
     * @param OutSPD Output speed.
     * 
     * **Blueprint Usage:**
     * ```
     * // Calculate base stats from ecology attributes
     * Calculate Base Stats From Ecology(Ecology Attributes, HP, ATK, DEF, SPD)
     * // Result: HP, ATK, DEF, SPD values based on habitat and size
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Attributes", meta = (DisplayName = "Calculate Base Stats From Ecology", Keywords = "stats calculator ecology"))
    static void CalculateBaseStatsFromEcology(const FEcologyAttributes& EcologyAttributes, float& OutHP, float& OutATK, float& OutDEF, float& OutSPD);

private:

    /** Helper function to get a seeded random stream */
    static FRandomStream GetSeededRandomStream(int32 Seed);

    /** Helper function to select a random enum value */
    template<typename TEnum>
    static TEnum GetRandomEnumValue(FRandomStream& RandomStream, int32 MaxValue);
};
