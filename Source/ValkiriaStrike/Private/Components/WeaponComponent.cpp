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

void UWeaponComponent::ShootFromSecondWeapon_OnServer_Implementation()
{
    if (!SecondWeapon) return;
    if (SecondWeapon->MakeShot())
    {
        OnChangeAmmo_Client(EVehicleItemType::SecondWeapon, SecondWeapon->GetAmmoCapacity());
        if (!SecondWeapon->IsEmpty())
        {
            OnStartReloading_Client(EVehicleItemType::SecondWeapon);
        }
    }
}

bool UWeaponComponent::AddAmmo(int32 Amount)
{
    /** handled on server. Pickup->DefaultWeeledVehicle */
    bool bCanAdd{false};
    if (SecondWeapon)
    {
        if (SecondWeapon->IsEmpty() && SecondWeapon->ChangeAmmoCapacity(Amount))
        {
            SecondWeapon->ReloadWeapon();
            OnStartReloading_Client(EVehicleItemType::SecondWeapon);
            bCanAdd = true;
        }
        else
        {
            bCanAdd = SecondWeapon->ChangeAmmoCapacity(Amount);
        }
        OnChangeAmmo_Client(EVehicleItemType::SecondWeapon, SecondWeapon->GetAmmoCapacity());
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
    // if (!GetOwner() || !GetOwner()->GetInstigatorController()) return;
    // const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkiriaPlayerState>();
    // if (!PlayerState) return;

    // for (auto& VehicleItem : PlayerState->GetVehicleItems())
    //{
    //     if (VehicleItem.ItemType == EVehicleItemType::Turret && VehicleItem.ItemClass)
    //     {
    //         VehicleTurret = MountWeapon<ATurret>(VehicleItem.ItemClass, TurretSocketName);
    //     }
    //     if (VehicleItem.ItemType == EVehicleItemType::SecondWeapon && VehicleItem.ItemClass)
    //     {
    //         SecondWeapon = MountWeapon<ASecondWeapon>(VehicleItem.ItemClass, SecondWeaponSocketName);
    //         SecondWeapon->SetWeaponData(VehicleItem);
    //         SecondWeapon->ChangeAmmoCapacity(VehicleItem.MaxAmmoCapacity);
    //     }
    //     OnItemMount_Client(VehicleItem);
    // }

    /* used in game level for debug */
    if (TurretClass)
    {
        VehicleTurret = MountWeapon<ATurret>(TurretClass, TurretSocketName);
    }
    if (SecondWeaponClass)
    {
        SecondWeapon = MountWeapon<ASecondWeapon>(SecondWeaponClass, SecondWeaponSocketName);
    }
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

void UWeaponComponent::OnChangeAmmo_Client_Implementation(EVehicleItemType Type, int32 Amount)
{
    OnChangeAmmo.Broadcast(Type, Amount);
}

void UWeaponComponent::OnStartReloading_Client_Implementation(EVehicleItemType Type)
{
    OnStartReloading.Broadcast(Type);
}