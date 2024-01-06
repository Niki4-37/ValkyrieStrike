// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultWeapon.generated.h"

class USkeletalMeshComponent;
class ADefaultProjectile;
class USoundCue;

UCLASS()
class VALKYRIESTRIKE_API ADefaultWeapon : public AActor
{
    GENERATED_BODY()

public:
    ADefaultWeapon();

    void StartFire(bool bIsPressed, const FVector& AimPosition);
    void SetFireRate(float NewFireRate);

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    USceneComponent* WeaponRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* WeaponStaticMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ADefaultProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName{"MuzzleSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float FireRate{0.5f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;

    virtual void BeginPlay() override;

private:
    FTimerHandle FireRateTimer;
    bool bWeaponIsReady{true};

    UFUNCTION()
    void MakeShot(const FVector& AimPosition);

    UFUNCTION(NetMulticast, unreliable)
    void SpawnSound_Multicast(USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None);
};
