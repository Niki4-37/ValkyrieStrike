// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RunAway.generated.h"

UCLASS()
class VALKYRIESTRIKE_API UBTTask_RunAway : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector DestinationPointKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float RunAwayDistance{1000.f};

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
