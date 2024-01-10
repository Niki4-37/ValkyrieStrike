// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Player/ModularVehicleBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "Components/WorkshopComponent.h"
#include "Components/InputComponent.h"
#include "Components/WheelManagerComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"
#include "Decorations/DecorationActor.h"
#include "NiagaraComponent.h"

#include "Engine.h"
#include "DrawDebugHelpers.h"

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
    EngineSound = CreateDefaultSubobject<UAudioComponent>("EngineSound");

    RareLeftWheelDust = CreateDefaultSubobject<UNiagaraComponent>("RareLeftWheelDust");
    RareLeftWheelDust->SetupAttachment(WheelRL);

    RareRightWheelDust = CreateDefaultSubobject<UNiagaraComponent>("RareRightWheelDust");
    RareRightWheelDust->SetupAttachment(WheelRR);
}

bool AModularVehicleBase::AddAmount(const FInteractionData& Data)
{
    bool bResult{false};
    switch (Data.Type)
    {
        case EItemPropertyType::Ammo: bResult = WeaponComponent->AddAmmo(Data.Amount); break;
        case EItemPropertyType::Money:
            WorkshopComponent->AddCoins(Data.Amount);
            bResult = true;
            break;
        case EItemPropertyType::Endurance:
            VehicleIndicatorsComp->AddHealth(Data.Amount);
            bResult = true;
            break;
        case EItemPropertyType::Fuel: bResult = VehicleIndicatorsComp->AddFuel(Data.Amount); break;
        default: bResult = true; break;
    }

    return bResult;
}

bool AModularVehicleBase::MakeMaintenance(EItemPropertyType Type)
{
    WorkshopComponent->MakeMaintenance(Type);
    return true;
}

bool AModularVehicleBase::SetWorkshopTasksData(const TArray<FInteractionData>& Tasks)
{
    WorkshopComponent->SetWorkshopTasks(Tasks);
    return false;
}

bool AModularVehicleBase::ShootFromWeapon()
{
    WeaponComponent->ShootFromSecondWeapon_OnServer();
    return false;
}

bool AModularVehicleBase::UseReverseGear(bool bIsUsing)
{
    bIsReverse = bIsUsing;
    MoveForvardAxis = -1.f;
    return false;
}

void AModularVehicleBase::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponComponent);
    check(VehicleIndicatorsComp);
    check(EngineSound);
    check(RareLeftWheelDust);
    check(RareRightWheelDust);

    Tags.Add("Player");

    if (HasAuthority())
    {
        WheelManagerComponent->StartSendData(SendDataRate);

        GetWorld()->GetTimerManager().SetTimer(DataTickTimer, this, &AModularVehicleBase::SendDataTick_Multicast, SendDataRate, true);
    }

    EngineSound->Play();

    VehicleIndicatorsComp->OnDeath.AddUObject(this, &AModularVehicleBase::OnDeath);
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
    const float SuspensionDumping = 2000.f;
    const float SuspensionStiffness = 2200.f;
    WheelsGroup.Add(FWheelsGroup(WheelsArray, SuspensionDumping, SuspensionStiffness, Chassis));
    WheelManagerComponent->SetWheelsGroups(WheelsGroup);
}

void AModularVehicleBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    PawnDeltaTime = DeltaTime;

    UpdateWheelsTurn();

    if (EngineSound)
    {
        EngineSound->SetFloatParameter(EngineRPMName, FMath::Abs(MoveForvardAxis));
    }
}

void AModularVehicleBase::Restart()
{
    Super::Restart();

    if (const auto ValkyriePlayerState = GetPlayerState<AValkyriePlayerState>())
    {
        // GetNetMode() != NM_Client ? GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("SERVER, EnablePlayerState")) :
        //                             GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("CLIENT, EnablePlayerState"));

        const auto UnitPtr = ValkyriePlayerState->GetVehicleUnits().FindByPredicate([](const FVehicleUnitData& Data) { return Data.UnitType == EVehicleUnitType::Body; });

        if (UnitPtr && UnitPtr->UnitComponents[0].UnitComponentMesh)
        {
            VehicleBody->SetStaticMesh(UnitPtr->UnitComponents[0].UnitComponentMesh);
        }

        WorkshopComponent->AddCoins(ValkyriePlayerState->GetCoins());
    }

    if (HasAuthority())
    {
        WeaponComponent->SetCompToAttachWeapons(VehicleBody);

        WeaponComponent->InitWeapons();
    }

    Camera->PostProcessSettings.ColorSaturation = FVector4(1.f, 1.f, 1.f, 1.f);
}

void AModularVehicleBase::UnPossessed()
{
    if (const auto ValkyriePS = GetPlayerState<AValkyriePlayerState>())
    {
        ValkyriePS->SaveCoins(WorkshopComponent->GetCoins());
    }

    WeaponComponent->CleanWeapons();

    Super::UnPossessed();
    Tags.Empty();
}

void AModularVehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AModularVehicleBase::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AModularVehicleBase::MoveRight);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UWeaponComponent::ShootFromSecondWeapon_OnServer);
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
    if (!bIsReverse)
    {
        MoveForvardAxis = FMath::Abs(Amount);
    }

    SmoothTurnHandle(Amount);

    if (WheelManagerComponent->IsWheelContact())
    {
        FVector VelocityAverage;
        GetVelocityAverage(VelocityAverage);

        RareLeftWheelDust->SetNiagaraVariableFloat(DustSpawnRateParamName, VelocityAverage.Size() / MinVelocityToDust);
        RareRightWheelDust->SetNiagaraVariableFloat(DustSpawnRateParamName, VelocityAverage.Size() / MinVelocityToDust);
    }
}

void AModularVehicleBase::MoveRight(float Value)
{
    MoveSideAxis = Value;
    if (FMath::Abs(MoveForvardAxis) < FMath::Abs(Value) && !bIsReverse)
    {
        MoveForvardAxis = FMath::Abs(Value);
    }
}

void AModularVehicleBase::SmoothTurnHandle(float YAxisValue)
{
    if (FMath::IsNearlyZero(MoveSideAxis) && FMath::IsNearlyZero(YAxisValue)) return;
    const auto DirectionAngle = FMath::Atan2(MoveSideAxis, YAxisValue);
    const auto DirectionAngleDeg = FMath::RadiansToDegrees(DirectionAngle);
    const auto WehicleAngleDeg = GetActorForwardVector().Rotation().Yaw * (bIsReverse ? -1.f : 1.f);
    const auto AngleBetween = DirectionAngleDeg - WehicleAngleDeg;

    auto Modifier = 0.f;
    if (AngleBetween > 180.f)
    {
        Modifier = 360.f;
    }
    else if (AngleBetween < -180.f)
    {
        Modifier = -360.f;
    }
    const auto MoveSideValue = (AngleBetween - Modifier) / 180.f;
    SmoothTurnValue = FMath::FInterpTo(SmoothTurnValue, MoveSideValue, PawnDeltaTime, 8.f);  // 6.f
}

void AModularVehicleBase::UpdateWheelsTurn()
{
    const FRotator NewRotationFrontWheels{FRotator(0.f, SmoothTurnValue * 30.f, 0.f)};  // 20.f, 0.f
    const FRotator NewRotationRareWheels{FRotator(0.f, SmoothTurnValue * -30.f, 0.f)};  //-20.f, 0.f
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
        WheelManagerComponent->SetControlInput(MoveForvardAxis);
    }
    else
    {
        if (!IsLocallyControlled()) return;

        SendMoveControls_Server(MoveForvardAxis, MoveSideAxis, SmoothTurnValue);
    }
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

void AModularVehicleBase::OnDeath()
{
    OnDeathCameraEffect_OnClient();

    GetWorld()->GetTimerManager().ClearTimer(DataTickTimer);
    MoveForvardAxis = 0.f;
    MoveSideAxis = 0.f;
    SendDataTick_Multicast();
}

void AModularVehicleBase::OnDeathCameraEffect_OnClient_Implementation()
{
    Camera->PostProcessSettings.bOverride_ColorSaturation = 1;
    Camera->PostProcessSettings.ColorSaturation = FVector4();
}