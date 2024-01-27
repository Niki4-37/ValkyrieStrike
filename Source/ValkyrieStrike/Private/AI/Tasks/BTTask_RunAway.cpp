// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Tasks/BTTask_RunAway.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    const auto DirectionSpread = 25.f;
    const auto HalfRad = FMath::DegreesToRadians(DirectionSpread);
   
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController || !AIController->GetPawn()) return EBTNodeResult::Failed;
    const FVector RunAwayDirection3D = FMath::VRandCone(- AIController->GetPawn()->GetActorForwardVector(), HalfRad);

    FHitResult Hit;
    const float DirectionModifier = FVector::DotProduct(AIController->GetPawn()->GetActorUpVector(), RunAwayDirection3D) > 0.f ? -1.f : 1.f;
    const auto ProjectXY = FVector::VectorPlaneProject(RunAwayDirection3D, AIController->GetPawn()->GetActorUpVector());
    const FVector TraceStart = AIController->GetPawn()->GetActorLocation() + ProjectXY * RunAwayDistance;
    const FVector TraceEnd = TraceStart + AIController->GetPawn()->GetActorUpVector() * 1000.f * DirectionModifier;
    FCollisionQueryParams CollisionParams;
    GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
    if (Hit.bBlockingHit)
    {
        Blackboard->SetValueAsVector(DestinationPointKey.SelectedKeyName, Hit.ImpactPoint);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
