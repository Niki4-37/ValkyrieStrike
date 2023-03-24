// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ADefaultProjectile;

UCLASS()
class VALKIRIASTRIKE_API ATurret : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATurret();

    UFUNCTION(Server, unreliable)
    void Fire_OnServer(bool bIsPressed);

    void RotateToTarget(const FRotator& Direction);

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* PawnRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FVector MuzzleOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRate{0.5f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ADefaultProjectile> DefaultProjectileClass;

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    FTimerHandle FireTimer;

    void MakeShot();
};
