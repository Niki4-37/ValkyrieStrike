// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/SecondWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include "Engine.h"

void ASecondWeapon::MakeShot()
{
    Super::MakeShot();

    if (IsReloading() || IsEmpty()) return;

    const FVector TraceStart = Gun->GetSocketLocation(MuzzleSocketName);
    const FVector TraceEnd = TraceStart + Gun->GetSocketRotation(MuzzleSocketName).Vector() * 2000.f;

    TArray<FHitResult> Hits;
    // UKismetSystemLibrary::SphereTraceMulti(GetWorld(),                                                           //
    //                                        TraceStart,                                                           //
    //                                        TraceEnd,                                                             //
    //                                        BeemRadius,                                                           //
    //                                        UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),  //
    //                                        false,                                                                //
    //                                        {GetOwner(), this},                                                   //
    //                                        EDrawDebugTrace::ForDuration,                                         //
    //                                        Hits,                                                                 //
    //                                        true);

    UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),                    //
                                                     TraceStart,                    //
                                                     TraceEnd,                      //
                                                     BeemRadius,                    //
                                                     TreceForObjectTypes,           //
                                                     false,                         //
                                                     {GetOwner(), this},            //
                                                     EDrawDebugTrace::ForDuration,  //
                                                     Hits,                          //
                                                     true);                         //

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

void ASecondWeapon::BeginPlay()
{
    Super::BeginPlay();
}