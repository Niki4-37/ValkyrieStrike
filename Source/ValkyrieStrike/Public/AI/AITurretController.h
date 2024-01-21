// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITurretController.generated.h"

class UUniversalAIPerceptionComponent;
class ATurretHubPawn;

UCLASS()
class VALKYRIESTRIKE_API AAITurretController : public AAIController
{
    GENERATED_BODY()

public:
    AAITurretController();

    void StartChoosingTarget();
    virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UUniversalAIPerceptionComponent* TurretHubPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AimingUpdateRate{0.1f};

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    FTimerHandle ChoosingTargetTimer;

    UPROPERTY()
    ATurretHubPawn* TurretHub{nullptr};

    void RotateToTarget();
};
