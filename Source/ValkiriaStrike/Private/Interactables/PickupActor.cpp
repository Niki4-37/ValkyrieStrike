// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Interactables/PickupActor.h"
#include "Components/SphereComponent.h"
#include "Interfaces/GameInterface.h"

#include "Engine.h"

APickupActor::APickupActor()
{
    bReplicates = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(300.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionComponent->SetupAttachment(RootComponent);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "APickupActor::NotifyActorBeginOverlap");

    if (!OtherActor || bHasOwner || SavedActors.Contains(OtherActor) || !OtherActor->ActorHasTag("Player")) return;
    Alpha = 0.f;
    bHasOwner = true;
    SavedActors.Add(OtherActor);

    const FVector FromLocation = GetActorLocation();
    const FVector ToLocation = OtherActor->GetActorLocation();

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&, FromLocation, ToLocation, OtherActor]()
        {
            if (IsAppliedToActor(OtherActor)) return;

            const FVector DeltaPosition = FMath::Lerp(FromLocation, ToLocation, Alpha);
            SetActorLocation(FVector(DeltaPosition.X, DeltaPosition.Y, GetActorLocation().Z));

            Alpha += 0.01f;
        });

    GetWorldTimerManager().SetTimer(MoveTimer, TimerDelegate, MoveRate / 100.f, true);
}

void APickupActor::NotifyActorEndOverlap(AActor* OtherActor)
{
    SavedActors.Remove(OtherActor);
}

bool APickupActor::IsAppliedToActor(AActor* OtherActor)
{
    if (Alpha < 0.7f) return false;

    auto Interface = Cast<IGameInterface>(OtherActor);
    if (Interface && Interface->AddAmount(Data))
    {
        Destroy();
    }

    bHasOwner = false;
    GetWorldTimerManager().ClearTimer(MoveTimer);
    return true;
}
