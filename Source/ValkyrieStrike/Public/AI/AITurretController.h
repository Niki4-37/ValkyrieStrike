// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITurretController.generated.h"

class UUniversalAIPerceptionComponent;
class ATurret;

UCLASS()
class VALKYRIESTRIKE_API AAITurretController : public AAIController
{
    GENERATED_BODY()

public:
    AAITurretController();

    void StartChoosingTarget();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UUniversalAIPerceptionComponent* TurretPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AimingUpdateRate{0.1f};

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    FTimerHandle AimingTimer;

    FTimerHandle ChoosingTargetTimer;

    UPROPERTY()
    ATurret* TurretPawn{nullptr};

    void RotateToTarget();
};
