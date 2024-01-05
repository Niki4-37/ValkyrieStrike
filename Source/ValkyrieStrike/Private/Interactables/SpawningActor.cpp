// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Interactables/SpawningActor.h"
#include "Components/SphereComponent.h"

ASpawningActor::ASpawningActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetSphereRadius(50.f);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionComponent->SetCanEverAffectNavigation(true);
}

void ASpawningActor::SpawnEnemy(UClass* Class) {}

void ASpawningActor::BeginPlay()
{
    Super::BeginPlay();
}
