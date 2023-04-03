// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

class UUniversalAIPerceptionComponent;

UCLASS()
class VALKIRIASTRIKE_API AAIEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AAIEnemyController();

    void StartFire(bool bEnebled);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UUniversalAIPerceptionComponent* AIPerceptionComponent;

    virtual void OnPossess(APawn* InPawn) override;
};
