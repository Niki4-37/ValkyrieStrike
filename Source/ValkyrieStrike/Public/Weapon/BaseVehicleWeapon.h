// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "BaseVehicleWeapon.generated.h"

class ADefaultProjectile;

UCLASS()
class VALKYRIESTRIKE_API ABaseVehicleWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseVehicleWeapon();

    FOnChangeAmmoSignature OnChangeAmmoInWeapon;
    FOnOnStartReloadingSignature OnStartWeaponReloading;

    void SetStaticMesh(EUnitComponentType Type, UStaticMesh* NewMesh, FName SocketName);

    void DefineSidePosition(float DotProduct);
    void DefineSideMode(float InAcos);

    void SetupWeapon(EVehicleUnitType InWeaponType, int32 InMaxAmmoCapacity, float InReloadingTime);
    EVehicleUnitType GetWeaponType() const { return WeaponType; }  //???
    void UpdateAimActor(AActor* NewAimActor, float UpdateTimeRate);

    bool HasAim() const { return bHasAim; }
    bool IsReloading() const { return bIsReloading; }

    bool IsEmpty() const { return AmmoCapacity == 0; }
    bool AddAmmo(int32 Amount, EVehicleUnitType InType);

    virtual void AlternativeShot();

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WeaponRootComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* Platform;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* Gun;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    FName MuzzleSocketName{"MuzzleSocket"};

    bool bIsSideMode{false};
    float SidePositionModifier{1.f};

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void RotateToTarget(AActor* Target);

    virtual void MakeShot();

    void ChangeAmmo(int32 Value);
    void Recharge();
    void ReloadWeapon();
    int32 GetAmmo() const { return AmmoCapacity; };

public:
    virtual void Tick(float DeltaTime) override;

private:
    FTimerHandle RotationTimer;

    EVehicleUnitType WeaponType;
    int32 MaxAmmoCapacity{0};
    int32 AmmoCapacity{0};

    FTimerHandle ReloadingTimer;
    float ReloadingTime{0.f};
    bool bIsReloading{false};

    bool bHasAim{false};

    UFUNCTION(Client, unreliable)
    void OnChangeAmmoInWeapon_OnClient(EVehicleUnitType Type, int32 Value);

    UFUNCTION(Client, unreliable)
    void OnStartWeaponReloading_OnClient(EVehicleUnitType Type);
};
