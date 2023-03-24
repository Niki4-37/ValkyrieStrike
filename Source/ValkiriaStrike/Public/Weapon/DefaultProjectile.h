// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class VALKIRIASTRIKE_API ADefaultProjectile : public AActor
{
    GENERATED_BODY()

public:
    ADefaultProjectile();

protected:
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* DefaultProjectileMesh;

    UPROPERTY(VisibleDefaultsOnly)
    UProjectileMovementComponent* ProjectileMovementComponent{nullptr};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
