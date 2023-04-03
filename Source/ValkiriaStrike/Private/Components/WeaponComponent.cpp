// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WeaponComponent.h"
#include "Weapon/Turret.h"
#include "WheeledVehicle.h"
#include "Net/UnrealNetwork.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    //InitWeapons_OnServer();
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UWeaponComponent, VehicleTurret);
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::InitWeapons_OnServer_Implementation()
{
    if (VehicleTurret) return;

    const auto WeeledVehicle = Cast<AWheeledVehicle>(GetOwner());
    if (!WeeledVehicle) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = GetOwner();
    VehicleTurret = GetWorld()->SpawnActor<ATurret>(TurretClass, SpawnParams);

    if (!VehicleTurret) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    VehicleTurret->AttachToComponent(WeeledVehicle->GetMesh(), AttachmentRules, TurretSocketName);

    if (!VehicleTurret->Controller)
    {
        VehicleTurret->SpawnDefaultController();
    }
}
