// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Services/FireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIEnemyController.h"

UFireService::UFireService()
{
    NodeName = "Fire Service";
}

void UFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (Blackboard)
    {
        const auto AimActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));

        if (const auto AIEnemyController = Cast<AAIEnemyController>(Controller))
        {
            AIEnemyController->StartFire(AimActor);
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
