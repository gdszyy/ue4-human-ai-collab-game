// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/AppearanceAssembler.h"
#include "Materials/MaterialInstanceDynamic.h"

bool UAppearanceAssembler::SelectSkeleton(const FEcologyAttributes& EcologyAttributes, UDataTable* SkeletonDataTable, FSkeletonData& OutSkeletonData)
{
    if (!SkeletonDataTable)
    {
        return false;
    }

    // Get all skeleton rows
    TArray<FSkeletonData*> AllSkeletons;
    SkeletonDataTable->GetAllRows<FSkeletonData>(TEXT("SelectSkeleton"), AllSkeletons);

    // Find matching skeletons (habitat and size class)
    TArray<FSkeletonData*> MatchingSkeletons;
    for (FSkeletonData* Skeleton : AllSkeletons)
    {
        if (Skeleton && Skeleton->Habitat == EcologyAttributes.Habitat && Skeleton->SizeClass == EcologyAttributes.SizeClass)
        {
            MatchingSkeletons.Add(Skeleton);
        }
    }

    // If no exact match, try to find skeleton with matching habitat only
    if (MatchingSkeletons.Num() == 0)
    {
        for (FSkeletonData* Skeleton : AllSkeletons)
        {
            if (Skeleton && Skeleton->Habitat == EcologyAttributes.Habitat)
            {
                MatchingSkeletons.Add(Skeleton);
            }
        }
    }

    // If still no match, use any skeleton
    if (MatchingSkeletons.Num() == 0)
    {
        MatchingSkeletons = AllSkeletons;
    }

    // Select the first matching skeleton
    if (MatchingSkeletons.Num() > 0 && MatchingSkeletons[0])
    {
        OutSkeletonData = *MatchingSkeletons[0];
        return true;
    }

    return false;
}

TArray<FPartData> UAppearanceAssembler::SelectParts(const FCombatAttributes& CombatAttributes, UDataTable* PartDataTable)
{
    TArray<FPartData> SelectedParts;

    if (!PartDataTable)
    {
        return SelectedParts;
    }

    // Get all part rows
    TArray<FPartData*> AllParts;
    PartDataTable->GetAllRows<FPartData>(TEXT("SelectParts"), AllParts);

    // For each combat trait, find matching parts
    for (const FCombatTrait& Trait : CombatAttributes.Traits)
    {
        for (const FName& PartID : Trait.AssociatedPartIDs)
        {
            // Find part with matching ID
            for (FPartData* Part : AllParts)
            {
                if (Part && Part->PartID == PartID)
                {
                    SelectedParts.Add(*Part);
                    break;
                }
            }
        }
    }

    return SelectedParts;
}

bool UAppearanceAssembler::GetPaletteForHabitat(EHabitatType Habitat, UDataTable* PaletteDataTable, FPaletteData& OutPaletteData)
{
    if (!PaletteDataTable)
    {
        return false;
    }

    // Get all palette rows
    TArray<FPaletteData*> AllPalettes;
    PaletteDataTable->GetAllRows<FPaletteData>(TEXT("GetPaletteForHabitat"), AllPalettes);

    // Find matching palette
    for (FPaletteData* Palette : AllPalettes)
    {
        if (Palette && Palette->Habitat == Habitat)
        {
            OutPaletteData = *Palette;
            return true;
        }
    }

    // Fallback: create default palette based on habitat
    OutPaletteData.Habitat = Habitat;
    switch (Habitat)
    {
    case EHabitatType::Forest:
        OutPaletteData.PrimaryColor = FLinearColor(0.2f, 0.8f, 0.2f, 1.0f); // Green
        OutPaletteData.SecondaryColor = FLinearColor(0.4f, 0.6f, 0.3f, 1.0f); // Dark Green
        OutPaletteData.AccentColor = FLinearColor(0.8f, 0.7f, 0.3f, 1.0f); // Yellow
        break;
    case EHabitatType::Desert:
        OutPaletteData.PrimaryColor = FLinearColor(0.9f, 0.7f, 0.3f, 1.0f); // Sand Yellow
        OutPaletteData.SecondaryColor = FLinearColor(0.7f, 0.5f, 0.2f, 1.0f); // Brown
        OutPaletteData.AccentColor = FLinearColor(1.0f, 0.9f, 0.7f, 1.0f); // Light Yellow
        break;
    case EHabitatType::Tundra:
        OutPaletteData.PrimaryColor = FLinearColor(0.7f, 0.9f, 1.0f, 1.0f); // Ice Blue
        OutPaletteData.SecondaryColor = FLinearColor(0.5f, 0.7f, 0.9f, 1.0f); // Dark Blue
        OutPaletteData.AccentColor = FLinearColor(0.9f, 0.95f, 1.0f, 1.0f); // White
        break;
    case EHabitatType::Cave:
        OutPaletteData.PrimaryColor = FLinearColor(0.4f, 0.4f, 0.5f, 1.0f); // Dark Gray
        OutPaletteData.SecondaryColor = FLinearColor(0.3f, 0.3f, 0.4f, 1.0f); // Darker Gray
        OutPaletteData.AccentColor = FLinearColor(0.6f, 0.6f, 0.7f, 1.0f); // Light Gray
        break;
    case EHabitatType::Volcano:
        OutPaletteData.PrimaryColor = FLinearColor(1.0f, 0.3f, 0.1f, 1.0f); // Lava Red
        OutPaletteData.SecondaryColor = FLinearColor(0.8f, 0.2f, 0.0f, 1.0f); // Dark Red
        OutPaletteData.AccentColor = FLinearColor(1.0f, 0.7f, 0.2f, 1.0f); // Orange
        break;
    case EHabitatType::Swamp:
        OutPaletteData.PrimaryColor = FLinearColor(0.4f, 0.6f, 0.3f, 1.0f); // Murky Green
        OutPaletteData.SecondaryColor = FLinearColor(0.3f, 0.5f, 0.2f, 1.0f); // Dark Green
        OutPaletteData.AccentColor = FLinearColor(0.6f, 0.7f, 0.4f, 1.0f); // Light Green
        break;
    default:
        OutPaletteData.PrimaryColor = FLinearColor::White;
        OutPaletteData.SecondaryColor = FLinearColor::Gray;
        OutPaletteData.AccentColor = FLinearColor::White;
        break;
    }

    return true;
}

UMaterialInstanceDynamic* UAppearanceAssembler::CreatePaletteSwapMaterial(UMaterialInterface* BaseMaterial, const FPaletteData& PaletteData)
{
    if (!BaseMaterial)
    {
        return nullptr;
    }

    // Create dynamic material instance
    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr);

    if (DynamicMaterial)
    {
        // Set palette parameters
        DynamicMaterial->SetVectorParameterValue(TEXT("PrimaryColor"), PaletteData.PrimaryColor);
        DynamicMaterial->SetVectorParameterValue(TEXT("SecondaryColor"), PaletteData.SecondaryColor);
        DynamicMaterial->SetVectorParameterValue(TEXT("AccentColor"), PaletteData.AccentColor);
    }

    return DynamicMaterial;
}

FVector UAppearanceAssembler::GetAnchorPointLocation(FName AnchorPointName, const FSkeletonData& SkeletonData)
{
    // In a full implementation, this would look up anchor points from the skeleton data
    // For now, use default anchor points
    TMap<FName, FVector> DefaultAnchors = GetDefaultAnchorPoints();

    if (DefaultAnchors.Contains(AnchorPointName))
    {
        return DefaultAnchors[AnchorPointName];
    }

    return FVector::ZeroVector;
}

float UAppearanceAssembler::CalculateMonsterScale(ESizeClass SizeClass, float BaseScale)
{
    float SizeMultiplier = 1.0f;

    switch (SizeClass)
    {
    case ESizeClass::Tiny:
        SizeMultiplier = 0.5f;
        break;
    case ESizeClass::Small:
        SizeMultiplier = 0.75f;
        break;
    case ESizeClass::Medium:
        SizeMultiplier = 1.0f;
        break;
    case ESizeClass::Large:
        SizeMultiplier = 1.5f;
        break;
    case ESizeClass::Giant:
        SizeMultiplier = 2.5f;
        break;
    }

    return BaseScale * SizeMultiplier;
}

TMap<FName, FVector> UAppearanceAssembler::GetDefaultAnchorPoints()
{
    TMap<FName, FVector> DefaultAnchors;

    // Define default anchor points
    DefaultAnchors.Add(TEXT("Head"), FVector(0.0f, 0.0f, 50.0f));
    DefaultAnchors.Add(TEXT("Back"), FVector(0.0f, 0.0f, 30.0f));
    DefaultAnchors.Add(TEXT("Tail"), FVector(-30.0f, 0.0f, 10.0f));
    DefaultAnchors.Add(TEXT("FrontLeft"), FVector(20.0f, -15.0f, 0.0f));
    DefaultAnchors.Add(TEXT("FrontRight"), FVector(20.0f, 15.0f, 0.0f));
    DefaultAnchors.Add(TEXT("BackLeft"), FVector(-20.0f, -15.0f, 0.0f));
    DefaultAnchors.Add(TEXT("BackRight"), FVector(-20.0f, 15.0f, 0.0f));

    return DefaultAnchors;
}
