// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Services/FindEnemyService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/UniversalAIPerceptionComponent.h"

UFindEnemyService::UFindEnemyService()
{
    NodeName = "Find Enemy";
}

void UFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = Controller->FindComponentByClass<UUniversalAIPerceptionComponent>();
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy(EnemyTag));
            // UE_LOG(LogTemp, Display, TEXT("EnemyFound"));
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
