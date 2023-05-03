// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/DropComponent.h"
#include "Interactables/PickupActor.h"

UDropComponent::UDropComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UDropComponent::DropItem()
{
    /** handled on server */
    if (!GetWorld() || !GetOwner() || !PickupActorClass || FMath::FRandRange(0.0f, 1.0f) > DropChance) return;
    FTransform SpawnTransform(FRotator::ZeroRotator, GetOwner()->GetActorLocation());
    auto Pickup = GetWorld()->SpawnActorDeferred<APickupActor>(PickupActorClass, SpawnTransform);
    if (Pickup)
    {
        Pickup->SetupPickup(PickupMesh, Material, DropData);
        Pickup->FinishSpawning(SpawnTransform);

        Pickup->ThrowUp(GetOwner()->GetActorLocation());
    }
}

void UDropComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UDropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
