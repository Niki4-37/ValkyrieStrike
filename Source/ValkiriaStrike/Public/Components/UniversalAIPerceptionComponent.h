// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "UniversalAIPerceptionComponent.generated.h"

UCLASS()
class VALKIRIASTRIKE_API UUniversalAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetClosestEnemy(FRotator& Direction) const;
};
