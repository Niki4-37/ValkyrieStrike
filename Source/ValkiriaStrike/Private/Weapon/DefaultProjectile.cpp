// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/DefaultProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"

ADefaultProjectile::ADefaultProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetSphereRadius(16.f);
    SetRootComponent(CollisionComponent);
    CollisionComponent->bReturnMaterialOnMove = true;      // hit Event returns PhysMaterial
    CollisionComponent->SetCanEverAffectNavigation(true);  // collision not affect navigation

    DefaultProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    DefaultProjectileMesh->SetupAttachment(CollisionComponent);
    DefaultProjectileMesh->SetCanEverAffectNavigation(false);
    DefaultProjectileMesh->SetMobility(EComponentMobility::Movable);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("UDefaultProjectileMovementComponent");
    ProjectileMovementComponent->UpdatedComponent = RootComponent;  //???
    ProjectileMovementComponent->InitialSpeed = 1000.f;
    ProjectileMovementComponent->MaxSpeed = 1000.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f;

    ProjectileMovementComponent->SetIsReplicated(true);
    ProjectileMovementComponent->bInterpMovement = true;
}

void ADefaultProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ADefaultProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
