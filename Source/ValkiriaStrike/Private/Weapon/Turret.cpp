// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/Turret.h"
#include "Weapon/DefaultProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon/DefaultProjectile.h"
#include "Net/UnrealNetwork.h"
#include "AI/AITurretController.h"
#include "TimerManager.h"

ATurret::ATurret()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = false;

    PawnRootComponent = CreateDefaultSubobject<USceneComponent>("PawnRootComponent");
    SetRootComponent(PawnRootComponent);

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
    TurretMesh->SetupAttachment(RootComponent);

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAITurretController::StaticClass();
}

void ATurret::RotateToTarget(const FRotator& Direction, float TimerRate)
{
    Alpha = 0.f;
    GetWorldTimerManager().ClearTimer(SmoothRotationTimer);

    const auto FromRotation = PawnRootComponent->GetComponentRotation();
    auto TimerDelegate = FTimerDelegate::CreateUObject(this, &ATurret::SmoothRotation, FromRotation, Direction);

    GetWorldTimerManager().SetTimer(SmoothRotationTimer, TimerDelegate, TimerRate / 10.f, true);
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

void ATurret::SmoothRotation(FRotator FromRotation, FRotator ToRotation)
{
    if (Alpha > 1.f)
    {
        GetWorldTimerManager().ClearTimer(SmoothRotationTimer);
        return;
    }
    const auto DeltaQuat = FQuat::Slerp(FQuat(FromRotation), FQuat(ToRotation), Alpha);
    PawnRootComponent->SetWorldRotation(FQuat(DeltaQuat));

    Alpha += 0.1f;
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
