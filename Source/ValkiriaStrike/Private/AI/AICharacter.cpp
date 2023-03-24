// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AICharacter.h"
#include "Components/HealthComponent.h"

AAICharacter::AAICharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void AAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    HealthComponent->SetHealth(100.f);
}

void AAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
