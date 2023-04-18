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

    TurretTower = CreateDefaultSubobject<UStaticMeshComponent>("TurretTower");
    TurretTower->SetupAttachment(RootComponent);
    TurretTower->SetIsReplicated(true);

    TurretMuzzle = CreateDefaultSubobject<UStaticMeshComponent>("TurretMuzzle");
    TurretMuzzle->SetupAttachment(RootComponent);
    TurretMuzzle->SetIsReplicated(true);

    // AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    // AIControllerClass = AAITurretController::StaticClass();
}

void ATurret::RotateToTarget(AActor* AimActor, float TimerRate)
{
    Alpha = 0.f;
    GetWorldTimerManager().ClearTimer(SmoothRotationTimer);

    AimActor ? Fire_OnServer(true) : Fire_OnServer(false);

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&, AimActor]()
        {
            if (Alpha > 1.f)
            {
                GetWorldTimerManager().ClearTimer(SmoothRotationTimer);
                return;
            }

            const auto AimLocation = AimActor ? AimActor->GetActorLocation() : FVector::ZeroVector;
            const auto TurretLocatiom = GetActorLocation();
            const auto FromRotation = TurretMuzzle->GetComponentRotation();
            const FRotator Direction = FRotationMatrix::MakeFromX(AimLocation - TurretLocatiom).Rotator();
            FQuat DeltaQuat = FQuat::Slerp(FQuat(FromRotation), FQuat(Direction), Alpha);

            float YawValue = DeltaQuat.Rotator().Yaw;
            TurretTower->SetWorldRotation(FRotator(0.f, YawValue, 0.f));
            TurretMuzzle->SetWorldRotation(DeltaQuat);
            RotateTurret_OnServer(DeltaQuat);

            Alpha += 0.1f;
        });

    GetWorldTimerManager().SetTimer(SmoothRotationTimer, TimerDelegate, TimerRate / 10.f, true);
}

void ATurret::Fire_OnServer_Implementation(bool bIsPressed)
{
    bIsPressed ?                                                                              //
        GetWorldTimerManager().SetTimer(FireTimer, this, &ATurret::MakeShot, FireRate, true)  //
        :                                                                                     //
        GetWorldTimerManager().ClearTimer(FireTimer);
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

    auto Rotation = TurretMuzzle->GetSocketRotation(MuzzleSocketName);

    auto MuzzleLocation = TurretMuzzle->GetSocketLocation(MuzzleSocketName);
    FTransform SpawningTransform(Rotation, MuzzleLocation);

    auto Bullet = GetWorld()->SpawnActor<ADefaultProjectile>(DefaultProjectileClass, SpawningTransform);
    if (Bullet)
    {
        Bullet->SetLifeSpan(1.f);
    }
}

void ATurret::RotateTurret_OnServer_Implementation(const FQuat& Value)
{
    RotateTurret_Multicast(Value);
}

void ATurret::RotateTurret_Multicast_Implementation(const FQuat& Value)
{
    if (Controller && !Controller->IsLocalPlayerController())
    {
        float YawValue = Value.Rotator().Yaw;
        TurretTower->SetWorldRotation(FRotator(0.f, YawValue, 0.f));
        TurretMuzzle->SetWorldRotation(Value);
    }
}
