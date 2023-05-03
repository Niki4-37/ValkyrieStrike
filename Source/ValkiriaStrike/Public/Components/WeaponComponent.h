// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCoreTypes.h"
#include "WeaponComponent.generated.h"

class ATurret;
class ASecondWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKIRIASTRIKE_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    FOnItemMountSignature OnItemMount;
    FOnChangeAmmoSignature OnChangeAmmo;
    FOnOnStartReloadingSignature OnStartReloading;

    UFUNCTION(Server, reliable)
    void ShootFromSecondWeapon_OnServer();
    bool AddAmmo(int32 Amount);

protected:
    /* used in game level for debug */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ATurret> TurretClass;
    /* used in game level for debug */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ASecondWeapon> SecondWeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName TurretSocketName{"TurretSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SecondWeaponSocketName{"SecondWeaponSocket"};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(Replicated)
    ATurret* VehicleTurret{nullptr};

    UPROPERTY(Replicated)
    ASecondWeapon* SecondWeapon{nullptr};

    void OnChangeAmmoInWeapon(EVehicleItemType Type, int32 AmmoAmount);
    void OnStartWeaponReloading(EVehicleItemType Type);

    UFUNCTION(Server, reliable)
    void InitWeapons_OnServer();

    UFUNCTION(Client, reliable)
    void OnItemMount_Client(const FVehicleItemData& Data);

    UFUNCTION(Client, reliable)
    void OnChangeAmmo_Client(EVehicleItemType Type, int32 Amount);

    UFUNCTION(Client, reliable)
    void OnStartReloading_Client(EVehicleItemType Type);

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
