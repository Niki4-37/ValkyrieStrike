#pragma once

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameCoreTypes.generated.h"

class UTexture2D;

//clang-format off
UENUM(BlueprintType)
enum class EMenuState : uint8
{
    MainMenu UMETA(DispayName = "Show Menu"),
    GameSettings UMETA(DispayName = "Show Setings"),
    GameConfig UMETA(DispayName = "Show Game Config")
};

UENUM(BlueprintType)
enum class EVehicleItemType : uint8
{
    Turret,
    SecondWeapon,
    Armor
};
//clang-format on

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuStateChangedSignature, EMenuState);

USTRUCT(BlueprintType)
struct FDataUnit
{
    GENERATED_USTRUCT_BODY()

    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    // UTexture2D* Tumb;
};

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelDisplayName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelThumb;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

USTRUCT(BlueprintType)
struct FVehicleItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName ItemName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UTexture2D* ItemTumb;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AActor> ItemClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    EVehicleItemType ItemType;

    UPROPERTY(EditDefaultsONly, BlueprintReadWrite)
    bool bIsLocked{false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 MaxAmmoCapacity{10};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReloadingTime{2.f};
};

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFuelValueChangedSignature, float);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemMountSignature, const FVehicleItemData&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponMakeShotSignature, EVehicleItemType, int32);
