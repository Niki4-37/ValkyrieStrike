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
    SecondWeapon->MakeShot();
}

bool UWeaponComponent::AddAmmo(int32 Amount)
{
    /** handled on server. Pickup->DefaultWeeledVehicle */
    return SecondWeapon ? SecondWeapon->ChangeAmmoCapacity(Amount) : false;
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

void UWeaponComponent::OnChangeAmmoInWeapon(EVehicleItemType Type, int32 AmmoAmount)
{
    OnChangeAmmo_Client(Type, AmmoAmount);
}

void UWeaponComponent::OnStartWeaponReloading(EVehicleItemType Type)
{
    OnStartReloading_Client(Type);
}

void UWeaponComponent::InitWeapons_OnServer_Implementation()
{
    if (!GetOwner() || !GetOwner()->GetInstigatorController()) return;
    const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkiriaPlayerState>();
    if (!PlayerState || PlayerState->GetVehicleItems().Num() == 0) return;

    for (auto& VehicleItem : PlayerState->GetVehicleItems())
    {
        if (!VehicleItem.ItemClass) continue;

        FString EnumNameString{UEnum::GetValueAsName(VehicleItem.ItemType).ToString()};
        int32 ScopeIndex = EnumNameString.Find(TEXT("::"), ESearchCase::CaseSensitive);
        FName SocketName{NAME_None};
        if (ScopeIndex != INDEX_NONE)
        {
            SocketName = FName(*(EnumNameString.Mid(ScopeIndex + 2) + "Socket"));
        }

        if (VehicleItem.ItemType == EVehicleItemType::Turret)
        {
            VehicleTurret = MountWeapon<ATurret>(VehicleItem.ItemClass, SocketName /*TurretSocketName*/);
            if (!VehicleTurret) continue;

            VehicleTurret->SetupWeapon(VehicleItem.MaxAmmoCapacity, VehicleItem.ReloadingTime);
            VehicleTurret->OnChangeAmmoInWeapon.AddUObject(this, &UWeaponComponent::OnChangeAmmoInWeapon);
            VehicleTurret->OnStartWeaponReloading.AddUObject(this, &UWeaponComponent::OnStartWeaponReloading);
        }
        if (VehicleItem.ItemType == EVehicleItemType::SecondWeapon)
        {
            SecondWeapon = MountWeapon<ASecondWeapon>(VehicleItem.ItemClass, SocketName /*SecondWeaponSocketName*/);
            if (!SecondWeapon) continue;

            SecondWeapon->SetupWeapon(VehicleItem.MaxAmmoCapacity, VehicleItem.ReloadingTime);

            SecondWeapon->OnChangeAmmoInWeapon.AddUObject(this, &UWeaponComponent::OnChangeAmmoInWeapon);
            SecondWeapon->OnStartWeaponReloading.AddUObject(this, &UWeaponComponent::OnStartWeaponReloading);
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
    //     SecondWeapon = MountWeapon<ASecondWeapon>(SecondWeaponClass, SecondWeaponSocketName);
    // }
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