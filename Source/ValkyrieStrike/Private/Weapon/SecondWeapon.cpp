// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/SecondWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "DrawDebugHelpers.h"
#include "Engine.h"

ASecondWeapon::ASecondWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    WeaponRootComponent = CreateDefaultSubobject<USceneComponent>("WeaponRootComponent");
    SetRootComponent(WeaponRootComponent);

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(RootComponent);
}

bool ASecondWeapon::ChangeAmmoCapacity(int32 Value)
{
    /** handled on server */
    bool bCanChanged{AmmoCapacity < MaxAmmoCapacity};

    if (IsEmpty() && Value > 0)
    {
        ReloadWeapon();
    }

    AmmoCapacity = FMath::Clamp(AmmoCapacity + Value, 0, MaxAmmoCapacity);

    OnChangeAmmoInWeapon.Broadcast(EVehicleUnitType::SecondWeapon, AmmoCapacity);
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(AmmoCapacity));

    return bCanChanged;
}

void ASecondWeapon::SetupWeapon(int32 InMaxAmmoCapacity, float InReloadingTime)
{
    MaxAmmoCapacity = InMaxAmmoCapacity;
    ReloadingTime = InReloadingTime;
    ChangeAmmoCapacity(MaxAmmoCapacity);
}

bool ASecondWeapon::MakeShot()
{
    if (!bIsReady || !WeaponMesh || IsEmpty()) return false;

    bIsReady = false;

    const FVector TraceStart = WeaponMesh->GetSocketLocation(MuzzleSocketName);
    const FVector TraceEnd = TraceStart + WeaponMesh->GetSocketRotation(MuzzleSocketName).Vector() * 2000.f;
    // FHitResult Hit;
    // FCollisionQueryParams CollisionParams;
    // GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
    // if (Hit.bBlockingHit)
    //{
    //     DrawDebugLine(GetWorld(), TraceStart, Hit.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
    // }
    // else
    //{
    //     DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
    // }

    // UKismetSystemLibrary::SphereTraceSingle(GetWorld(),                                                           //
    //                                         TraceStart,                                                           //
    //                                         TraceEnd,                                                             //
    //                                         BeemRadius,                                                           //
    //                                         UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),  //
    //                                         false,                                                                //
    //                                         {GetOwner(), this},                                                   //
    //                                         EDrawDebugTrace::ForDuration,                                         //
    //                                         Hit,                                                                  //
    //                                         true);

    TArray<FHitResult> Hits;
    UKismetSystemLibrary::SphereTraceMulti(GetWorld(),                                                           //
                                           TraceStart,                                                           //
                                           TraceEnd,                                                             //
                                           BeemRadius,                                                           //
                                           UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),  //
                                           false,                                                                //
                                           {GetOwner(), this},                                                   //
                                           EDrawDebugTrace::ForDuration,                                         //
                                           Hits,                                                                 //
                                           true);
    /* debug */
    TArray<AActor*> HitActors;
    for (auto& Hit : Hits)
    {
        if (!Hit.GetActor()) continue;
        HitActors.AddUnique(Hit.GetActor());
    }

    for (auto HitActor : HitActors)
    {
        UGameplayStatics::ApplyDamage(HitActor, WeaponDamage, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, HitActor->GetName());
    }

    ChangeAmmoCapacity(-1);

    if (!IsEmpty())
    {
        ReloadWeapon();
    }

    return true;
}

void ASecondWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ASecondWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASecondWeapon, bIsReady);
    DOREPLIFETIME(ASecondWeapon, AmmoCapacity);
    DOREPLIFETIME(ASecondWeapon, MaxAmmoCapacity);
}

void ASecondWeapon::ReloadWeapon()
{
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda([&]() { bIsReady = true; });
    GetWorldTimerManager().SetTimer(ReloadingTimer, TimerDelegate, ReloadingTime, false);

    OnStartWeaponReloading.Broadcast(EVehicleUnitType::SecondWeapon);
}