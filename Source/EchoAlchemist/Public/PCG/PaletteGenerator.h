#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
};
