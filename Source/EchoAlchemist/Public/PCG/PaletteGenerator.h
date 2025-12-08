#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PCG/MonsterAttributes.h"
#include "PaletteGenerator.generated.h"

/**
 * @struct FPalette
 * @brief Represents a color palette containing an array of colors.
 */
USTRUCT(BlueprintType)
struct FPalette
{
    GENERATED_BODY()

    /** The colors in this palette. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> Colors;
};

/**
 * @class UPaletteGenerator
 * @brief A Blueprint Function Library for generating color palettes procedurally.
 */
UCLASS()
class ECHOALCHEMIST_API UPaletteGenerator : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Generates a color palette from a given spectrum of colors.
     *
     * @param Spectrum The base colors to generate the palette from.
     * @param NumColors The number of colors to generate in the final palette.
     * @return A new FPalette struct containing the generated colors.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Palette From Spectrum", Keywords = "color palette generator spectrum"))
    static FPalette GeneratePaletteFromSpectrum(const TArray<FLinearColor>& Spectrum, int32 NumColors);

    /**
     * Generates a monochromatic palette from a base color.
     *
     * @param BaseColor The base color for the palette.
     * @param NumColors The number of colors to generate.
     * @param SaturationRange The range of saturation values to use.
     * @param ValueRange The range of value (brightness) values to use.
     * @return A new FPalette struct containing the generated monochromatic colors.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Monochromatic Palette", Keywords = "color palette generator mono"))
    static FPalette GenerateMonochromaticPalette(FLinearColor BaseColor, int32 NumColors, FVector2D SaturationRange = FVector2D(0.2f, 1.0f), FVector2D ValueRange = FVector2D(0.3f, 1.0f));

    /**
     * Generates a color palette based on ecology attributes (habitat type).
     *
     * @param EcologyAttributes The ecology attributes of the monster.
     * @param Seed Random seed for deterministic generation.
     * @return A new FPalette struct containing colors appropriate for the habitat.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Palette From Ecology", Keywords = "color palette generator ecology habitat"))
    static FPalette GeneratePaletteFromEcology(const FEcologyAttributes& EcologyAttributes, int32 Seed = 0);

    /**
     * Generates a complete monster palette based on both ecology and combat attributes.
     *
     * @param MonsterAttributes The complete monster attributes.
     * @param Seed Random seed for deterministic generation.
     * @return A new FPalette struct containing the monster's color scheme.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Monster Palette", Keywords = "color palette generator monster"))
    static FPalette GenerateMonsterPalette(const FMonsterAttributes& MonsterAttributes, int32 Seed = 0);

    /**
     * Gets the base color spectrum for a given habitat type.
     *
     * @param Habitat The habitat type.
     * @return An array of base colors for the habitat.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Get Habitat Spectrum", Keywords = "color spectrum habitat"))
    static TArray<FLinearColor> GetHabitatSpectrum(EHabitatType Habitat);

    /**
     * Adjusts a palette based on combat trait rarity.
     *
     * @param BasePalette The base palette to adjust.
     * @param Rarity The rarity level of the combat trait.
     * @return An adjusted FPalette with modified saturation and brightness.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Adjust Palette For Rarity", Keywords = "color palette adjust rarity"))
    static FPalette AdjustPaletteForRarity(const FPalette& BasePalette, ERarity Rarity);

private:
    /** Helper function to get habitat-specific base colors */
    static TArray<FLinearColor> GetForestSpectrum();
    static TArray<FLinearColor> GetDesertSpectrum();
    static TArray<FLinearColor> GetTundraSpectrum();
    static TArray<FLinearColor> GetCaveSpectrum();
    static TArray<FLinearColor> GetVolcanoSpectrum();
    static TArray<FLinearColor> GetSwampSpectrum();
};
