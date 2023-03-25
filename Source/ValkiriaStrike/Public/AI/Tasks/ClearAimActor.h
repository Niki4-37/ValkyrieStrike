// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ClearAimActor.generated.h"

/**
 *
 */
UCLASS()
class VALKIRIASTRIKE_API UClearAimActor : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UClearAimActor();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector AimActorKey;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
