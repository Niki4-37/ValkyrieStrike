// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Decorations/DecorationActor.h"

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
}

void ADecorationActor::SetupDecoration(UStaticMesh* Mesh)
{
    if (!MeshComp || !Mesh) return;
    MeshComp->SetRelativeScale3D(FVector(1.f));
    MeshComp->SetStaticMesh(Mesh);
}

void ADecorationActor::BeginPlay()
{
    Super::BeginPlay();
}
