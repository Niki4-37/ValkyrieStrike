// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Decorations/DecorationActor.h"
#include "Components/HealthComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "DrawDebugHelpers.h"

ADecorationActor::ADecorationActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ActorRootComp = CreateDefaultSubobject<USceneComponent>("ActorRootComp");
    SetRootComponent(ActorRootComp);

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    MeshComp->SetMobility(EComponentMobility::Movable);
    MeshComp->SetupAttachment(ActorRootComp);
    MeshComp->SetIsReplicated(true);

    HitPointComponent = CreateDefaultSubobject<UHealthComponent>("HitPointComponent");
}

void ADecorationActor::SetupDecoration(UStaticMesh* Mesh)
{
    if (!MeshComp || !Mesh) return;
    MeshComp->SetRelativeScale3D(FVector(1.f));
    MeshComp->SetStaticMesh(Mesh);
}

void ADecorationActor::IsDealsDamage(bool bEnabled, float InDamage, float InRadius)
{
    bDealsDamage = bEnabled;
    Damage = InDamage;
    Radius = InRadius;
}

void ADecorationActor::SetDestructionTimer(float InTime)
{
    if (!GetWorld()) return;

    GetWorldTimerManager().SetTimer(DestructionTimer, this, &ADecorationActor::OnDeath, InTime);
}

void ADecorationActor::Throw_Multicast_Implementation()
{
    Throw();
}

void ADecorationActor::ThrowTo_Multicast_Implementation(const FVector& ToLocation)
{
    ThrowTo(ToLocation);
}

void ADecorationActor::Throw()
{
    if (!GetWorld()) return;

    FHitResult Hit;
    const FVector TraceStart = GetActorLocation();
    const FVector TraceEnd = TraceStart + GetActorUpVector() * (-1000.f);
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Orange, false, 5.f, 0, 5.f);

    if (Hit.bBlockingHit)
    {
        const float Height{Hit.ImpactPoint.Z};

        FTimerDelegate TimerDelegate;
        TimerDelegate.BindLambda(
            [&, Height]()
            {
                Impulse -= Gravity;
                float LocationZ = GetActorLocation().Z + Impulse;

                if (LocationZ < Height)
                {
                    GetWorldTimerManager().ClearTimer(TrowTimer);
                    return;
                }
                SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, LocationZ));
            });
        GetWorldTimerManager().SetTimer(TrowTimer, TimerDelegate, 0.01f, true);
    }
}

void ADecorationActor::ThrowTo(const FVector& ToLocation)
{
    const FVector FromLocation = GetActorLocation();
    Alpha = 0.f;

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [this, ToLocation, FromLocation]
        {
            Impulse -= Gravity;
            float LocationZ = GetActorLocation().Z + Impulse;

            if (Alpha > 1.f || LocationZ < ToLocation.Z)
            {
                GetWorldTimerManager().ClearTimer(TrowTimer);
                return;
            }

            const FVector DeltaPosition = FMath::Lerp(FromLocation, ToLocation, Alpha);
            SetActorLocation(FVector(DeltaPosition.X, DeltaPosition.Y, LocationZ));

            Alpha += 0.01;
        });
    GetWorldTimerManager().SetTimer(TrowTimer, TimerDelegate, 0.01f, true);
}

void ADecorationActor::BeginPlay()
{
    Super::BeginPlay();

    check(HitPointComponent);

    HitPointComponent->OnDeath.AddUObject(this, &ADecorationActor::OnDeath);
}

void ADecorationActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void ADecorationActor::OnDeath()
{
    PlayFXEffect_Multicast(DestructionEffect);

    if (bDealsDamage)
    {
        UGameplayStatics::ApplyRadialDamage(GetWorld(),          //
                                            Damage,              //
                                            GetActorLocation(),  //
                                            Radius,              //
                                            DamageType,          //
                                            {});
        DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 16, FColor::Red, false, 5.f);
    }

    GetWorldTimerManager().ClearTimer(DestructionTimer);
    GetWorldTimerManager().ClearTimer(TrowTimer);

    Destroy();
}

void ADecorationActor::PlayFXEffect_Multicast_Implementation(UNiagaraSystem* FXEffect)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXEffect, GetActorLocation());
}
