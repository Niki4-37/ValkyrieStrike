// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Player/ModularVehicleBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "Components/WorkshopComponent.h"
#include "Components/InputComponent.h"
#include "Components/WheelManagerComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AModularVehicleBase::AModularVehicleBase()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    PawnRootComponent = CreateDefaultSubobject<USceneComponent>("PawnRootComponent");
    SetRootComponent(PawnRootComponent);

    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>("VehicleBody");
    VehicleBody->SetupAttachment(RootComponent);
    VehicleBody->SetCanEverAffectNavigation(false);
    VehicleBody->SetMobility(EComponentMobility::Movable);
    // VehicleBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // VehicleBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    Chassis = CreateDefaultSubobject<USkeletalMeshComponent>("Chassis");
    Chassis->BodyInstance.bSimulatePhysics = true;
    Chassis->SetupAttachment(VehicleBody);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->TargetOffset = FVector(0.f, 0.f, 1500.f);
    SpringArm->SetupAttachment(VehicleBody);
    SpringArm->TargetArmLength = 1500.0f;
    SpringArm->SetUsingAbsoluteRotation(true);
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritRoll = false;

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;
    Camera->FieldOfView = 90.f;

    // HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
    // HitBox->SetBoxExtent(FVector(200.f));
    // HitBox->SetupAttachment(RootComponent);

    WheelFR = CreateDefaultSubobject<USceneComponent>("WheelFR");
    WheelFR->SetupAttachment(Chassis);
    WheelFL = CreateDefaultSubobject<USceneComponent>("WheelFL");
    WheelFL->SetupAttachment(Chassis);
    WheelRR = CreateDefaultSubobject<USceneComponent>("WheelRR");
    WheelRR->SetupAttachment(Chassis);
    WheelRL = CreateDefaultSubobject<USceneComponent>("WheelRL");
    WheelRL->SetupAttachment(Chassis);

    WheelManagerComponent = CreateDefaultSubobject<UWheelManagerComponent>("WheelManagerComponent");
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    VehicleIndicatorsComp = CreateDefaultSubobject<UVehicleIndicatorsComponent>("VehicleIndicatorsComp");
    WorkshopComponent = CreateDefaultSubobject<UWorkshopComponent>("WorkshopComponent");
}

void AModularVehicleBase::BeginPlay()
{
    Super::BeginPlay();
}

void AModularVehicleBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    check(WheelManagerComponent);
    TArray<FWheelData> WheelsArray;
    WheelsArray.Reserve(4);

    WheelsArray.Add(FWheelData(WheelFR, WheelManagerComponent->GetWheelDefultDistance(), FVector::ZeroVector));
    WheelsArray.Add(FWheelData(WheelFL, WheelManagerComponent->GetWheelDefultDistance(), FVector::ZeroVector));
    WheelsArray.Add(FWheelData(WheelRR, WheelManagerComponent->GetWheelDefultDistance(), FVector::ZeroVector));
    WheelsArray.Add(FWheelData(WheelRL, WheelManagerComponent->GetWheelDefultDistance(), FVector::ZeroVector));

    TArray<FWheelsGroup> WheelsGroup;
    WheelsGroup.Add(FWheelsGroup(WheelsArray, 2000.f, 2200.f, Chassis));
    WheelManagerComponent->SetWheelsGroups(WheelsGroup);
}

void AModularVehicleBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    PawnDeltaTime = DeltaTime;
}

void AModularVehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AModularVehicleBase::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AModularVehicleBase::MoveRight);
}

bool AModularVehicleBase::GetVelocityAverage(FVector& VelocityAverage) const
{
    return WheelManagerComponent->GetVelocityAverage(VelocityAverage);
}

bool AModularVehicleBase::GetWheelsGroups(TArray<FWheelsGroup>& WheelsGroups) const
{
    return WheelManagerComponent->GetWheelsGroups(WheelsGroups);
}

void AModularVehicleBase::MoveForward(float Amount)
{
    WheelManagerComponent->SetControlInput(Amount);
}

void AModularVehicleBase::MoveRight(float Value)
{
    SmoothTurnValue = FMath::FInterpTo(SmoothTurnValue, Value, PawnDeltaTime, 6.f);

    const FRotator NewRotationFrontWheels{FRotator(0.f, SmoothTurnValue * 20.f, 0.f)};
    const FRotator NewRotationRareWheels{FRotator(0.f, SmoothTurnValue * - 20.f, 0.f)};
    WheelFR->SetRelativeRotation(NewRotationFrontWheels);
    WheelFL->SetRelativeRotation(NewRotationFrontWheels);
    WheelRR->SetRelativeRotation(NewRotationRareWheels);
    WheelRL->SetRelativeRotation(NewRotationRareWheels);
}
