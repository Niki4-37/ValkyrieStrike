// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"
#include "Weapon/BaseVehicleWeapon.h"
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
    // if (!SecondWeapon) return;
    // SecondWeapon->MakeShot();
}

bool UWeaponComponent::AddAmmo(int32 Amount)
{
    /** handled on server. Pickup->DefaultWeeledVehicle */
    // return SecondWeapon ? SecondWeapon->ChangeAmmoCapacity(Amount) : false;

    return false;
}

void UWeaponComponent::InitWeapons_OnServer_Implementation()
{
    if (!CompToAttachWeapons || !GetOwner() || !GetOwner()->GetInstigatorController()) return;
    const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkyriePlayerState>();

    if (!PlayerState || PlayerState->GetVehicleUnits().Num() == 0) return;

    for (auto& Unit : PlayerState->GetVehicleUnits())
    {
        if (Unit.UnitType == EVehicleUnitType::Turret || Unit.UnitType == EVehicleUnitType::SecondWeapon)
        {
            if (!GetWorld() || !Unit.UnitComponents.Num() || !Unit.UnitSpawnClass) continue;

            auto VehicleItem = GetWorld()->SpawnActor<ABaseVehicleWeapon>(Unit.UnitSpawnClass);  // use to undo unittyte checking

            if (!VehicleItem) continue;

            FName SocketName{NAME_None};
            for (const auto& UnitComponent : Unit.UnitComponents)
            {
                VehicleItem->SetStaticMesh(UnitComponent.ComponentType, UnitComponent.UnitComponentMesh, UnitComponent.SocketName);

                if (UnitComponent.ComponentType != EUnitComponentType::Platform) continue;
                SocketName = UnitComponent.SocketName;
            }

            // GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, CompToAttachWeapons->GetComponentLocation().ToString());

            /* Side checking. Minus for the left */
            const auto SocketVector = (CompToAttachWeapons->GetSocketLocation(SocketName) - GetOwner()->GetActorLocation()).GetSafeNormal();
            const auto DotProduct = FVector::DotProduct(SocketVector, GetOwner()->GetActorRightVector());
            VehicleItem->DefineSidePosition(DotProduct);

            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            VehicleItem->AttachToComponent(CompToAttachWeapons, AttachmentRules, SocketName);

            const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetOwner()->GetActorUpVector(), VehicleItem->GetActorUpVector()));
            VehicleItem->DefineSideMode(AngleBetween);

            VehicleItem->SetWeaponMovable(true);

            VehicleWeapons.Add(VehicleItem);
        }
    }

    // if (VehicleTurret && !VehicleTurret->Controller)
    //{
    //     VehicleTurret->SpawnDefaultController();
    // }
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
    // if (GetOwner()->GetRemoteRole() == ENetRole::ROLE_Authority)
    //{
    //     InitWeapons_OnServer();
    // }
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UWeaponComponent, CompToAttachWeapons);
    DOREPLIFETIME(UWeaponComponent, VehicleWeapons);
}

void UWeaponComponent::OnDeath()
{
    // if (VehicleTurret && VehicleTurret->Controller)
    //{
    //     VehicleTurret->Controller->Destroy();
    // }
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