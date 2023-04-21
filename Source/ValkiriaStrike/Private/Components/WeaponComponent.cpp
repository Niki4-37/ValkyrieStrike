// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WeaponComponent.h"
#include "Weapon/Turret.h"
#include "Weapon/SecondWeapon.h"
#include "WheeledVehicle.h"
#include "Net/UnrealNetwork.h"
#include "GameLevelsConfig/ValkiriaPlayerState.h"

#include "Engine.h"

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

    OnWeaponMakeShot.Broadcast(EVehicleItemType::SecondWeapon, SecondWeapon->GetAmmoCapacity());
}

bool UWeaponComponent::AddAmmo(int32 Amount)
{
    /** handled on server. Pickup->DefaultWeeledVehicle */
    bool bCanAdd {false};
    if (SecondWeapon)
    {
        bCanAdd = SecondWeapon->ChangeAmmoCapacity(Amount);
        OnWeaponMakeShot.Broadcast(EVehicleItemType::SecondWeapon, SecondWeapon->GetAmmoCapacity());
    }
    return bCanAdd;
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

    for (auto& VehicleItem : PlayerState->GetVehicleItems())
    {
        if (VehicleItem.ItemType == EVehicleItemType::Turret && VehicleItem.ItemClass)
        {
            VehicleTurret = MountWeapon<ATurret>(VehicleItem.ItemClass, TurretSocketName);
        }
        if (VehicleItem.ItemType == EVehicleItemType::SecondWeapon && VehicleItem.ItemClass)
        {
            SecondWeapon = MountWeapon<ASecondWeapon>(VehicleItem.ItemClass, SecondWeaponSocketName);
            SecondWeapon->SetWeaponData(VehicleItem);
            SecondWeapon->ChangeAmmoCapacity(VehicleItem.MaxAmmoCapacity);
        }
        OnItemMount_Client(VehicleItem);
    }

    /* used in game level for debug */
    // if (TurretClass)
    //{
    //     VehicleTurret = MountWeapon<ATurret>(TurretClass, TurretSocketName);
    // }
    // if (SecondWeaponClass)
    //{
    //    SecondWeapon = MountWeapon<ASecondWeapon>(SecondWeaponClass, SecondWeaponSocketName);
    //}
    /* end debug */

    if (VehicleTurret && !VehicleTurret->Controller)
    {
        VehicleTurret->SpawnDefaultController();
    }
}

void UWeaponComponent::OnItemMount_Client_Implementation(const FVehicleItemData& Data)
{
    OnItemMount.Broadcast(Data);

    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "UWeaponComponent: OnItemMount");
}
