// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.


#include "AI/Tasks/BTTask_FindNewBomb.h"
#include "GameLevelsConfig/FirstLevelGameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactables/SpawningActor.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_FindNewBomb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!GetWorld()) return EBTNodeResult::Failed;
    const auto FirstLevelGameMode = GetWorld()->GetAuthGameMode<AFirstLevelGameModeBase>();
    if (!FirstLevelGameMode) return EBTNodeResult::Failed;
    
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController || !AIController->GetPawn()) return EBTNodeResult::Failed;

    float BestDistance = MAX_FLT;
    AActor* ClosestBomb{nullptr};
    for (const auto SpawnActor : FirstLevelGameMode->GetSpawningActors())
    {
        if (!SpawnActor) continue;
        const auto CurrentDistance = (SpawnActor->GetActorLocation() - AIController->GetPawn()->GetActorLocation()).Size();
        if (CurrentDistance < BestDistance)
        {
            BestDistance = CurrentDistance;
            ClosestBomb = SpawnActor;
        }
    }
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;
    Blackboard->SetValueAsObject(NewBombKey.SelectedKeyName, ClosestBomb);
    return EBTNodeResult::Succeeded;
}
