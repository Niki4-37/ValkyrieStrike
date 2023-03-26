// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/Services/FireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapon/Turret.h"

UFireService::UFireService()
{
    NodeName = "Fire Service";
}

void UFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Controller)
    {
        // const auto TurretPawn = Cast<ATurret>(Controller->GetPawn());
        // if (TurretPawn)
        //{
        //     HasAim ? TurretPawn->Fire_OnServer(true) : TurretPawn->Fire_OnServer(false);
        // }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
