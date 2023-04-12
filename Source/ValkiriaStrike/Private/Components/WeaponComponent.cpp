// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WeaponComponent.h"
#include "Weapon/Turret.h"
#include "Weapon/SecondWeapon.h"
#include "WheeledVehicle.h"
#include "Net/UnrealNetwork.h"
#include "GameLevelsConfig/ValkiriaPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponent_LOG, All, All)

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UWeaponComponent::ShootFromSecondWeapon()
{
    if (!SecondWeapon) return;
    SecondWeapon->MakeShot_OnServer();
    UE_LOG(WeaponComponent_LOG, Display, TEXT("Fire from alternative weapon!!!"));
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    InitWeapons_OnServer();
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UWeaponComponent, VehicleTurret);
    DOREPLIFETIME(UWeaponComponent, SecondWeapon);
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::InitWeapons_OnServer_Implementation()
{
    if (!GetOwner() || !GetOwner()->GetInstigatorController()) return;
    const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkiriaPlayerState>();
    if (!PlayerState) return;

    //if (PlayerState->GetTurretClass())
    //{
    //    VehicleTurret = MountWeapon<ATurret>(PlayerState->GetTurretClass(), TurretSocketName);
    //}
    //if (PlayerState->GetSecondWeaponClass())
    //{
    //    SecondWeapon = MountWeapon<ASecondWeapon>(PlayerState->GetSecondWeaponClass(), SecondWeaponSocketName);
    //}

    /* used in game level for debug */
    if (TurretClass)
    {
        VehicleTurret = MountWeapon<ATurret>(TurretClass, TurretSocketName);
    }
    if  (SecondWeaponClass)
    {
        SecondWeapon = MountWeapon<ASecondWeapon>(SecondWeaponClass, SecondWeaponSocketName);
    }
    /* end debug */

    if (VehicleTurret && !VehicleTurret->Controller)
    {
        VehicleTurret->SpawnDefaultController();
    }
}
