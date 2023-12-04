// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Interactables/SpawningActor.h"

ASpawningActor::ASpawningActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASpawningActor::SpawnEnemy(UClass* Class) {}

void ASpawningActor::BeginPlay()
{
    Super::BeginPlay();
}
