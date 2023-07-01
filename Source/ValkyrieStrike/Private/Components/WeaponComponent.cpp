// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"
#include "Weapon/Turret.h"
#include "Weapon/SecondWeapon.h"
#include "WheeledVehicle.h"
#include "Net/UnrealNetwork.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"

#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponent_LOG, All, All)

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UWeaponComponent::ShootFromSecondWeapon_OnServer_Implementation()
{
    if (!SecondWeapon) return;
    SecondWeapon->MakeShot();
}

bool UWeaponComponent::AddAmmo(int32 Amount)
{
    /** handled on server. Pickup->DefaultWeeledVehicle */
    return SecondWeapon ? SecondWeapon->ChangeAmmoCapacity(Amount) : false;
}

void UWeaponComponent::InitWeapons_OnServer_Implementation()
{
    // if (!GetOwner() || !GetOwner()->GetInstigatorController()) return;
    // const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkyriePlayerState>();

    // if (!PlayerState || PlayerState->GetVehicleItems().Num() == 0) return;

    // for (auto& VehicleItem : PlayerState->GetVehicleItems())
    //{
    //     if (!VehicleItem.ItemClass) continue;

    //    FString EnumNameString{UEnum::GetValueAsName(VehicleItem.ItemType).ToString()};
    //    int32 ScopeIndex = EnumNameString.Find(TEXT("::"), ESearchCase::CaseSensitive);
    //    FName SocketName{NAME_None};
    //    if (ScopeIndex != INDEX_NONE)
    //    {
    //        SocketName = FName(*(EnumNameString.Mid(ScopeIndex + 2) + "Socket"));
    //    }

    //    if (VehicleItem.ItemType == EVehicleItemType::Turret)
    //    {
    //        VehicleTurret = MountWeapon<ATurret>(VehicleItem.ItemClass, SocketName /*TurretSocketName*/);
    //        if (!VehicleTurret) continue;

    //        VehicleTurret->SetupWeapon(VehicleItem.MaxAmmoCapacity, VehicleItem.ReloadingTime);
    //        VehicleTurret->OnChangeAmmoInWeapon.AddUObject(this, &UWeaponComponent::OnChangeAmmo_Client);
    //        VehicleTurret->OnStartWeaponReloading.AddUObject(this, &UWeaponComponent::OnStartReloading_Client);
    //    }
    //    if (VehicleItem.ItemType == EVehicleItemType::SecondWeapon)
    //    {
    //        SecondWeapon = MountWeapon<ASecondWeapon>(VehicleItem.ItemClass, SocketName /*SecondWeaponSocketName*/);
    //        if (!SecondWeapon) continue;

    //        SecondWeapon->SetupWeapon(VehicleItem.MaxAmmoCapacity, VehicleItem.ReloadingTime);
    //        SecondWeapon->OnChangeAmmoInWeapon.AddUObject(this, &UWeaponComponent::OnChangeAmmo_Client);
    //        SecondWeapon->OnStartWeaponReloading.AddUObject(this, &UWeaponComponent::OnStartReloading_Client);
    //    }
    //}

    /* used in game level for debug */
    if (TurretData.ItemClass)
    {
        VehicleTurret = MountWeapon<ATurret>(TurretData.ItemClass, TurretSocketName);
        if (VehicleTurret)
        {
            VehicleTurret->SetupWeapon(TurretData.MaxAmmoCapacity, TurretData.ReloadingTime);
            VehicleTurret->OnChangeAmmoInWeapon.AddUObject(this, &UWeaponComponent::OnChangeAmmo_Client);
            VehicleTurret->OnStartWeaponReloading.AddUObject(this, &UWeaponComponent::OnStartReloading_Client);
            OnItemMount_Client(TurretData);
        }
    }
    if (SecondWeaponData.ItemClass)
    {
        SecondWeapon = MountWeapon<ASecondWeapon>(SecondWeaponData.ItemClass, SecondWeaponSocketName);
        if (SecondWeapon)
        {
            SecondWeapon->SetupWeapon(SecondWeaponData.MaxAmmoCapacity, SecondWeaponData.ReloadingTime);
            SecondWeapon->OnChangeAmmoInWeapon.AddUObject(this, &UWeaponComponent::OnChangeAmmo_Client);
            SecondWeapon->OnStartWeaponReloading.AddUObject(this, &UWeaponComponent::OnStartReloading_Client);
            OnItemMount_Client(SecondWeaponData);
        }
    }
    /* end debug */

    if (VehicleTurret && !VehicleTurret->Controller)
    {
        VehicleTurret->SpawnDefaultController();
    }
}

void UWeaponComponent::UpdateWidgets()
{
    if (!GetOwner() || !GetOwner()->GetInstigatorController()) return;
    const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkyriePlayerState>();

    // if (!PlayerState || PlayerState->GetVehicleItems().Num() == 0) return;

    // for (auto& VehicleItem : PlayerState->GetVehicleItems())
    //{
    //     OnItemMount_Client(VehicleItem);
    // }
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (const auto HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>())
    {
        HealthComponent->OnDeath.AddUObject(this, &UWeaponComponent::OnDeath);
    }

    /* debug */
    if (GetOwner()->GetRemoteRole() == ENetRole::ROLE_Authority)
    {
        InitWeapons_OnServer();
    }
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    /* debug */
    DOREPLIFETIME(UWeaponComponent, TurretData);
    DOREPLIFETIME(UWeaponComponent, SecondWeaponData);

    DOREPLIFETIME(UWeaponComponent, VehicleTurret);
    DOREPLIFETIME(UWeaponComponent, SecondWeapon);
}

void UWeaponComponent::OnDeath()
{
    if (VehicleTurret && VehicleTurret->Controller)
    {
        VehicleTurret->Controller->Destroy();
    }
}

void UWeaponComponent::OnItemMount_Client_Implementation(const FVehicleItemData& Data)
{
    FString Test = "UWeaponComponent : OnItemMount " + Data.ItemName.ToString();
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Test);

    OnItemMount.Broadcast(Data);
}

void UWeaponComponent::OnChangeAmmo_Client_Implementation(EVehicleItemType Type, int32 Amount)
{
    OnChangeAmmo.Broadcast(Type, Amount);
}

void UWeaponComponent::OnStartReloading_Client_Implementation(EVehicleItemType Type)
{
    OnStartReloading.Broadcast(Type);
}