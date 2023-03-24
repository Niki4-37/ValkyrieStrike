// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/UniversalAIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/HealthComponent.h"
#include "AIController.h"

AActor* UUniversalAIPerceptionComponent::GetClosestEnemy(FRotator& Direction) const
{
    Direction = FRotator::ZeroRotator;
    TArray<AActor*> PercievedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercievedActors);
    if (PercievedActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;
    for (const auto PercievedActor : PercievedActors)
    {
        const auto HealthComponent = PercievedActor->FindComponentByClass<UHealthComponent>();
        if (HealthComponent && !HealthComponent->IsDead())  // TODO: check if enemies or not
        {
            const auto CurrentDistance = (PercievedActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercievedActor;
            }
        }
    }

    /* find Direction to Enemy */
    if (BestPawn)
    {
        Direction = FRotationMatrix::MakeFromX(BestPawn->GetActorLocation() - Pawn->GetActorLocation()).Rotator();
    }

    return BestPawn;
}
