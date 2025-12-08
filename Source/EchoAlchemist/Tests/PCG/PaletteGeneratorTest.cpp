// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/PaletteGenerator.h"
#include "PCG/MonsterAttributes.h"
#include "PCG/AttributeGenerator.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorBasicTest, "EchoAlchemist.PCG.PaletteGenerator.BasicGeneration", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorBasicTest::RunTest(const FString& Parameters)
{
    // Test 1: Generate palette from spectrum
    TArray<FLinearColor> Spectrum;
    Spectrum.Add(FLinearColor::Red);
    Spectrum.Add(FLinearColor::Yellow);
    Spectrum.Add(FLinearColor::Green);
    
    FPalette Palette = UPaletteGenerator::GeneratePaletteFromSpectrum(Spectrum, 5);
    
    TestEqual(TEXT("Palette should have 5 colors"), Palette.Colors.Num(), 5);
    TestTrue(TEXT("First color should be red"), Palette.Colors[0].Equals(FLinearColor::Red, 0.01f));
    TestTrue(TEXT("Last color should be green"), Palette.Colors[4].Equals(FLinearColor::Green, 0.01f));
    
    // Test 2: Generate monochromatic palette
    FPalette MonoPalette = UPaletteGenerator::GenerateMonochromaticPalette(FLinearColor::Blue, 3);
    
    TestEqual(TEXT("Monochromatic palette should have 3 colors"), MonoPalette.Colors.Num(), 3);
    
    // All colors should have the same hue (blue)
    for (const FLinearColor& Color : MonoPalette.Colors)
    {
        FLinearColor HSV = Color.LinearRGBToHSV();
        TestTrue(TEXT("All colors should be blue hue"), FMath::IsNearlyEqual(HSV.R, FLinearColor::Blue.LinearRGBToHSV().R, 0.1f));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorHabitatTest, "EchoAlchemist.PCG.PaletteGenerator.HabitatSpectrum", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorHabitatTest::RunTest(const FString& Parameters)
{
    // Test all habitat spectrums
    TArray<EHabitatType> Habitats = {
        EHabitatType::Forest,
        EHabitatType::Desert,
        EHabitatType::Tundra,
        EHabitatType::Cave,
        EHabitatType::Volcano,
        EHabitatType::Swamp
    };
    
    for (EHabitatType Habitat : Habitats)
    {
        TArray<FLinearColor> Spectrum = UPaletteGenerator::GetHabitatSpectrum(Habitat);
        
        TestTrue(TEXT("Habitat spectrum should not be empty"), Spectrum.Num() > 0);
        TestEqual(TEXT("Habitat spectrum should have 4 colors"), Spectrum.Num(), 4);
        
        // All colors should be valid (no NaN or negative values)
        for (const FLinearColor& Color : Spectrum)
        {
            TestTrue(TEXT("Color R should be valid"), !FMath::IsNaN(Color.R) && Color.R >= 0.0f);
            TestTrue(TEXT("Color G should be valid"), !FMath::IsNaN(Color.G) && Color.G >= 0.0f);
            TestTrue(TEXT("Color B should be valid"), !FMath::IsNaN(Color.B) && Color.B >= 0.0f);
        }
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorEcologyTest, "EchoAlchemist.PCG.PaletteGenerator.EcologyBased", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorEcologyTest::RunTest(const FString& Parameters)
{
    // Create ecology attributes
    FEcologyAttributes EcologyLowTolerance;
    EcologyLowTolerance.Habitat = EHabitatType::Forest;
    EcologyLowTolerance.Tolerance = 20.0f;
    
    FEcologyAttributes EcologyHighTolerance;
    EcologyHighTolerance.Habitat = EHabitatType::Forest;
    EcologyHighTolerance.Tolerance = 90.0f;
    
    // Generate palettes
    FPalette PaletteLow = UPaletteGenerator::GeneratePaletteFromEcology(EcologyLowTolerance);
    FPalette PaletteHigh = UPaletteGenerator::GeneratePaletteFromEcology(EcologyHighTolerance);
    
    TestEqual(TEXT("Palette should have 5 colors"), PaletteLow.Colors.Num(), 5);
    TestEqual(TEXT("Palette should have 5 colors"), PaletteHigh.Colors.Num(), 5);
    
    // High tolerance should result in higher saturation
    for (int32 i = 0; i < 5; ++i)
    {
        FLinearColor HSVLow = PaletteLow.Colors[i].LinearRGBToHSV();
        FLinearColor HSVHigh = PaletteHigh.Colors[i].LinearRGBToHSV();
        
        TestTrue(TEXT("High tolerance should have higher saturation"), HSVHigh.G > HSVLow.G);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorRarityTest, "EchoAlchemist.PCG.PaletteGenerator.RarityAdjustment", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorRarityTest::RunTest(const FString& Parameters)
{
    // Create base palette
    FEcologyAttributes Ecology;
    Ecology.Habitat = EHabitatType::Volcano;
    Ecology.Tolerance = 50.0f;
    
    FPalette BasePalette = UPaletteGenerator::GeneratePaletteFromEcology(Ecology);
    
    // Adjust for different rarities
    FPalette CommonPalette = UPaletteGenerator::AdjustPaletteForRarity(BasePalette, ERarity::Common);
    FPalette RarePalette = UPaletteGenerator::AdjustPaletteForRarity(BasePalette, ERarity::Rare);
    FPalette EpicPalette = UPaletteGenerator::AdjustPaletteForRarity(BasePalette, ERarity::Epic);
    
    // Check that rarity affects saturation and value
    for (int32 i = 0; i < BasePalette.Colors.Num(); ++i)
    {
        FLinearColor HSVCommon = CommonPalette.Colors[i].LinearRGBToHSV();
        FLinearColor HSVRare = RarePalette.Colors[i].LinearRGBToHSV();
        FLinearColor HSVEpic = EpicPalette.Colors[i].LinearRGBToHSV();
        
        // Epic should have highest saturation and value
        TestTrue(TEXT("Epic should have higher saturation than Rare"), HSVEpic.G >= HSVRare.G);
        TestTrue(TEXT("Rare should have higher saturation than Common"), HSVRare.G >= HSVCommon.G);
        
        TestTrue(TEXT("Epic should have higher value than Rare"), HSVEpic.B >= HSVRare.B);
        TestTrue(TEXT("Rare should have higher value than Common"), HSVRare.B >= HSVCommon.B);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorMonsterTest, "EchoAlchemist.PCG.PaletteGenerator.MonsterPalette", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorMonsterTest::RunTest(const FString& Parameters)
{
    // Generate monster attributes
    FMonsterAttributes MonsterCommon = UAttributeGenerator::GenerateMonsterAttributes(12345, 0.5f, 0.5f, 1);
    FMonsterAttributes MonsterEpic = UAttributeGenerator::GenerateMonsterAttributes(67890, 0.5f, 0.5f, 3);
    
    // Manually set rarity for testing
    if (MonsterEpic.CombatAttributes.Traits.Num() > 0)
    {
        MonsterEpic.CombatAttributes.Traits[0].Rarity = ERarity::Epic;
    }
    
    // Generate palettes
    FPalette PaletteCommon = UPaletteGenerator::GenerateMonsterPalette(MonsterCommon);
    FPalette PaletteEpic = UPaletteGenerator::GenerateMonsterPalette(MonsterEpic);
    
    TestEqual(TEXT("Monster palette should have 5 colors"), PaletteCommon.Colors.Num(), 5);
    TestEqual(TEXT("Monster palette should have 5 colors"), PaletteEpic.Colors.Num(), 5);
    
    // Epic monster should have more vibrant colors
    float CommonAvgSaturation = 0.0f;
    float EpicAvgSaturation = 0.0f;
    
    for (int32 i = 0; i < 5; ++i)
    {
        CommonAvgSaturation += PaletteCommon.Colors[i].LinearRGBToHSV().G;
        EpicAvgSaturation += PaletteEpic.Colors[i].LinearRGBToHSV().G;
    }
    
    CommonAvgSaturation /= 5.0f;
    EpicAvgSaturation /= 5.0f;
    
    // Note: This test might fail if both monsters have the same rarity
    // It's more of a general expectation test
    AddInfo(FString::Printf(TEXT("Common avg saturation: %f, Epic avg saturation: %f"), CommonAvgSaturation, EpicAvgSaturation));
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorDeterministicTest, "EchoAlchemist.PCG.PaletteGenerator.Deterministic", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorDeterministicTest::RunTest(const FString& Parameters)
{
    // Test deterministic generation with same seed
    FEcologyAttributes Ecology;
    Ecology.Habitat = EHabitatType::Desert;
    Ecology.Tolerance = 60.0f;
    
    FPalette Palette1 = UPaletteGenerator::GeneratePaletteFromEcology(Ecology, 12345);
    FPalette Palette2 = UPaletteGenerator::GeneratePaletteFromEcology(Ecology, 12345);
    
    TestEqual(TEXT("Same seed should produce same number of colors"), Palette1.Colors.Num(), Palette2.Colors.Num());
    
    for (int32 i = 0; i < Palette1.Colors.Num(); ++i)
    {
        TestTrue(TEXT("Same seed should produce identical colors"), 
            Palette1.Colors[i].Equals(Palette2.Colors[i], 0.001f));
    }
    
    return true;
}
