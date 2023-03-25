// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Tasks/AimToTargetTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AITurretController.h"

UAimToTargetTask::UAimToTargetTask()
{
    NodeName = "Aim To Target";
}

EBTNodeResult::Type UAimToTargetTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto TurretController = Cast<AAITurretController>(OwnerComp.GetAIOwner());

    if (!Blackboard || !TurretController) return EBTNodeResult::Failed;

    TurretController->SetAimActor(Cast<AActor>(Blackboard->GetValueAsObject(AimActorKey.SelectedKeyName)));
    return EBTNodeResult::Succeeded;
}
