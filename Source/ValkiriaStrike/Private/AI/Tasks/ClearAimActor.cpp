// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Tasks/ClearAimActor.h"
#include "BehaviorTree/BlackboardComponent.h"

UClearAimActor::UClearAimActor()
{
    NodeName = "Clear Aiming Actor";
}

EBTNodeResult::Type UClearAimActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    Blackboard->SetValueAsObject(AimActorKey.SelectedKeyName, nullptr);
    return EBTNodeResult::Succeeded;
}
