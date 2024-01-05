// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/DropComponent.h"
#include "Interactables/PickupActor.h"

UDropComponent::UDropComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UDropComponent::DropItem()
{
    /** handled on server */
    if (!DropElements.Num()) return;

    if (!GetWorld() || !GetOwner() || !PickupActorClass) return;
    FTransform SpawnTransform(FRotator::ZeroRotator, GetOwner()->GetActorLocation());
    const auto DropElement = Roll();
    if (!DropElement.PickupMesh) return;
    auto Pickup = GetWorld()->SpawnActorDeferred<APickupActor>(PickupActorClass, SpawnTransform);
    if (Pickup)
    {
        Pickup->SetupPickup(DropElement.PickupMesh, DropElement.DropData);
        Pickup->FinishSpawning(SpawnTransform);

        Pickup->ThrowUp_Multicast(GetOwner()->GetActorLocation());
    }
}

void UDropComponent::BeginPlay()
{
    Super::BeginPlay();

    for (const auto& DropElement : DropElements)
    {
        SumOfWeights += DropElement.DropChance;
    }
}

const FDropComponentElement& UDropComponent::Roll()
{
    const auto Chance = FMath::RandRange(0.f, SumOfWeights);
    float CumulativeWeights{0};

    auto FoundElement = DropElements.FindByPredicate(
        [Chance, &CumulativeWeights](const FDropComponentElement& Element)
        {
            CumulativeWeights += Element.DropChance;
            return Chance < CumulativeWeights;
        });

    if (FoundElement)
    {
        return *FoundElement;
    }

    return DropElements[0];
}