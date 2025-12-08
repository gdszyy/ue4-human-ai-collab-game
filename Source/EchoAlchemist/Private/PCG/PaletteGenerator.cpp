#include "PCG/PaletteGenerator.h"
#include "Math/UnrealMathUtility.h"

FPalette UPaletteGenerator::GeneratePaletteFromSpectrum(const TArray<FLinearColor>& Spectrum, int32 NumColors)
{
    FPalette Palette;
    if (Spectrum.Num() == 0 || NumColors <= 0)
    {
        return Palette;
    }

    for (int32 i = 0; i < NumColors; ++i)
    {
        float Alpha = (NumColors == 1) ? 0.0f : (float)i / (float)(NumColors - 1);
        int32 Index = FMath::FloorToInt(Alpha * (Spectrum.Num() - 1));
        float LerpAlpha = FMath::Frac(Alpha * (Spectrum.Num() - 1));

        FLinearColor Color = FLinearColor::LerpUsingHSV(Spectrum[Index], Spectrum[FMath::Min(Index + 1, Spectrum.Num() - 1)], LerpAlpha);
        Palette.Colors.Add(Color);
    }

    return Palette;
}

FPalette UPaletteGenerator::GenerateMonochromaticPalette(FLinearColor BaseColor, int32 NumColors, FVector2D SaturationRange, FVector2D ValueRange)
{
    FPalette Palette;
    if (NumColors <= 0)
    {
        return Palette;
    }

    FLinearColor HSV = BaseColor.LinearRGBToHSV();

    for (int32 i = 0; i < NumColors; ++i)
    {
        float Alpha = (NumColors == 1) ? 0.5f : (float)i / (float)(NumColors - 1);
        float NewSaturation = FMath::Lerp(SaturationRange.X, SaturationRange.Y, Alpha);
        float NewValue = FMath::Lerp(ValueRange.X, ValueRange.Y, Alpha);

        FLinearColor NewColor = FLinearColor(HSV.R, NewSaturation, NewValue).HSVToLinearRGB();
        Palette.Colors.Add(NewColor);
    }

    return Palette;
}

// ========== Habitat-specific color spectrums ==========

TArray<FLinearColor> UPaletteGenerator::GetForestSpectrum()
{
    TArray<FLinearColor> Spectrum;
    // Forest: Green, brown, earthy tones
    Spectrum.Add(FLinearColor(0.2f, 0.4f, 0.1f));  // Dark green
    Spectrum.Add(FLinearColor(0.3f, 0.6f, 0.2f));  // Medium green
    Spectrum.Add(FLinearColor(0.4f, 0.3f, 0.1f));  // Brown
    Spectrum.Add(FLinearColor(0.5f, 0.7f, 0.3f));  // Light green
    return Spectrum;
}

TArray<FLinearColor> UPaletteGenerator::GetDesertSpectrum()
{
    TArray<FLinearColor> Spectrum;
    // Desert: Yellow, orange, sandy tones
    Spectrum.Add(FLinearColor(0.8f, 0.6f, 0.2f));  // Sand
    Spectrum.Add(FLinearColor(0.9f, 0.7f, 0.3f));  // Light sand
    Spectrum.Add(FLinearColor(0.7f, 0.4f, 0.1f));  // Dark orange
    Spectrum.Add(FLinearColor(0.9f, 0.5f, 0.2f));  // Orange
    return Spectrum;
}

TArray<FLinearColor> UPaletteGenerator::GetTundraSpectrum()
{
    TArray<FLinearColor> Spectrum;
    // Tundra: White, blue, icy tones
    Spectrum.Add(FLinearColor(0.7f, 0.8f, 0.9f));  // Ice blue
    Spectrum.Add(FLinearColor(0.9f, 0.9f, 1.0f));  // White
    Spectrum.Add(FLinearColor(0.4f, 0.6f, 0.8f));  // Deep blue
    Spectrum.Add(FLinearColor(0.6f, 0.7f, 0.85f)); // Light blue
    return Spectrum;
}

TArray<FLinearColor> UPaletteGenerator::GetCaveSpectrum()
{
    TArray<FLinearColor> Spectrum;
    // Cave: Gray, dark, stone tones
    Spectrum.Add(FLinearColor(0.2f, 0.2f, 0.2f));  // Dark gray
    Spectrum.Add(FLinearColor(0.4f, 0.4f, 0.4f));  // Medium gray
    Spectrum.Add(FLinearColor(0.3f, 0.3f, 0.35f)); // Blue-gray
    Spectrum.Add(FLinearColor(0.5f, 0.5f, 0.5f));  // Light gray
    return Spectrum;
}

TArray<FLinearColor> UPaletteGenerator::GetVolcanoSpectrum()
{
    TArray<FLinearColor> Spectrum;
    // Volcano: Red, orange, fiery tones
    Spectrum.Add(FLinearColor(0.8f, 0.2f, 0.1f));  // Deep red
    Spectrum.Add(FLinearColor(1.0f, 0.4f, 0.1f));  // Orange-red
    Spectrum.Add(FLinearColor(0.9f, 0.6f, 0.2f));  // Orange
    Spectrum.Add(FLinearColor(1.0f, 0.8f, 0.3f));  // Yellow-orange
    return Spectrum;
}

TArray<FLinearColor> UPaletteGenerator::GetSwampSpectrum()
{
    TArray<FLinearColor> Spectrum;
    // Swamp: Dark green, murky, toxic tones
    Spectrum.Add(FLinearColor(0.2f, 0.3f, 0.1f));  // Dark murky green
    Spectrum.Add(FLinearColor(0.3f, 0.4f, 0.2f));  // Swamp green
    Spectrum.Add(FLinearColor(0.4f, 0.5f, 0.3f));  // Moss green
    Spectrum.Add(FLinearColor(0.5f, 0.6f, 0.2f));  // Toxic green
    return Spectrum;
}

TArray<FLinearColor> UPaletteGenerator::GetHabitatSpectrum(EHabitatType Habitat)
{
    switch (Habitat)
    {
    case EHabitatType::Forest:
        return GetForestSpectrum();
    case EHabitatType::Desert:
        return GetDesertSpectrum();
    case EHabitatType::Tundra:
        return GetTundraSpectrum();
    case EHabitatType::Cave:
        return GetCaveSpectrum();
    case EHabitatType::Volcano:
        return GetVolcanoSpectrum();
    case EHabitatType::Swamp:
        return GetSwampSpectrum();
    default:
        return GetForestSpectrum();
    }
}

// ========== Ecology-based palette generation ==========

FPalette UPaletteGenerator::GeneratePaletteFromEcology(const FEcologyAttributes& EcologyAttributes, int32 Seed)
{
    // Get base spectrum for habitat
    TArray<FLinearColor> BaseSpectrum = GetHabitatSpectrum(EcologyAttributes.Habitat);
    
    // Generate a palette with 5 colors (primary, secondary, tertiary, highlight, shadow)
    FPalette Palette = GeneratePaletteFromSpectrum(BaseSpectrum, 5);
    
    // Adjust based on tolerance (affects color intensity)
    // Higher tolerance = more saturated colors
    float ToleranceFactor = EcologyAttributes.Tolerance / 100.0f;
    for (FLinearColor& Color : Palette.Colors)
    {
        FLinearColor HSV = Color.LinearRGBToHSV();
        HSV.G = FMath::Lerp(0.3f, 1.0f, ToleranceFactor); // Adjust saturation
        Color = HSV.HSVToLinearRGB();
    }
    
    return Palette;
}

// ========== Rarity-based palette adjustment ==========

FPalette UPaletteGenerator::AdjustPaletteForRarity(const FPalette& BasePalette, ERarity Rarity)
{
    FPalette AdjustedPalette = BasePalette;
    
    float SaturationMultiplier = 1.0f;
    float ValueMultiplier = 1.0f;
    
    switch (Rarity)
    {
    case ERarity::Common:
        SaturationMultiplier = 0.8f;
        ValueMultiplier = 0.9f;
        break;
    case ERarity::Rare:
        SaturationMultiplier = 1.1f;
        ValueMultiplier = 1.1f;
        break;
    case ERarity::Epic:
        SaturationMultiplier = 1.3f;
        ValueMultiplier = 1.2f;
        break;
    }
    
    for (FLinearColor& Color : AdjustedPalette.Colors)
    {
        FLinearColor HSV = Color.LinearRGBToHSV();
        HSV.G = FMath::Clamp(HSV.G * SaturationMultiplier, 0.0f, 1.0f);
        HSV.B = FMath::Clamp(HSV.B * ValueMultiplier, 0.0f, 1.0f);
        Color = HSV.HSVToLinearRGB();
    }
    
    return AdjustedPalette;
}

// ========== Complete monster palette generation ==========

FPalette UPaletteGenerator::GenerateMonsterPalette(const FMonsterAttributes& MonsterAttributes, int32 Seed)
{
    // Start with ecology-based palette
    FPalette Palette = GeneratePaletteFromEcology(MonsterAttributes.EcologyAttributes, Seed);
    
    // Adjust based on highest rarity combat trait
    ERarity HighestRarity = ERarity::Common;
    for (const FCombatTrait& Trait : MonsterAttributes.CombatAttributes.Traits)
    {
        if (static_cast<uint8>(Trait.Rarity) > static_cast<uint8>(HighestRarity))
        {
            HighestRarity = Trait.Rarity;
        }
    }
    
    Palette = AdjustPaletteForRarity(Palette, HighestRarity);
    
    return Palette;
}
