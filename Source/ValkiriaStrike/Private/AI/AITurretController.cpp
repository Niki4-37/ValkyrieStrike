// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AITurretController.h"
#include "Components/UniversalAIPerceptionComponent.h"
#include "Weapon/Turret.h"

AAITurretController::AAITurretController()
{
    TurretPerceptionComponent = CreateDefaultSubobject<UUniversalAIPerceptionComponent>("TurretPerceptionComponent");
    SetPerceptionComponent(*TurretPerceptionComponent);
}

void AAITurretController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    TurretPawn = Cast<ATurret>(InPawn);
}

void AAITurretController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator Direction;
    const auto AimActor = TurretPerceptionComponent->GetClosestEnemy(Direction);
    if (TurretPawn && AimActor)
    {
        TurretPawn->RotateToTarget(Direction);
    }
}
