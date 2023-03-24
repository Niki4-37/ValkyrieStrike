// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponent_LOG, All, All);

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealthComponent, Health);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::SetHealth(float NewHealth)
{
    float DeltaHealth = NewHealth - Health;
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
    // OnHealthChanged_OnClient(Health, MaxHealth, DeltaHealth);
    UE_LOG(HealthComponent_LOG, Display, TEXT("SetHealth: %f"), Health);
}
