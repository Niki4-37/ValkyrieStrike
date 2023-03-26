// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "AI/AITurretController.h"
#include "Components/UniversalAIPerceptionComponent.h"
#include "Weapon/Turret.h"
#include "BehaviorTree/BlackboardComponent.h"

AAITurretController::AAITurretController()
{
    TurretPerceptionComponent = CreateDefaultSubobject<UUniversalAIPerceptionComponent>("TurretPerceptionComponent");
    SetPerceptionComponent(*TurretPerceptionComponent);
}

void AAITurretController::StartChoosingTarget()
{
    GetWorldTimerManager().SetTimer(ChoosingTargetTimer, this, &AAITurretController::RotateToTarget, AimingUpdateRate, true);
}

void AAITurretController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    StartChoosingTarget();

    TurretPawn = Cast<ATurret>(InPawn);
}

void AAITurretController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAITurretController::RotateToTarget()
{
    if (!TurretPawn)
    {
        GetWorldTimerManager().ClearTimer(ChoosingTargetTimer);
        return;
    }

    TurretPawn->RotateToTarget(TurretPerceptionComponent->GetClosestEnemy(), AimingUpdateRate);
}
