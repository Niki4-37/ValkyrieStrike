// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Turret.h"
#include "Weapon/DefaultProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon/DefaultProjectile.h"
#include "Net/UnrealNetwork.h"
#include "AI/AITurretController.h"

ATurret::ATurret()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = false;

    PawnRootComponent = CreateDefaultSubobject<USceneComponent>("PawnRootComponent");
    SetRootComponent(PawnRootComponent);

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
    TurretMesh->SetupAttachment(RootComponent);

    AIControllerClass = AAITurretController::StaticClass();
}

void ATurret::RotateToTarget(const FRotator& Direction)
{
    // float YawValue = GetActorRotation().Yaw + Amount;
    //  SetActorRotation(FQuat(FRotator(0.f, YawValue, 0.f)));
    PawnRootComponent->SetWorldRotation(FQuat(Direction));
}

void ATurret::Fire_OnServer_Implementation(bool bIsPressed)
{
    if (bIsPressed)
    {
        GetWorldTimerManager().SetTimer(FireTimer, this, &ATurret::MakeShot, FireRate, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(FireTimer);
    }
}

void ATurret::BeginPlay()
{
    Super::BeginPlay();
}

void ATurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ATurret::MakeShot()
{
    /* handled on server */
    UE_LOG(LogTemp, Display, TEXT("Fire"));
    auto Rotation = GetActorForwardVector().Rotation();

    auto MuzzleLocation = GetActorLocation() + Rotation.RotateVector(MuzzleOffset);
    FTransform SpawningTransform(Rotation, MuzzleLocation);

    auto Bullet = GetWorld()->SpawnActor<ADefaultProjectile>(DefaultProjectileClass, SpawningTransform);
    if (Bullet)
    {
        UE_LOG(LogTemp, Display, TEXT("Location %s"), *Bullet->GetActorLocation().ToString());
        Bullet->SetLifeSpan(1.f);
    }
}
