// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/BaseVehicleWeapon.h"
#include "NiagaraComponent.h"
#include "Player/VehiclePlayerController.h"
#include "Kismet/GameplayStatics.h"

ABaseVehicleWeapon::ABaseVehicleWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    WeaponRootComponent = CreateDefaultSubobject<USceneComponent>("WeaponRootComponent");
    SetRootComponent(WeaponRootComponent);

    Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
    Platform->SetupAttachment(RootComponent);
    Platform->SetMobility(EComponentMobility::Movable);
    Platform->SetIsReplicated(true);
    Platform->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>("Gun");
    Gun->SetMobility(EComponentMobility::Movable);
    Gun->SetIsReplicated(true);
    Gun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GunsightComponent = CreateDefaultSubobject<UNiagaraComponent>("GunsightComponent");
    GunsightComponent->SetupAttachment(Gun, MuzzleSocketName);
}

void ABaseVehicleWeapon::SetStaticMesh(EUnitComponentType Type, UStaticMesh* NewMesh, FName SocketName)
{
    if (!NewMesh) return;
    switch (Type)
    {
        case EUnitComponentType::Platform: Platform->SetStaticMesh(NewMesh); break;
        case EUnitComponentType::Gun:
        {
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            Gun->AttachToComponent(Platform, AttachmentRules, SocketName);
            Gun->SetStaticMesh(NewMesh);
            break;
        }
        case EUnitComponentType::Other:
        {
            auto ExtraSMComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
            if (!ExtraSMComponent) return;
            ExtraSMComponent->SetMobility(EComponentMobility::Movable);
            ExtraSMComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            ExtraSMComponent->SetIsReplicated(true);
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            ExtraSMComponent->RegisterComponent();
            ExtraSMComponent->AttachToComponent(Gun, AttachmentRules, SocketName);
            ExtraSMComponent->SetStaticMesh(NewMesh);
            ExtraSMComponent->SetRelativeScale3D(FVector(1.f));
        }
    }
}

void ABaseVehicleWeapon::DefineSidePosition(float DotProduct)
{
    /** Minus for the left */
    SidePositionModifier = DotProduct > 0.f ? 1.f : -1.f;
}

void ABaseVehicleWeapon::DefineSideMode(float InAcos)
{
    bIsSideMode = !FMath::IsNearlyZero(InAcos);
}

void ABaseVehicleWeapon::SetupWeapon(EVehicleUnitType InWeaponType, int32 InMaxAmmoCapacity, float InReloadingTime)
{
    WeaponType = InWeaponType;
    AmmoCapacity = MaxAmmoCapacity = InMaxAmmoCapacity;
    ReloadingTime = InReloadingTime;
}

void ABaseVehicleWeapon::UpdateAimActor(AActor* NewAimActor, float UpdateTimeRate)
{
    const bool bLocalHasAim = NewAimActor ? true : false;
    if (bHasAim != bLocalHasAim)
    {
        bHasAim = bLocalHasAim;

        CheckMusicTheme(bHasAim);
    }

    GetWorldTimerManager().ClearTimer(RotationTimer);
    auto TimerDelegate = FTimerDelegate::CreateUObject(this, &ABaseVehicleWeapon::RotateToTarget, NewAimActor);
    const float RotationSpeed = UpdateTimeRate / 15.f;
    GetWorldTimerManager().SetTimer(RotationTimer, TimerDelegate, RotationSpeed, true);
}

bool ABaseVehicleWeapon::AddAmmo(int32 Amount, EVehicleUnitType InType)
{
    if (WeaponType != InType || AmmoCapacity == MaxAmmoCapacity) return false;
    IsEmpty() ? ReloadWeapon() : ChangeAmmo(Amount);
    return true;
}

void ABaseVehicleWeapon::AlternativeShot() {}

void ABaseVehicleWeapon::SpawnSound_Multicast_Implementation(USoundBase* NewSound)
{
    UGameplayStatics::SpawnSoundAttached(NewSound, Gun, MuzzleSocketName);
}

void ABaseVehicleWeapon::MakeShot()
{
    /* handled on server */
}

void ABaseVehicleWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseVehicleWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void ABaseVehicleWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseVehicleWeapon::RotateToTarget(AActor* Target)
{
    const auto NoneAimLocation = GetOwner() ? (GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 1000.f) : FVector::ZeroVector;
    const auto AimLocation = Target ? Target->GetActorLocation() : NoneAimLocation;
    const auto PlatformLocatiom = Platform->GetComponentLocation();

    const FRotator Direction = FRotationMatrix::MakeFromX(AimLocation - GetActorLocation()).Rotator();

    const FRotator PlatfromRotation = Platform->GetComponentRotation();

    FRotator Delta = Direction - PlatfromRotation;
    Delta.Normalize();

    const auto DeltaValue = bIsSideMode  //
                                ?
                                FMath::Clamp(Delta.Pitch, -1.f, 1.f)  //
                                :
                                FMath::Clamp(Delta.Yaw, -1.f, 1.f);

    const auto ValueToSet = bIsSideMode  //
                                ?
                                Platform->GetRelativeRotation().Yaw - SidePositionModifier * DeltaValue  //
                                :
                                Platform->GetRelativeRotation().Yaw + DeltaValue;

    /** checking side platform to prevetn turn down */
    auto PlatformValueToSet = (bIsSideMode && SidePositionModifier * ValueToSet > 0.f) ? 0.f : ValueToSet;

    Platform->SetRelativeRotation(FRotator(0.f, PlatformValueToSet, 0.f));
}

void ABaseVehicleWeapon::ChangeAmmo(int32 Value)
{
    AmmoCapacity = FMath::Clamp(AmmoCapacity + Value, 0, MaxAmmoCapacity);
    OnChangeAmmoInWeapon_OnClient(WeaponType, AmmoCapacity);
}

void ABaseVehicleWeapon::Recharge()
{
    if (bIsReloading) return;
    bIsReloading = true;
    GetWorldTimerManager().SetTimer(
        ReloadingTimer,
        [&]()
        {
            bIsReloading = false;
            OnChangeAmmoInWeapon_OnClient(WeaponType, AmmoCapacity);
        },
        ReloadingTime, false);
    OnStartWeaponReloading_OnClient(WeaponType);
}

void ABaseVehicleWeapon::ReloadWeapon()
{
    AmmoCapacity = MaxAmmoCapacity;
    Recharge();
}

void ABaseVehicleWeapon::CheckMusicTheme(bool bFoundAim)
{
    if (!GetOwner()) return;

    if (const auto VehiclePC = Cast<AVehiclePlayerController>(GetOwner()->GetInstigatorController()))
    {
        VehiclePC->CheckMusicTheme_OnClient(bFoundAim);
    }
}

void ABaseVehicleWeapon::OnChangeAmmoInWeapon_OnClient_Implementation(EVehicleUnitType Type, int32 Value)
{
    OnChangeAmmoInWeapon.Broadcast(Type, Value);
}

void ABaseVehicleWeapon::OnStartWeaponReloading_OnClient_Implementation(EVehicleUnitType Type)
{
    OnStartWeaponReloading.Broadcast(Type);
}