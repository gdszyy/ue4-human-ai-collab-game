// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/MonsterActor.h"
#include "PCG/AppearanceAssembler.h"
#include "Components/SceneComponent.h"

AMonsterActor::AMonsterActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create root component
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // Create base flipbook component
    BaseFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BaseFlipbook"));
    BaseFlipbookComponent->SetupAttachment(RootComponent);
}

void AMonsterActor::BeginPlay()
{
    Super::BeginPlay();

    // If attributes are set in the editor, reconstruct appearance
    if (MonsterAttributes.EcologyAttributes.Habitat != EHabitatType::Forest || 
        MonsterAttributes.CombatAttributes.Traits.Num() > 0)
    {
        ReconstructAppearance();
    }
}

void AMonsterActor::SetMonsterAttributes(const FMonsterAttributes& Attributes)
{
    MonsterAttributes = Attributes;
    ReconstructAppearance();
}

void AMonsterActor::ReconstructAppearance()
{
    // Clear existing appearance
    ClearAppearance();

    // Construct new appearance
    ConstructBaseSkeleton();
    AttachParts();
    ApplyPaletteSwapping();
    ApplyScale();
}

void AMonsterActor::PlayAnimation(EMonsterAnimationType AnimationType)
{
    if (!BaseFlipbookComponent || !CurrentSkeletonData.IdleFlipbook)
    {
        return;
    }

    UPaperFlipbook* FlipbookToPlay = nullptr;

    switch (AnimationType)
    {
    case EMonsterAnimationType::Idle:
        FlipbookToPlay = CurrentSkeletonData.IdleFlipbook;
        break;
    case EMonsterAnimationType::Walk:
        FlipbookToPlay = CurrentSkeletonData.WalkFlipbook;
        break;
    case EMonsterAnimationType::Attack:
        FlipbookToPlay = CurrentSkeletonData.AttackFlipbook;
        break;
    case EMonsterAnimationType::Death:
        FlipbookToPlay = CurrentSkeletonData.DeathFlipbook;
        break;
    }

    if (FlipbookToPlay)
    {
        BaseFlipbookComponent->SetFlipbook(FlipbookToPlay);
        BaseFlipbookComponent->Play();
    }
}

void AMonsterActor::ClearAppearance()
{
    // Destroy all part components
    for (UPaperSpriteComponent* PartComponent : PartComponents)
    {
        if (PartComponent)
        {
            PartComponent->DestroyComponent();
        }
    }
    PartComponents.Empty();

    // Clear base flipbook
    if (BaseFlipbookComponent)
    {
        BaseFlipbookComponent->SetFlipbook(nullptr);
    }
}

void AMonsterActor::ConstructBaseSkeleton()
{
    if (!SkeletonDataTable || !BaseFlipbookComponent)
    {
        return;
    }

    // Select skeleton based on ecology attributes
    FSkeletonData SkeletonData;
    if (UAppearanceAssembler::SelectSkeleton(MonsterAttributes.EcologyAttributes, SkeletonDataTable, SkeletonData))
    {
        CurrentSkeletonData = SkeletonData;

        // Set idle flipbook as default
        if (SkeletonData.IdleFlipbook)
        {
            BaseFlipbookComponent->SetFlipbook(SkeletonData.IdleFlipbook);
            BaseFlipbookComponent->Play();
        }
    }
}

void AMonsterActor::AttachParts()
{
    if (!PartDataTable || !BaseFlipbookComponent)
    {
        return;
    }

    // Select parts based on combat attributes
    TArray<FPartData> SelectedParts = UAppearanceAssembler::SelectParts(MonsterAttributes.CombatAttributes, PartDataTable);

    // Create and attach part components
    for (const FPartData& PartData : SelectedParts)
    {
        if (!PartData.PartSprite)
        {
            continue;
        }

        // Create part sprite component
        UPaperSpriteComponent* PartComponent = NewObject<UPaperSpriteComponent>(this);
        if (PartComponent)
        {
            PartComponent->RegisterComponent();
            PartComponent->AttachToComponent(BaseFlipbookComponent, FAttachmentTransformRules::KeepRelativeTransform);
            PartComponent->SetSprite(PartData.PartSprite);

            // Set relative transform
            PartComponent->SetRelativeLocation(PartData.RelativeLocation);
            PartComponent->SetRelativeRotation(PartData.RelativeRotation);
            PartComponent->SetRelativeScale3D(PartData.RelativeScale);

            // Set Z-order (translucent sort priority)
            PartComponent->SetTranslucentSortPriority(PartData.ZOrder);

            PartComponents.Add(PartComponent);
        }
    }
}

void AMonsterActor::ApplyPaletteSwapping()
{
    if (!PaletteDataTable || !PaletteSwapMaterial || !BaseFlipbookComponent)
    {
        return;
    }

    // Get palette for habitat
    FPaletteData PaletteData;
    if (UAppearanceAssembler::GetPaletteForHabitat(MonsterAttributes.EcologyAttributes.Habitat, PaletteDataTable, PaletteData))
    {
        CurrentPaletteData = PaletteData;

        // Create palette swap material
        UMaterialInstanceDynamic* DynamicMaterial = UAppearanceAssembler::CreatePaletteSwapMaterial(PaletteSwapMaterial, PaletteData);
        if (DynamicMaterial)
        {
            BaseFlipbookComponent->SetMaterial(0, DynamicMaterial);
        }
    }
}

void AMonsterActor::ApplyScale()
{
    if (!BaseFlipbookComponent)
    {
        return;
    }

    // Calculate final scale
    float FinalScale = UAppearanceAssembler::CalculateMonsterScale(
        MonsterAttributes.EcologyAttributes.SizeClass,
        CurrentSkeletonData.BaseScale
    );

    // Apply scale to base flipbook component
    BaseFlipbookComponent->SetRelativeScale3D(FVector(FinalScale, FinalScale, FinalScale));
}
