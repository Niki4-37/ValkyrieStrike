// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UHealthComponent;

UCLASS()
class VALKIRIASTRIKE_API AAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAICharacter();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UHealthComponent* HealthComponent;

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
