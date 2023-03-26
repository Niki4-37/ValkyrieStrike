// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Tasks/ResetAimTask.h"
#include "BehaviorTree/BlackboardComponent.h"

UResetAimTask::UResetAimTask()
{
    NodeName = "Clear Aiming Actor";
}

EBTNodeResult::Type UResetAimTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    Blackboard->SetValueAsObject(AimActorKey.SelectedKeyName, nullptr);
    return EBTNodeResult::Succeeded;
}
