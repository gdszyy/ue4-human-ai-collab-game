#include "PCG/PaletteGenerator.h"

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
