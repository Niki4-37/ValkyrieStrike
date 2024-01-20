// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Weapon/TurretHubPawn.h"
// #include "Weapon/DefaultProjectile.h"
#include "Components/WeaponComponent.h"
#include "Weapon/BaseVehicleWeapon.h"
#include "Net/UnrealNetwork.h"
#include "AI/AITurretController.h"

ATurretHubPawn::ATurretHubPawn()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = false;

    PawnRootComponent = CreateDefaultSubobject<USceneComponent>("PawnRootComponent");
    SetRootComponent(PawnRootComponent);

    // AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAITurretController::StaticClass();
}

void ATurretHubPawn::UpdateAimActor(AActor* AimActor, float TimerRate)
{
    if (!WeaponComp) return;
    for (const auto VehicleWeapon : WeaponComp->GetVehicleWeapons())
    {
        if (!VehicleWeapon) continue;
        VehicleWeapon->UpdateAimActor(AimActor, TimerRate);
    }
}

void ATurretHubPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ATurretHubPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATurretHubPawn, WeaponComp);
}