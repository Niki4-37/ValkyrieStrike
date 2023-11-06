// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/SecondWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "DrawDebugHelpers.h"
#include "Engine.h"

void ASecondWeapon::MakeShot()
{
    Super::MakeShot();

    if (IsReloading() || IsEmpty()) return;

    const FVector TraceStart = Gun->GetSocketLocation(MuzzleSocketName);
    const FVector TraceEnd = TraceStart + Gun->GetSocketRotation(MuzzleSocketName).Vector() * 2000.f;

    TArray<FHitResult> Hits;
    UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),             //
                                                     TraceStart,             //
                                                     TraceEnd,               //
                                                     BeemRadius,             //
                                                     TreceForObjectTypes,    //
                                                     false,                  //
                                                     {GetOwner(), this},     //
                                                     EDrawDebugTrace::None,  //
                                                     Hits,                   //
                                                     true);                  //

    SpawnTraceFX_Multicast(TraceStart, TraceEnd);

    TArray<AActor*> HitActors;
    for (auto& Hit : Hits)
    {
        if (!Hit.GetActor()) continue;
        HitActors.AddUnique(Hit.GetActor());
    }

    for (auto HitActor : HitActors)
    {
        UGameplayStatics::ApplyDamage(HitActor, WeaponDamage, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
        // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, HitActor->GetName());
    }

    ChangeAmmo(-1);

    if (!IsEmpty())
    {
        Recharge();
    }
}

void ASecondWeapon::AlternativeShot()
{
    Super::AlternativeShot();
    MakeShot();
}

void ASecondWeapon::SpawnTraceFX_Multicast_Implementation(const FVector& TraceStart, const FVector& TraceEnd)
{
    SpawnTraceFX(TraceStart, TraceEnd);
}

void ASecondWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableFloat(BeamWidthName, BeemRadius * 2.f);
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}

void ASecondWeapon::BeginPlay()
{
    Super::BeginPlay();
}