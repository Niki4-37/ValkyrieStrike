#pragma once

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameCoreTypes.generated.h"

class UTexture2D;

// clang-format off
UENUM(BlueprintType)
enum class EMenuState : uint8
{
    MainMenu        UMETA(DispayName = "Show Menu"),
    GameSettings    UMETA(DispayName = "Show Setings"),
    GameConfig      UMETA(DispayName = "Show Game Config")
};

UENUM(BlueprintType)
enum class EVehicleItemType : uint8
{
    Turret,
    SecondWeapon,
    Armor
};

UENUM(BlueprintType)
enum class EItemPropertyType : uint8
{
    Ammo,
    Endurance,
    Armor,
    Money,
    Fuel,
    NoType,

    Begin = Ammo    UMETA(Hidden),
    End = NoType    UMETA(Hidden)
};
// clang-format on

static EItemPropertyType& operator++(EItemPropertyType& EType)
{
    EType = EItemPropertyType(static_cast<std::underlying_type<EItemPropertyType>::type>(EType) + 1);
    return EType;
}

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
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFuelValueChangedSignature, float);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemMountSignature, const FVehicleItemData&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeAmmoSignature, EVehicleItemType, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOnStartReloadingSignature, EVehicleItemType);

USTRUCT(BlueprintType)
struct FInteractionData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemPropertyType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Amount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UTexture2D* ItemTumb;

    FInteractionData(EItemPropertyType InType, int32 InAmount, UTexture2D* InItemTumb) : Type(InType), Amount(InAmount), ItemTumb(InItemTumb) {}
    FInteractionData()
    {
        Type = EItemPropertyType::NoType;
        Amount = 0;
        ItemTumb = nullptr;
    }
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWorkshopTasksUpdatedSignature, const TArray<FInteractionData>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCoinsChangedSignature, int32);

USTRUCT(BlueprintType)
struct FWorkshopPriceData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EndurancePerUnit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ArmorPerUnit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoPerUnit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FuelPerUnit;
};