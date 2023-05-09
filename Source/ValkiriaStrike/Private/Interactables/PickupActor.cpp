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

void APickupActor::SetupPickup(UStaticMesh* Mesh, UMaterialInterface* Material, const FInteractionData& InData)
{
    /** handled on server */
    if (Mesh)
    {
        MeshComponent->SetStaticMesh(Mesh);
        MeshComponent->SetRelativeScale3D(FVector(1.f));
        SetMaterial_Multicast(Material);
    }
    Data = InData;
}

void APickupActor::ThrowUp(const FVector& StartPosition)
{
    const float LocationX = StartPosition.X;
    const float LocationY = StartPosition.Y;
    const float LocationZ = StartPosition.Z;

    Height = LocationZ;

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&, LocationX, LocationY, LocationZ]
        {
            Impulse -= Gravity;
            Height += Impulse;

            if (Height < LocationZ)
            {
                GetWorldTimerManager().ClearTimer(TrowTimer);
                return;
            }
            SetActorLocation_Multicast(FVector(LocationX, LocationY, Height));

            GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, FVector(LocationX, LocationY, Height).ToString());
        });

    GetWorldTimerManager().SetTimer(TrowTimer, TimerDelegate, 0.01f, true);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
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
    if (OtherActor)
    {
        if (Alpha < 0.7f) return false;

        auto Interface = Cast<IGameInterface>(OtherActor);
        if (Interface && Interface->AddAmount(Data))
        {
            Destroy();
        }
    }

    bHasOwner = false;
    GetWorldTimerManager().ClearTimer(MoveTimer);
    return true;
}

void APickupActor::SetMaterial_Multicast_Implementation(UMaterialInterface* Material)
{
    if (!Material) return;
    MeshComponent->SetMaterial(0, Material);
}

void APickupActor::SetActorLocation_Multicast_Implementation(const FVector& NewLocation)
{
    SetActorLocation(NewLocation);
}