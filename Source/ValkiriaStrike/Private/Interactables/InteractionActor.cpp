// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Interactables/InteractionActor.h"
#include "Net/UnrealNetwork.h"

AInteractionActor::AInteractionActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ActorRootComponent = CreateDefaultSubobject<USceneComponent>("ActorRootComponent");
    SetRootComponent(ActorRootComponent);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetIsReplicated(true);
}

void AInteractionActor::BeginPlay()
{
    Super::BeginPlay();
}

void AInteractionActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AInteractionActor, bHasOwner);
}
