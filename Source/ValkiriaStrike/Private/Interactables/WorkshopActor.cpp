// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Interactables/WorkshopActor.h"
#include "Interfaces/GameInterface.h"
#include "Components/BoxComponent.h"

AWorkshopActor::AWorkshopActor()
{
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
    CollisionComponent->SetBoxExtent(FVector(300.f));
    CollisionComponent->SetupAttachment(ActorRootComponent);

    WorkshopMesh = CreateDefaultSubobject<UStaticMeshComponent>("WorkshopMesh");
    WorkshopMesh->SetupAttachment(ActorRootComponent);
}

void AWorkshopActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    const auto GameInterface = Cast<IGameInterface>(OtherActor);
    if (GameInterface)
    {
        GameInterface->SetWorkshopTasksData(WorkshopTasks);
    }
}

void AWorkshopActor::NotifyActorEndOverlap(AActor* OtherActor)
{
    const auto GameInterface = Cast<IGameInterface>(OtherActor);
    if (GameInterface)
    {
        GameInterface->SetWorkshopTasksData(TArray<FInteractionData>{});
    }
}
