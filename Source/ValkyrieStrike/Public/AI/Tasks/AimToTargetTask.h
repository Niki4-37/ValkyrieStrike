// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AimToTargetTask.generated.h"

UCLASS()
class VALKYRIESTRIKE_API UAimToTargetTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UAimToTargetTask();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector AimActorKey;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
