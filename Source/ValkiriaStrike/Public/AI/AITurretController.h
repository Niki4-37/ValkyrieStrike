// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITurretController.generated.h"

class UUniversalAIPerceptionComponent;
class ATurret;

UCLASS()
class VALKIRIASTRIKE_API AAITurretController : public AAIController
{
    GENERATED_BODY()

public:
    AAITurretController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UUniversalAIPerceptionComponent* TurretPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TurretRotationValue{20.f};

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    ATurret* TurretPawn{nullptr};
};
