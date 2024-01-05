// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Decorators/BTDecorator_IsCloseDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

bool UBTDecorator_IsCloseDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return false;
    const auto EnemyActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if (!EnemyActor) return false;
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController || !AIController->GetPawn()) return false;
    const float Distance = (EnemyActor->GetActorLocation() - AIController->GetPawn()->GetActorLocation()).Size();
    return Distance > MinimalDistance;
}
