// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"
#include "Weapon/TurretHubPawn.h"
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
    for (const auto VehicleWeapon : VehicleWeapons)
    {
        VehicleWeapon->AlternativeShot();
    }
}

bool UWeaponComponent::AddAmmo(int32 Amount)
{
    /** handled on server. Pickup */
    for (const auto VehicleWeapon : VehicleWeapons)
    {
        if (VehicleWeapon->AddAmmo(Amount, EVehicleUnitType::SecondWeapon)) return true;
    }

    return false;
}

void UWeaponComponent::InitWeapons()
{
    /** spawn turret hub */
    checkf(TurretHubClass, TEXT("TurretHubPawnHubClass not define!"));
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = GetOwner();
    TurretHub = GetWorld()->SpawnActor<ATurretHubPawn>(TurretHubClass, SpawnParams);

    if (TurretHub)
    {
        TurretHub->AttachToActor(GetOwner(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
        TurretHub->SetWeaponComponent(this);
    }

    if (TurretHub && !TurretHub->Controller)
    {
        TurretHub->SpawnDefaultController();
    }

    /** spawn weapons */
    if (!CompToAttachWeapons || !GetOwner() || !GetOwner()->GetInstigatorController()) return;
    const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkyriePlayerState>();

    if (!PlayerState || PlayerState->GetVehicleUnits().Num() == 0) return;

    for (auto& Unit : PlayerState->GetVehicleUnits())
    {
        if (Unit.UnitType == EVehicleUnitType::Turret || Unit.UnitType == EVehicleUnitType::SecondWeapon)
        {
            if (!GetWorld() || !Unit.UnitComponents.Num() || !Unit.UnitSpawnClass) continue;

            auto VehicleWeapon = GetWorld()->SpawnActor<ABaseVehicleWeapon>(Unit.UnitSpawnClass);  // how to undo unittyte checking?

            if (!VehicleWeapon) continue;

            FName SocketName{NAME_None};
            for (const auto& UnitComponent : Unit.UnitComponents)
            {
                VehicleWeapon->SetStaticMesh(UnitComponent.ComponentType, UnitComponent.UnitComponentMesh, UnitComponent.SocketName);

                if (UnitComponent.ComponentType != EUnitComponentType::Platform) continue;
                SocketName = UnitComponent.SocketName;
            }

            VehicleWeapon->SetOwner(GetOwner());

            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
            VehicleWeapon->AttachToComponent(CompToAttachWeapons, AttachmentRules, SocketName);

            /* Side mode checking */
            const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetOwner()->GetActorUpVector(), VehicleWeapon->GetActorUpVector()));
            VehicleWeapon->DefineSideMode(AngleBetween);

            /* Side checking. Minus for the left */
            const auto SocketVector = (CompToAttachWeapons->GetSocketLocation(SocketName) - GetOwner()->GetActorLocation()).GetSafeNormal();
            const auto DotProduct = FVector::DotProduct(SocketVector, GetOwner()->GetActorRightVector());
            VehicleWeapon->DefineSidePosition(DotProduct);

            VehicleWeapon->SetupWeapon(Unit.UnitType, Unit.UnitMaxValue, Unit.ExecutionTime);

            VehicleWeapons.Add(VehicleWeapon);
        }
    }
}

void UWeaponComponent::UpdateWidgets()
{
    if (!GetOwner() || !GetOwner()->GetInstigatorController()) return;
    const auto PlayerState = GetOwner()->GetInstigatorController()->GetPlayerState<AValkyriePlayerState>();

    if (!PlayerState || PlayerState->GetVehicleUnits().Num() == 0) return;

    for (auto& Unit : PlayerState->GetVehicleUnits())
    {
        switch (Unit.UnitType)
        {
            case EVehicleUnitType::Turret: OnUnitMount_Client(Unit); break;
            case EVehicleUnitType::SecondWeapon: OnUnitMount_Client(Unit); break;
            default: break;
        }
    }
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (const auto HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>())
    {
        HealthComponent->OnDeath.AddUObject(this, &UWeaponComponent::OnDeath);
    }
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UWeaponComponent, CompToAttachWeapons);
    DOREPLIFETIME(UWeaponComponent, VehicleWeapons);
}

void UWeaponComponent::OnDeath()
{
    if (TurretHub && TurretHub->Controller)
    {
        TurretHub->Controller->Destroy();
    }
}

void UWeaponComponent::OnUnitMount_Client_Implementation(const FVehicleUnitData& Data)
{
    OnUnitMount.Broadcast(Data);
}

void UWeaponComponent::OnChangeAmmo_Client_Implementation(EVehicleUnitType Type, int32 Amount)
{
    OnChangeAmmo.Broadcast(Type, Amount);
}

void UWeaponComponent::OnStartReloading_Client_Implementation(EVehicleUnitType Type)
{
    OnStartReloading.Broadcast(Type);
}