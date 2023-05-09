// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FindEnemyService.generated.h"

UCLASS()
class VALKYRIESTRIKE_API UFindEnemyService : public UBTService
{
    GENERATED_BODY()

public:
    UFindEnemyService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName EnemyTag{"Player"};

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
