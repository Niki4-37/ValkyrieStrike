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
#include "GameLevelsConfig/ValkyriePlayerState.h"

#include "Engine.h"

AModularVehicleBase::AModularVehicleBase()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    Chassis = CreateDefaultSubobject<USkeletalMeshComponent>("Chassis");
    Chassis->BodyInstance.bSimulatePhysics = true;
    SetRootComponent(Chassis);

    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>("VehicleBody");
    VehicleBody->SetupAttachment(Chassis);
    VehicleBody->SetRelativeScale3D(FVector(1.f));
    VehicleBody->SetCanEverAffectNavigation(false);
    VehicleBody->SetMobility(EComponentMobility::Movable);
    // VehicleBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // VehicleBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    VehicleBody->SetIsReplicated(true);

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

    if (HasAuthority())
    {
        // Set ClearTimer!!!
        GetWorld()->GetTimerManager().SetTimer(DataTickTimer, this, &AModularVehicleBase::SendDataTick_Multicast, 0.03f, true);
    }
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

    WheelsTurn();
}

void AModularVehicleBase::Restart()
{
    Super::Restart();

    if (const auto ValkyriePlayerState = GetPlayerState<AValkyriePlayerState>())
    {
        GetNetMode() != NM_Client ? GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("SERVER, EnablePlayerState")) :
                                    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("CLIENT, EnablePlayerState"));

        const auto UnitPtr = ValkyriePlayerState->GetVehicleUnits().FindByPredicate([](const FVehicleUnitData& Data) { return Data.UnitType == EVehicleUnitType::Body; });

        if (UnitPtr && UnitPtr->UnitComponents[0].UnitComponentMesh)
        {
            SetStaticMesh_OnServer(UnitPtr->UnitComponents[0].UnitComponentMesh);
        }
    }

    if (HasAuthority())
    {
        WeaponComponent->SetCompToAttachWeapons(VehicleBody);

        WeaponComponent->InitWeapons_OnServer();
    }

    // if (const auto ValkyriePS = GetPlayerState<AValkyriePlayerState>())
    //{
    //     WorkshopComponent->AddCoins(ValkyriePS->GetCoins());
    // }
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
    MoveForvardAxis = Amount;
}

void AModularVehicleBase::MoveRight(float Value)
{
    MoveSideAxis = Value;
    SmoothTurnValue = FMath::FInterpTo(SmoothTurnValue, MoveSideAxis, PawnDeltaTime, 6.f);
}

void AModularVehicleBase::WheelsTurn()
{
    const FRotator NewRotationFrontWheels{FRotator(0.f, SmoothTurnValue * 20.f, 0.f)};
    const FRotator NewRotationRareWheels{FRotator(0.f, SmoothTurnValue * -20.f, 0.f)};
    WheelFR->SetRelativeRotation(NewRotationFrontWheels);
    WheelFL->SetRelativeRotation(NewRotationFrontWheels);
    WheelRR->SetRelativeRotation(NewRotationRareWheels);
    WheelRL->SetRelativeRotation(NewRotationRareWheels);
}

void AModularVehicleBase::SendDataTick_Multicast_Implementation()
{
    if (HasAuthority())
    {
        SendMoveControls_Multicast(MoveForvardAxis, MoveSideAxis, SmoothTurnValue);
    }
    else
    {
        if (!IsLocallyControlled()) return;

        SendMoveControls_Server(MoveForvardAxis, MoveSideAxis, SmoothTurnValue);
    }

    WheelManagerComponent->SetControlInput(MoveForvardAxis);
}

void AModularVehicleBase::SendMoveControls_Server_Implementation(float InMoveForvardAxis, float InMoveSideAxis, float InSmoothTurnValue)
{

    if (IsLocallyControlled()) return;

    MoveForvardAxis = InMoveForvardAxis;
    MoveSideAxis = InMoveSideAxis;
    SmoothTurnValue = InSmoothTurnValue;
}

void AModularVehicleBase::SendMoveControls_Multicast_Implementation(float InMoveForvardAxis, float InMoveSideAxis, float InSmoothTurnValue)
{

    if (IsLocallyControlled()) return;

    MoveForvardAxis = InMoveForvardAxis;
    MoveSideAxis = InMoveSideAxis;
    SmoothTurnValue = InSmoothTurnValue;
}

void AModularVehicleBase::SetStaticMesh_OnServer_Implementation(UStaticMesh* NewMesh)
{
    if (IsLocallyControlled())
    {
        VehicleBody->SetStaticMesh(NewMesh);
    }

    SetStaticMesh_Multcast(NewMesh);
}

void AModularVehicleBase::SetStaticMesh_Multcast_Implementation(UStaticMesh* NewMesh)
{
    if (!IsLocallyControlled())
    {
        VehicleBody->SetStaticMesh(NewMesh);
    }
}