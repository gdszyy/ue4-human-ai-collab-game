// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "PCG/MonsterAttributes.h"
#include "PCG/AppearanceLibrary.h"
#include "MonsterActor.generated.h"

/**
 * @class AMonsterActor
 * @brief A complete monster actor that assembles appearance from attributes.
 * 
 * This actor serves as the base class for blueprint-based monster actors.
 * It handles:
 * - Skeleton selection and flipbook animation
 * - Part attachment based on combat traits
 * - Palette swapping for habitat-based colors
 * - Scale adjustment based on size class
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Spawn a monster actor
 * Spawn Actor(BP_MonsterActor, Location) → Monster Actor
 * 
 * // Set monster attributes
 * Monster Actor → Set Monster Attributes(Monster Attributes)
 * 
 * // Play animation
 * Monster Actor → Play Animation(Walk)
 * ```
 * 
 * **How to Create Blueprint:**
 * 1. Create a Blueprint based on AMonsterActor (e.g., BP_MonsterActor)
 * 2. Set Data Table references in the Blueprint defaults:
 *    - Skeleton Data Table: DT_Skeletons
 *    - Part Data Table: DT_Parts
 *    - Palette Data Table: DT_Palettes
 *    - Palette Swap Material: M_PaletteSwap_Master
 * 3. Use the Blueprint in your game
 */
UCLASS(Blueprintable)
class ECHOALCHEMIST_API AMonsterActor : public AActor
{
    GENERATED_BODY()

public:

    /** Constructor */
    AMonsterActor();

protected:

    /** Called when the game starts or when spawned */
    virtual void BeginPlay() override;

public:

    /**
     * Sets the monster attributes and reconstructs appearance.
     * 
     * @param Attributes The monster attributes to set.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate attributes
     * Generate Monster Attributes(Seed, Climate, Terrain, 2) → Monster Attributes
     * 
     * // Set attributes and reconstruct appearance
     * Monster Actor → Set Monster Attributes(Monster Attributes)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster")
    void SetMonsterAttributes(const FMonsterAttributes& Attributes);

    /**
     * Gets the current monster attributes.
     * 
     * @return The current monster attributes.
     * 
     * **Blueprint Usage:**
     * ```
     * // Get current attributes
     * Monster Attributes = Monster Actor → Get Monster Attributes()
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster")
    FMonsterAttributes GetMonsterAttributes() const { return MonsterAttributes; }

    /**
     * Reconstructs the monster appearance based on current attributes.
     * 
     * **Blueprint Usage:**
     * ```
     * // Reconstruct appearance after modifying attributes
     * Monster Actor.Monster Attributes.Ecology Attributes.Size Class = Large
     * Monster Actor → Reconstruct Appearance()
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster")
    void ReconstructAppearance();

    /**
     * Plays a specific animation.
     * 
     * @param AnimationType The type of animation to play (Idle, Walk, Attack, Death).
     * 
     * **Blueprint Usage:**
     * ```
     * // Play walk animation
     * Monster Actor → Play Animation(Walk)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster")
    void PlayAnimation(EMonsterAnimationType AnimationType);

    /**
     * Clears the current appearance (removes all components).
     * 
     * **Blueprint Usage:**
     * ```
     * // Clear appearance before reconstruction
     * Monster Actor → Clear Appearance()
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster")
    void ClearAppearance();

protected:

    /** Monster attributes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
    FMonsterAttributes MonsterAttributes;

    /** Skeleton data table reference */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster | Data Tables")
    UDataTable* SkeletonDataTable;

    /** Part data table reference */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster | Data Tables")
    UDataTable* PartDataTable;

    /** Palette data table reference */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster | Data Tables")
    UDataTable* PaletteDataTable;

    /** Palette swap master material */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster | Materials")
    UMaterialInterface* PaletteSwapMaterial;

    /** Base flipbook component for skeleton animations */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | Components")
    UPaperFlipbookComponent* BaseFlipbookComponent;

    /** Array of part sprite components */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | Components")
    TArray<UPaperSpriteComponent*> PartComponents;

    /** Current skeleton data */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | Runtime")
    FSkeletonData CurrentSkeletonData;

    /** Current palette data */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | Runtime")
    FPaletteData CurrentPaletteData;

private:

    /** Constructs the base skeleton */
    void ConstructBaseSkeleton();

    /** Attaches parts based on combat traits */
    void AttachParts();

    /** Applies palette swapping */
    void ApplyPaletteSwapping();

    /** Applies scale based on size class */
    void ApplyScale();
};

/**
 * @enum EMonsterAnimationType
 * @brief Types of monster animations.
 */
UENUM(BlueprintType)
enum class EMonsterAnimationType : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Walk UMETA(DisplayName = "Walk"),
    Attack UMETA(DisplayName = "Attack"),
    Death UMETA(DisplayName = "Death")
};
