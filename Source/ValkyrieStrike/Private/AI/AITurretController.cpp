// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/AITurretController.h"
#include "Components/UniversalAIPerceptionComponent.h"
#include "Weapon/TurretHubPawn.h"
#include "BehaviorTree/BlackboardComponent.h"

AAITurretController::AAITurretController()
{
    TurretHubPerceptionComponent = CreateDefaultSubobject<UUniversalAIPerceptionComponent>("TurretHubPerceptionComponent");
    SetPerceptionComponent(*TurretHubPerceptionComponent);
}

void AAITurretController::StartChoosingTarget()
{
    GetWorldTimerManager().SetTimer(ChoosingTargetTimer, this, &AAITurretController::RotateToTarget, AimingUpdateRate, true);
}

void AAITurretController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    StartChoosingTarget();

    TurretHub = Cast<ATurretHubPawn>(InPawn);
}

void AAITurretController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAITurretController::RotateToTarget()
{
    if (!TurretHub)
    {
        GetWorldTimerManager().ClearTimer(ChoosingTargetTimer);
        return;
    }

    TurretHub->UpdateAimActor(TurretHubPerceptionComponent->GetClosestEnemy("Enemy"), AimingUpdateRate);
}
