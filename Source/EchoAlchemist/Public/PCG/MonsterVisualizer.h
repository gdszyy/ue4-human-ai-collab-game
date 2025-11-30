// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "PCG/MonsterAttributes.h"
#include "MonsterVisualizer.generated.h"

/**
 * @class AMonsterVisualizer
 * @brief A temporary visual module for previewing procedurally generated monsters.
 * 
 * This Actor visualizes a monster based on its attributes using simple colored sprites.
 * It's designed for testing and preview purposes in the UE4 editor.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // In the editor:
 * 1. Place a Monster Visualizer Actor in the level
 * 2. Set Monster Attributes in the Details panel
 * 3. Click "Regenerate Visual" to preview the monster
 * 
 * // In Blueprint:
 * Spawn Actor(Monster Visualizer, Transform)
 * Set Monster Attributes(Monster Attributes)
 * Regenerate Visual()
 * ```
 */
UCLASS()
class ECHOALCHEMIST_API AMonsterVisualizer : public AActor
{
    GENERATED_BODY()

public:
    /** Constructor */
    AMonsterVisualizer();

    /** Called when the game starts or when spawned */
    virtual void BeginPlay() override;

#if WITH_EDITOR
    /** Called when a property is changed in the editor */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    /**
     * Sets the monster attributes and regenerates the visual.
     * 
     * @param Attributes The monster attributes to visualize.
     * 
     * **Blueprint Usage:**
     * ```
     * // Set attributes and regenerate visual
     * Set Monster Attributes(Monster Attributes)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster Visualizer")
    void SetMonsterAttributes(const FMonsterAttributes& Attributes);

    /**
     * Regenerates the visual representation based on current attributes.
     * 
     * **Blueprint Usage:**
     * ```
     * // Regenerate visual after changing attributes
     * Regenerate Visual()
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster Visualizer")
    void RegenerateVisual();

    /**
     * Clears all visual components.
     * 
     * **Blueprint Usage:**
     * ```
     * // Clear the current visual
     * Clear Visual()
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "Monster Visualizer")
    void ClearVisual();

protected:
    /** The monster attributes to visualize */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Visualizer")
    FMonsterAttributes MonsterAttributes;

    /** Root scene component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster Visualizer")
    USceneComponent* RootSceneComponent;

    /** Base body sprite component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster Visualizer")
    UPaperSpriteComponent* BodySprite;

    /** Additional part sprite components (for combat traits) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster Visualizer")
    TArray<UPaperSpriteComponent*> PartSprites;

    /** Whether to auto-regenerate visual when attributes change */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Visualizer")
    bool bAutoRegenerateOnChange = true;

private:
    /** Creates the base body sprite based on ecology attributes */
    void CreateBodySprite();

    /** Creates additional part sprites based on combat traits */
    void CreatePartSprites();

    /** Gets the color for a habitat type */
    FLinearColor GetHabitatColor(EHabitatType Habitat) const;

    /** Gets the scale multiplier for a size class */
    float GetSizeScale(ESizeClass SizeClass) const;

    /** Gets the color for a combat trait rarity */
    FLinearColor GetRarityColor(ERarity Rarity) const;
};
