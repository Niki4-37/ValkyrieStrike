// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/BaseVehicleWeapon.h"
#include "Net/UnrealNetwork.h"  //to delete

#include "Engine.h"

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
    GetWorldTimerManager().ClearTimer(RotationTimer);
    bHasAim = NewAimActor != nullptr;
    auto TimerDelegate = FTimerDelegate::CreateUObject(this, &ABaseVehicleWeapon::RotateToTarget, NewAimActor);
    GetWorldTimerManager().SetTimer(RotationTimer, TimerDelegate, UpdateTimeRate / 10.f, true);
}

void ABaseVehicleWeapon::MakeShot()
{
    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "ABaseVehicleWeapon::MakeShot");
    /* handled on server */

    // const FVector TraceStart = Gun->GetSocketLocation(MuzzleSocketName);
    // const FVector TraceEnd = TraceStart + Gun->GetSocketRotation(MuzzleSocketName).Vector() * 2000.f;

    // TArray<FHitResult> Hits;
    // UKismetSystemLibrary::SphereTraceMulti(GetWorld(),                                                           //
    //                                        TraceStart,                                                           //
    //                                        TraceEnd,                                                             //
    //                                        /*BeemRadius*/ 10.f,                                                  //
    //                                        UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),  //
    //                                        false,                                                                //
    //                                        {GetOwner(), this},                                                   //
    //                                        EDrawDebugTrace::ForDuration,                                         //
    //                                        Hits,                                                                 //
    //                                        true);
    ///* debug */
    // TArray<AActor*> HitActors;
    // for (auto& Hit : Hits)
    //{
    //     if (!Hit.GetActor()) continue;
    //     HitActors.AddUnique(Hit.GetActor());
    // }

    // for (auto HitActor : HitActors)
    //{
    //     // UGameplayStatics::ApplyDamage(HitActor, WeaponDamage, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
    //     GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, HitActor->GetName());
    // }
}

void ABaseVehicleWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseVehicleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
    const auto NoneAimLocation = GetOwner() ? GetOwner()->GetActorForwardVector() * 1000.f : FVector::ZeroVector;

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

    Platform->SetRelativeRotation(FRotator(0.f, ValueToSet, 0.f));
}

void ABaseVehicleWeapon::ChangeAmmo(int32 Value)
{
    AmmoCapacity = FMath::Clamp(AmmoCapacity + Value, 0, MaxAmmoCapacity);
    OnChangeAmmoInWeapon_OnClient(WeaponType, AmmoCapacity);
}

void ABaseVehicleWeapon::ReloadWeapon()
{
    if (bIsReloading) return;
    bIsReloading = true;
    GetWorldTimerManager().SetTimer(
        ReloadingTimer,
        [&]()
        {
            bIsReloading = false;
            AmmoCapacity = MaxAmmoCapacity;
            OnChangeAmmoInWeapon_OnClient(WeaponType, AmmoCapacity);
        },
        ReloadingTime, false);
    OnStartWeaponReloading_OnClient(WeaponType);
}

void ABaseVehicleWeapon::OnChangeAmmoInWeapon_OnClient_Implementation(EVehicleUnitType Type, int32 Value)
{
    OnChangeAmmoInWeapon.Broadcast(Type, Value);
}

void ABaseVehicleWeapon::OnStartWeaponReloading_OnClient_Implementation(EVehicleUnitType Type)
{
    OnStartWeaponReloading.Broadcast(Type);
}