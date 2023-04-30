// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Player/DefaultWeeledVehicle.h"
#include "../ValkiriaStrikeWheelFront.h"
#include "../ValkiriaStrikeWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"

#include "Engine.h"

ADefaultWeeledVehicle::ADefaultWeeledVehicle()
{
    bReplicates = true;
    // Simulation
    UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

    check(Vehicle4W->WheelSetups.Num() == 4);

    Vehicle4W->WheelSetups[0].WheelClass = UValkiriaStrikeWheelFront::StaticClass();
    Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
    Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

    Vehicle4W->WheelSetups[1].WheelClass = UValkiriaStrikeWheelFront::StaticClass();
    Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
    Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

    Vehicle4W->WheelSetups[2].WheelClass = UValkiriaStrikeWheelRear::StaticClass();
    Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
    Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

    Vehicle4W->WheelSetups[3].WheelClass = UValkiriaStrikeWheelRear::StaticClass();
    Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
    Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComp->TargetOffset = FVector(0.f, 0.f, 1500.f);
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 1500.0f;
    SpringArmComp->SetUsingAbsoluteRotation(true);
    SpringArmComp->bInheritPitch = false;
    SpringArmComp->bInheritRoll = false;

    CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;
    CameraComp->FieldOfView = 90.f;

    bInReverseGear = false;

    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    VehicleIndicatorsComp = CreateDefaultSubobject<UVehicleIndicatorsComponent>("VehicleIndicatorsComp");
}

bool ADefaultWeeledVehicle::AddAmount(const FInteractionData& Data)
{
    // GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::FromInt(Amount));

    bool bResult{false};
    switch (Data.Type)
    {
        case EItemPropertyType::Ammo: bResult = WeaponComponent->AddAmmo(Data.Amount); break;
        case EItemPropertyType::Money:
            VehicleIndicatorsComp->AddCoins(Data.Amount);
            bResult = true;
            break;
        case EItemPropertyType::Fuel: bResult = VehicleIndicatorsComp->AddFuel(Data.Amount); break;
    }

    return bResult;
}

bool ADefaultWeeledVehicle::MakeMaintenance(EItemPropertyType Type)
{
    const int32 EnabledCoins = VehicleIndicatorsComp->GetCoins();
    if (!EnabledCoins) return false;

    const auto ItemPrice = WorkshopTasks.FindByPredicate([&](const FInteractionData& Data) { return Data.Type == Type; })->Amount;

    switch (Type)
    {
        case EItemPropertyType::Ammo:                         //
            if (EnabledCoins < ItemPrice) return false;       //
            WeaponComponent->AddAmmo(1);                      //
            VehicleIndicatorsComp->AddCoins(-ItemPrice);      //
            break;                                            //
        case EItemPropertyType::Endurance:                    //
            VehicleIndicatorsComp->RepairVehicle(ItemPrice);  //
            break;                                            //
        case EItemPropertyType::Armor:                        //
            VehicleIndicatorsComp->RepairArmor(ItemPrice);    //
            break;                                            //
        case EItemPropertyType::Fuel:                         //
            VehicleIndicatorsComp->Refuel(ItemPrice);         //
            break;                                            //
        default: break;
    }

    return true;
}

bool ADefaultWeeledVehicle::SetWorkshopTasksData(const TArray<FInteractionData>& Tasks)
{
    WorkshopTasks = Tasks;

    TArray<FInteractionData> WorkshopCost;
    for (auto& Task : Tasks)
    {
        if (Task.Type == EItemPropertyType::NoType) continue;

        UE_LOG(LogTemp, Display, TEXT("Task: %s"), *UEnum::GetValueAsString(Task.Type));

        if (Task.Type == EItemPropertyType::Ammo)
        {
            WorkshopCost.Add(Task);
        }
        else
        {
            WorkshopCost.Add(FInteractionData(Task.Type, Task.Amount * VehicleIndicatorsComp->GetNeededValue(Task.Type), Task.ItemTumb));
        }
    }
    OnWorkshopTasksUpdated.Broadcast(WorkshopCost);
    return false;
}

void ADefaultWeeledVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(ADefaultWeeledVehicle, Pricies);
}

void ADefaultWeeledVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // set up gameplay key bindings
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ADefaultWeeledVehicle::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ADefaultWeeledVehicle::MoveRight);

    DECLARE_DELEGATE_OneParam(FOnPressingBreak, bool);
    PlayerInputComponent->BindAction<FOnPressingBreak>("Handbrake", IE_Pressed, this, &ADefaultWeeledVehicle::OnHandbrakePressed, true);
    PlayerInputComponent->BindAction<FOnPressingBreak>("Handbrake", IE_Released, this, &ADefaultWeeledVehicle::OnHandbrakePressed, false);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UWeaponComponent::ShootFromSecondWeapon_OnServer);
}

void ADefaultWeeledVehicle::Tick(float Delta)
{
    Super::Tick(Delta);

    bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

    // if (bIsAutoMoveForward)
    //{
    //     GetVehicleMovementComponent()->SetThrottleInput(0.4f);
    // }
}

void ADefaultWeeledVehicle::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponComponent);
    check(VehicleIndicatorsComp);

    Tags.Add("Player");
}

void ADefaultWeeledVehicle::MoveForward(float Val)
{
    bIsAutoMoveForward = FMath::IsNearlyZero(Val) ? true : false;
    GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ADefaultWeeledVehicle::MoveRight(float Val)
{
    GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ADefaultWeeledVehicle::OnHandbrakePressed(bool bIsUsed)
{
    GetVehicleMovementComponent()->SetHandbrakeInput(bIsUsed);
}

void ADefaultWeeledVehicle::OnFireEvent(bool bIsEnabled) {}
