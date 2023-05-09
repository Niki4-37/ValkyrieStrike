// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "SecondWeapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class VALKYRIESTRIKE_API ASecondWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASecondWeapon();

    FOnChangeAmmoSignature OnChangeAmmoInWeapon;
    FOnOnStartReloadingSignature OnStartWeaponReloading;

    bool MakeShot();

    bool ChangeAmmoCapacity(int32 Value);

    void SetupWeapon(int32 InMaxAmmoCapacity, float InReloadingTime);

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WeaponRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BeemRadius = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WeaponDamage = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName{"MuzzleSocket"};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    FTimerHandle ReloadingTimer;

    UPROPERTY(Replicated)
    FVehicleItemData WeaponData;

    UPROPERTY(Replicated)
    bool bIsReady{true};

    UPROPERTY(Replicated)
    int32 AmmoCapacity;

    UPROPERTY(Replicated)
    int32 MaxAmmoCapacity;
    float ReloadingTime;

    bool IsEmpty() const { return AmmoCapacity == 0; };
    void ReloadWeapon();
};
