// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCoreTypes.h"
#include "WeaponComponent.generated.h"

class ATurretHubPawn;
class ABaseVehicleWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKYRIESTRIKE_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    FOnUnitMountSignature OnUnitMount;
    FOnChangeAmmoSignature OnChangeAmmo;
    FOnOnStartReloadingSignature OnStartReloading;

    UFUNCTION(Server, reliable)
    void ShootFromSecondWeapon_OnServer();
    bool AddAmmo(int32 Amount);

    void InitWeapons();
    void CleanWeapons();
    void UpdateWidgets();

    void SetCompToAttachWeapons(USceneComponent* NewComp) { CompToAttachWeapons = NewComp; };

    TArray<ABaseVehicleWeapon*> GetVehicleWeapons() const { return VehicleWeapons; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ATurretHubPawn> TurretHubClass;

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)
    USceneComponent* CompToAttachWeapons{nullptr};

    UPROPERTY(Replicated)
    TArray<ABaseVehicleWeapon*> VehicleWeapons;

    UPROPERTY()
    ATurretHubPawn* TurretHub;

    void OnDeath();

    UFUNCTION(Client, reliable)
    void OnUnitMount_Client(const FVehicleUnitData& Data);

    UFUNCTION(Client, reliable)
    void OnChangeAmmo_Client(EVehicleUnitType Type, int32 Amount);

    UFUNCTION(Client, reliable)
    void OnStartReloading_Client(EVehicleUnitType Type);

    template <class T>
    T* MountWeapon(UClass* Class, FName SocketName);
};

template <class T>
inline T* UWeaponComponent::MountWeapon(UClass* Class, FName SocketName)
{
    const auto WeeledVehicle = Cast<AWheeledVehicle>(GetOwner());
    if (!WeeledVehicle) return nullptr;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = GetOwner();
    auto Weapon = GetWorld()->SpawnActor<T>(Class, SpawnParams);

    if (!Weapon) return nullptr;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(WeeledVehicle->GetMesh(), AttachmentRules, SocketName);
    return Weapon;
}
