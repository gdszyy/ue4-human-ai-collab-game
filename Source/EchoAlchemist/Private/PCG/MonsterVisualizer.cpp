// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/MonsterVisualizer.h"
#include "Components/SceneComponent.h"
#include "Paper2D/Classes/PaperSprite.h"

AMonsterVisualizer::AMonsterVisualizer()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create root scene component
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    // Create body sprite component
    BodySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BodySprite"));
    BodySprite->SetupAttachment(RootComponent);
}

void AMonsterVisualizer::BeginPlay()
{
    Super::BeginPlay();
    
    // Generate visual on begin play
    RegenerateVisual();
}

#if WITH_EDITOR
void AMonsterVisualizer::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Auto-regenerate visual when attributes change in editor
    if (bAutoRegenerateOnChange && PropertyChangedEvent.Property != nullptr)
    {
        FName PropertyName = PropertyChangedEvent.Property->GetFName();
        if (PropertyName == GET_MEMBER_NAME_CHECKED(AMonsterVisualizer, MonsterAttributes))
        {
            RegenerateVisual();
        }
    }
}
#endif

void AMonsterVisualizer::SetMonsterAttributes(const FMonsterAttributes& Attributes)
{
    MonsterAttributes = Attributes;
    RegenerateVisual();
}

void AMonsterVisualizer::RegenerateVisual()
{
    // Clear existing visual
    ClearVisual();

    // Create new visual based on attributes
    CreateBodySprite();
    CreatePartSprites();
}

void AMonsterVisualizer::ClearVisual()
{
    // Clear body sprite
    if (BodySprite)
    {
        BodySprite->SetSprite(nullptr);
        BodySprite->SetSpriteColor(FLinearColor::White);
    }

    // Destroy all part sprites
    for (UPaperSpriteComponent* PartSprite : PartSprites)
    {
        if (PartSprite)
        {
            PartSprite->DestroyComponent();
        }
    }
    PartSprites.Empty();
}

void AMonsterVisualizer::CreateBodySprite()
{
    if (!BodySprite)
    {
        return;
    }

    // Get habitat color
    FLinearColor HabitatColor = GetHabitatColor(MonsterAttributes.EcologyAttributes.Habitat);
    
    // Get size scale
    float SizeScale = GetSizeScale(MonsterAttributes.EcologyAttributes.SizeClass);

    // Apply color and scale to body sprite
    BodySprite->SetSpriteColor(HabitatColor);
    BodySprite->SetRelativeScale3D(FVector(SizeScale, SizeScale, 1.0f));

    // Note: In a real implementation, you would set an actual sprite asset here
    // For now, we're just using color and scale as a simple visualization
    // BodySprite->SetSprite(LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/MonsterBody")));
}

void AMonsterVisualizer::CreatePartSprites()
{
    // Create a sprite component for each combat trait
    for (int32 i = 0; i < MonsterAttributes.CombatAttributes.Traits.Num(); i++)
    {
        const FCombatTrait& Trait = MonsterAttributes.CombatAttributes.Traits[i];

        // Create a new sprite component for this trait
        FString ComponentName = FString::Printf(TEXT("PartSprite_%d"), i);
        UPaperSpriteComponent* PartSprite = NewObject<UPaperSpriteComponent>(this, FName(*ComponentName));
        
        if (PartSprite)
        {
            PartSprite->RegisterComponent();
            PartSprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

            // Get rarity color
            FLinearColor RarityColor = GetRarityColor(Trait.Rarity);
            PartSprite->SetSpriteColor(RarityColor);

            // Offset the part sprite slightly for visibility
            float OffsetX = (i - MonsterAttributes.CombatAttributes.Traits.Num() / 2.0f) * 20.0f;
            PartSprite->SetRelativeLocation(FVector(OffsetX, 0.0f, 50.0f));
            PartSprite->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));

            // Note: In a real implementation, you would set an actual sprite asset here
            // based on the trait's AssociatedPartIDs
            // PartSprite->SetSprite(LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/Parts/FireWings")));

            PartSprites.Add(PartSprite);
        }
    }
}

FLinearColor AMonsterVisualizer::GetHabitatColor(EHabitatType Habitat) const
{
    switch (Habitat)
    {
    case EHabitatType::Forest:
        return FLinearColor(0.2f, 0.8f, 0.2f, 1.0f); // Green
    case EHabitatType::Desert:
        return FLinearColor(0.9f, 0.7f, 0.3f, 1.0f); // Sand yellow
    case EHabitatType::Tundra:
        return FLinearColor(0.7f, 0.9f, 1.0f, 1.0f); // Ice blue
    case EHabitatType::Cave:
        return FLinearColor(0.4f, 0.4f, 0.5f, 1.0f); // Dark gray
    case EHabitatType::Volcano:
        return FLinearColor(1.0f, 0.3f, 0.1f, 1.0f); // Lava red
    case EHabitatType::Swamp:
        return FLinearColor(0.4f, 0.6f, 0.3f, 1.0f); // Murky green
    default:
        return FLinearColor::White;
    }
}

float AMonsterVisualizer::GetSizeScale(ESizeClass SizeClass) const
{
    switch (SizeClass)
    {
    case ESizeClass::Tiny:
        return 0.5f;
    case ESizeClass::Small:
        return 0.75f;
    case ESizeClass::Medium:
        return 1.0f;
    case ESizeClass::Large:
        return 1.5f;
    case ESizeClass::Giant:
        return 2.5f;
    default:
        return 1.0f;
    }
}

FLinearColor AMonsterVisualizer::GetRarityColor(ERarity Rarity) const
{
    switch (Rarity)
    {
    case ERarity::Common:
        return FLinearColor(0.8f, 0.8f, 0.8f, 1.0f); // Gray
    case ERarity::Rare:
        return FLinearColor(0.3f, 0.5f, 1.0f, 1.0f); // Blue
    case ERarity::Epic:
        return FLinearColor(0.8f, 0.2f, 1.0f, 1.0f); // Purple
    default:
        return FLinearColor::White;
    }
}
