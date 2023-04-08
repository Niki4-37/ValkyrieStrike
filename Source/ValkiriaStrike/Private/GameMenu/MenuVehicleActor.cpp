// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuVehicleActor.h"

AMenuVehicleActor::AMenuVehicleActor()
{
    PrimaryActorTick.bCanEverTick = false;

    ActorRootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
    SetRootComponent(ActorRootComp);

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
    Mesh->SetupAttachment(ActorRootComp);
}

void AMenuVehicleActor::BeginPlay()
{
    Super::BeginPlay();
}
