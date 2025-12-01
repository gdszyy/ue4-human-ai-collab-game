// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "PCG/MonsterAttributes.h"
#include "AppearanceLibrary.generated.h"

/**
 * @struct FSkeletonData
 * @brief Defines a base skeleton for monsters.
 * 
 * Base skeletons determine the monster's basic animations (walk, attack, idle, death).
 * They are selected based on ecology attributes (habitat and size class).
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Create a skeleton data entry in a Data Table
 * Skeleton ID: "Forest_Medium_Quadruped"
 * Habitat: Forest
 * Size Class: Medium
 * Idle Flipbook: FB_Quadruped_Idle
 * Walk Flipbook: FB_Quadruped_Walk
 * Attack Flipbook: FB_Quadruped_Attack
 * Death Flipbook: FB_Quadruped_Death
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FSkeletonData : public FTableRowBase
{
    GENERATED_BODY()

    /** Unique skeleton identifier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
    FName SkeletonID;

    /** Habitat type this skeleton is designed for */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
    EHabitatType Habitat;

    /** Size class this skeleton is designed for */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
    ESizeClass SizeClass;

    /** Idle animation flipbook */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton | Animations")
    UPaperFlipbook* IdleFlipbook;

    /** Walk animation flipbook */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton | Animations")
    UPaperFlipbook* WalkFlipbook;

    /** Attack animation flipbook */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton | Animations")
    UPaperFlipbook* AttackFlipbook;

    /** Death animation flipbook */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton | Animations")
    UPaperFlipbook* DeathFlipbook;

    /** Base scale multiplier for this skeleton */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
    float BaseScale = 1.0f;

    /** Default constructor */
    FSkeletonData()
        : SkeletonID(NAME_None)
        , Habitat(EHabitatType::Forest)
        , SizeClass(ESizeClass::Medium)
        , IdleFlipbook(nullptr)
        , WalkFlipbook(nullptr)
        , AttackFlipbook(nullptr)
        , DeathFlipbook(nullptr)
        , BaseScale(1.0f)
    {
    }
};

/**
 * @struct FPartData
 * @brief Defines a visual part that can be attached to a monster skeleton.
 * 
 * Parts are selected based on combat traits and attached to specific anchor points
 * on the base skeleton.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Create a part data entry in a Data Table
 * Part ID: "FireWings"
 * Part Name: "火焰之翼"
 * Part Sprite: S_FireWings
 * Anchor Point: "Back"
 * Relative Location: (0, 0, 20)
 * Relative Scale: (1, 1, 1)
 * Z Order: 1
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FPartData : public FTableRowBase
{
    GENERATED_BODY()

    /** Unique part identifier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FName PartID;

    /** Display name of the part */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FText PartName;

    /** Sprite for this part */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    UPaperSprite* PartSprite;

    /** Anchor point name on the skeleton (e.g., "Head", "Back", "Tail") */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FName AnchorPoint;

    /** Relative location offset from anchor point */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FVector RelativeLocation = FVector::ZeroVector;

    /** Relative rotation offset from anchor point */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FRotator RelativeRotation = FRotator::ZeroRotator;

    /** Relative scale multiplier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FVector RelativeScale = FVector::OneVector;

    /** Z-order for sprite layering (higher values render on top) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    int32 ZOrder = 0;

    /** Default constructor */
    FPartData()
        : PartID(NAME_None)
        , PartName(FText::GetEmpty())
        , PartSprite(nullptr)
        , AnchorPoint(NAME_None)
        , RelativeLocation(FVector::ZeroVector)
        , RelativeRotation(FRotator::ZeroRotator)
        , RelativeScale(FVector::OneVector)
        , ZOrder(0)
    {
    }
};

/**
 * @struct FPaletteData
 * @brief Defines a color palette for a specific habitat.
 * 
 * Palettes are used for runtime color replacement (palette swapping) to make
 * monsters visually match their habitat.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Create a palette data entry in a Data Table
 * Habitat: Forest
 * Primary Color: (0.2, 0.8, 0.2, 1.0) - Green
 * Secondary Color: (0.4, 0.6, 0.3, 1.0) - Dark Green
 * Accent Color: (0.8, 0.7, 0.3, 1.0) - Yellow
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FPaletteData : public FTableRowBase
{
    GENERATED_BODY()

    /** Habitat type this palette is designed for */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    EHabitatType Habitat;

    /** Primary color (body) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor PrimaryColor = FLinearColor::White;

    /** Secondary color (details) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor SecondaryColor = FLinearColor::Gray;

    /** Accent color (highlights) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor AccentColor = FLinearColor::White;

    /** Default constructor */
    FPaletteData()
        : Habitat(EHabitatType::Forest)
        , PrimaryColor(FLinearColor::White)
        , SecondaryColor(FLinearColor::Gray)
        , AccentColor(FLinearColor::White)
    {
    }
};

/**
 * @struct FAnchorPoint
 * @brief Defines an anchor point on a skeleton for attaching parts.
 * 
 * Anchor points define where parts can be attached to the base skeleton.
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Define anchor points for a skeleton
 * Anchor Name: "Head"
 * Relative Location: (0, 0, 50)
 * 
 * Anchor Name: "Back"
 * Relative Location: (0, 0, 30)
 * ```
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FAnchorPoint
{
    GENERATED_BODY()

    /** Anchor point name */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anchor")
    FName AnchorName;

    /** Relative location on the skeleton */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anchor")
    FVector RelativeLocation = FVector::ZeroVector;

    /** Default constructor */
    FAnchorPoint()
        : AnchorName(NAME_None)
        , RelativeLocation(FVector::ZeroVector)
    {
    }
};
