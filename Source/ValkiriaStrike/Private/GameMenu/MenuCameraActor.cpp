// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuCameraActor.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

AMenuCameraActor::AMenuCameraActor()
{
    PrimaryActorTick.bCanEverTick = false;
    ActorRootComponent = CreateDefaultSubobject<USceneComponent>("ActorRootComponent");
    SetRootComponent(ActorRootComponent);
    // MenuActorCamera = CreateDefaultSubobject<ACameraActor>("MenuActorCamera");
    CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComp->SetupAttachment(ActorRootComponent);
}

void AMenuCameraActor::BeginPlay()
{
    Super::BeginPlay();
}
