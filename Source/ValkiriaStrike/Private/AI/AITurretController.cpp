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

void AAITurretController::SetAimActor(AActor* AimActor)
{
    auto TimerDelegate = FTimerDelegate::CreateUObject(this, &AAITurretController::RotateToTarget, AimActor);
    GetWorldTimerManager().SetTimer(AimingTimer, TimerDelegate, AimingUpdateRate, true);
}

void AAITurretController::RotateToTarget(AActor* AimActor)
{
    if (AimActor && TurretPawn)
    {
        const FVector AimLocation = AimActor->GetActorLocation();
        const FVector TurretLocation = TurretPawn->GetActorLocation();
        const FRotator Direction = FRotationMatrix::MakeFromX(AimLocation - TurretLocation).Rotator();

        TurretPawn->RotateToTarget(Direction, AimingUpdateRate);
    }
}

void AAITurretController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    TurretPawn = Cast<ATurret>(InPawn);
    if (TurretPawn)
    {
        RunBehaviorTree(TurretPawn->BehaviorTreeAsset);
    }
}

void AAITurretController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
