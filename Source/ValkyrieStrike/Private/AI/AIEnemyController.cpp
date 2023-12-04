// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/AIEnemyController.h"
#include "Components/UniversalAIPerceptionComponent.h"
#include "Components/RespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacter.h"

AAIEnemyController::AAIEnemyController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UUniversalAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*AIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnComponent");
}

void AAIEnemyController::StartFire(AActor* AimActor)
{
    const auto ControlledCharacter = Cast<AAICharacter>(GetPawn());
    if (!ControlledCharacter) return;
    ControlledCharacter->AttackEnemy(AimActor);
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (const auto ControlledCharacter = Cast<AAICharacter>(InPawn))
    {
        RunBehaviorTree(ControlledCharacter->BehaviorTreeAsset);
    }
}
