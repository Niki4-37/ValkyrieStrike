// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ATurret;
class ASecondVehicleWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKIRIASTRIKE_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    void ShootFromSecondWeapon();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ATurret> TurretClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ASecondVehicleWeapon> SecondWeaponClass;

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
    ASecondVehicleWeapon* SecondWeapon{nullptr};

    UFUNCTION(Server, reliable)
    void InitWeapons_OnServer();

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
