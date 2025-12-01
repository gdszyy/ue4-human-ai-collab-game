// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PCG/MonsterAttributes.h"
#include "PCG/AppearanceLibrary.h"
#include "AppearanceAssembler.generated.h"

/**
 * @class UAppearanceAssembler
 * @brief A Blueprint Function Library for assembling monster appearance from attributes.
 * 
 * This class provides functions to select skeletons, parts, and palettes based on
 * monster attributes, and to apply palette swapping for runtime color replacement.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Select skeleton based on ecology attributes
 * Skeleton Data = Select Skeleton(Ecology Attributes, Skeleton Data Table)
 * 
 * // Select parts based on combat attributes
 * Part Data Array = Select Parts(Combat Attributes, Part Data Table)
 * 
 * // Get palette for habitat
 * Palette Data = Get Palette For Habitat(Habitat, Palette Data Table)
 * ```
 */
UCLASS()
class ECHOALCHEMIST_API UAppearanceAssembler : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Selects a skeleton based on ecology attributes.
     * 
     * @param EcologyAttributes The ecology attributes to select skeleton from.
     * @param SkeletonDataTable Data table containing skeleton definitions.
     * @param OutSkeletonData Output skeleton data.
     * @return True if a skeleton was found, false otherwise.
     * 
     * **Blueprint Usage:**
     * ```
     * // Select skeleton for a forest medium monster
     * Select Skeleton(Ecology Attributes, DT_Skeletons, Skeleton Data) → Success
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Appearance", meta = (DisplayName = "Select Skeleton", Keywords = "skeleton select appearance"))
    static bool SelectSkeleton(const FEcologyAttributes& EcologyAttributes, UDataTable* SkeletonDataTable, FSkeletonData& OutSkeletonData);

    /**
     * Selects parts based on combat attributes.
     * 
     * @param CombatAttributes The combat attributes to select parts from.
     * @param PartDataTable Data table containing part definitions.
     * @return Array of selected part data.
     * 
     * **Blueprint Usage:**
     * ```
     * // Select parts for combat traits
     * Part Data Array = Select Parts(Combat Attributes, DT_Parts)
     * // Result: Array of parts matching the trait's AssociatedPartIDs
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Appearance", meta = (DisplayName = "Select Parts", Keywords = "parts select appearance"))
    static TArray<FPartData> SelectParts(const FCombatAttributes& CombatAttributes, UDataTable* PartDataTable);

    /**
     * Gets the palette for a specific habitat.
     * 
     * @param Habitat The habitat type to get palette for.
     * @param PaletteDataTable Data table containing palette definitions.
     * @param OutPaletteData Output palette data.
     * @return True if a palette was found, false otherwise.
     * 
     * **Blueprint Usage:**
     * ```
     * // Get palette for forest habitat
     * Get Palette For Habitat(Forest, DT_Palettes, Palette Data) → Success
     * // Result: Palette Data with green colors
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Appearance", meta = (DisplayName = "Get Palette For Habitat", Keywords = "palette habitat color"))
    static bool GetPaletteForHabitat(EHabitatType Habitat, UDataTable* PaletteDataTable, FPaletteData& OutPaletteData);

    /**
     * Creates a dynamic material instance with palette swapping parameters.
     * 
     * @param BaseMaterial The base material to create instance from.
     * @param PaletteData The palette data to apply.
     * @return Dynamic material instance with palette parameters set.
     * 
     * **Blueprint Usage:**
     * ```
     * // Create palette swap material
     * Material Instance = Create Palette Swap Material(M_PaletteSwap_Master, Palette Data)
     * // Apply to sprite component
     * Sprite Component → Set Material(0, Material Instance)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Appearance", meta = (DisplayName = "Create Palette Swap Material", Keywords = "palette swap material color"))
    static UMaterialInstanceDynamic* CreatePaletteSwapMaterial(UMaterialInterface* BaseMaterial, const FPaletteData& PaletteData);

    /**
     * Gets the anchor point location for a part.
     * 
     * @param AnchorPointName The name of the anchor point.
     * @param SkeletonData The skeleton data containing anchor points.
     * @return The relative location of the anchor point.
     * 
     * **Blueprint Usage:**
     * ```
     * // Get anchor point location
     * Location = Get Anchor Point Location("Head", Skeleton Data)
     * // Use to position part sprite
     * Part Component → Set Relative Location(Location)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Appearance", meta = (DisplayName = "Get Anchor Point Location", Keywords = "anchor point location"))
    static FVector GetAnchorPointLocation(FName AnchorPointName, const FSkeletonData& SkeletonData);

    /**
     * Calculates the final scale for a monster based on size class.
     * 
     * @param SizeClass The size class of the monster.
     * @param BaseScale The base scale from skeleton data.
     * @return The final scale multiplier.
     * 
     * **Blueprint Usage:**
     * ```
     * // Calculate final scale
     * Final Scale = Calculate Monster Scale(Large, 1.0)
     * // Result: 1.5 (Large size multiplier)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Appearance", meta = (DisplayName = "Calculate Monster Scale", Keywords = "scale size monster"))
    static float CalculateMonsterScale(ESizeClass SizeClass, float BaseScale = 1.0f);

private:

    /** Default anchor points for skeletons (fallback if not defined in data table) */
    static TMap<FName, FVector> GetDefaultAnchorPoints();
};
