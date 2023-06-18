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
enum class EVehicleConstructPartType : uint8
{
    Body,
    Chassis
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

UENUM()
enum class EValkyrieGameState : uint8
{
    WaitingToStart = 0,
    InProgress,
    Pause,
    GameSettings,
    GameOver
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
struct FVehicleConstructPart : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    EVehicleConstructPartType PartType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FString PartName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UTexture2D* PartThumb;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UStaticMesh* PartMesh;
};

USTRUCT(BlueprintType)
struct FVehicleItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName ItemName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UTexture2D* ItemThumb;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AActor> ItemClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    EVehicleItemType ItemType;

    UPROPERTY(EditDefaultsONly, BlueprintReadWrite)
    bool bIsLocked{false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 MaxAmmoCapacity{10};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float ReloadingTime{2.f};
};

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemValueChangedSignature, EItemPropertyType, float, float);

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

    FInteractionData(EItemPropertyType InType, int32 InAmount, UTexture2D* InItemTumb = nullptr) : Type(InType), Amount(InAmount), ItemTumb(InItemTumb) {}
    FInteractionData()
    {
        Type = EItemPropertyType::NoType;
        Amount = 0;
        ItemTumb = nullptr;
    }
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWorkshopTasksUpdatedSignature, const TArray<FInteractionData>&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateCostSignature, EItemPropertyType, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCoinsChangedSignature, int32);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnFoundSessionDataSignature, const FString&, int32, int32);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EValkyrieGameState);

USTRUCT(BlueprintType)
struct FWheelData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USceneComponent* WheelRoot;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float LastContactDistance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FVector WheelVelocity;

    FWheelData() : WheelRoot(nullptr), LastContactDistance(0.f), WheelVelocity(FVector::ZeroVector) {}
    FWheelData(USceneComponent* InWheelRoot, float InLastContactDistance, const FVector& InWheelVelocity)  //
        : WheelRoot(InWheelRoot),                                                                          //
          LastContactDistance(InLastContactDistance),                                                      //
          WheelVelocity(InWheelVelocity)
    {
    }
};

USTRUCT(BlueprintType)
struct FWheelsGroup
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<FWheelData> WheelsArray;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float SuspensionDumping;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float SuspensionStiffness;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UPrimitiveComponent* RootSimulatedComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName RootSimulatedBoneName;

    FWheelsGroup() : WheelsArray({}), SuspensionDumping(0.f), SuspensionStiffness(0.f), RootSimulatedComponent(nullptr), RootSimulatedBoneName(NAME_None) {}
    FWheelsGroup(const TArray<FWheelData>& InWheelsArray, float InSuspensionDumping, float InSuspensionStiffness, UPrimitiveComponent* InRootSimulatedComponent, FName BoneName = NAME_None)  //
        : WheelsArray(InWheelsArray),                                                                                                                                                         //
          SuspensionDumping(InSuspensionDumping),                                                                                                                                             //
          SuspensionStiffness(InSuspensionStiffness),                                                                                                                                         //
          RootSimulatedComponent(InRootSimulatedComponent),                                                                                                                                   //
          RootSimulatedBoneName(BoneName)
    {
    }
};