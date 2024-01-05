// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Tasks/BTTask_AttackEnemy.h"
#include "AIController.h"
#include "Interfaces/BattleInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_AttackEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;
    const auto BattleInterface = Cast<IBattleInterface>(AIController->GetPawn());
    if (!BattleInterface) return EBTNodeResult::Failed;
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;
    BattleInterface->AttackEnemy(Cast<AActor>(Blackboard->GetValueAsObject(FoundEnemyKey.SelectedKeyName)));

    return EBTNodeResult::Succeeded;
}
