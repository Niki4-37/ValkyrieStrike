// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/SecondWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

#include "DrawDebugHelpers.h"

ASecondWeapon::ASecondWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    WeaponRootComponent = CreateDefaultSubobject<USceneComponent>("WeaponRootComponent");
    SetRootComponent(WeaponRootComponent);

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(RootComponent);
}

void ASecondWeapon::MakeShot_OnServer_Implementation()
{
    if (!bIsReady || !WeaponMesh) return;

    bIsReady = false;

    const FVector TraceStart = WeaponMesh->GetSocketLocation(MuzzleSocketName);
    const FVector TraceEnd = TraceStart + WeaponMesh->GetSocketRotation(MuzzleSocketName).Vector() * 2000.f;
    FHitResult Hit;
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

    UKismetSystemLibrary::SphereTraceSingle(GetWorld(),                                                           //
                                            TraceStart,                                                           //
                                            TraceEnd,                                                             //
                                            BeemRadius,                                                           //
                                            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),  //
                                            false,                                                                //
                                            {GetOwner(), this},                                                   //
                                            EDrawDebugTrace::ForDuration,                                         //
                                            Hit,                                                                  //
                                            true);

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda([&]() { bIsReady = true; });
    GetWorldTimerManager().SetTimer(ReloadingTimer, TimerDelegate, ReloadingTime, false);
}

void ASecondWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ASecondWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASecondWeapon, bIsReady);
}
