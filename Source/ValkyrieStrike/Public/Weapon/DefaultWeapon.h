// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultWeapon.generated.h"

class USkeletalMeshComponent;
class ADefaultProjectile;

UCLASS()
class VALKYRIESTRIKE_API ADefaultWeapon : public AActor
{
    GENERATED_BODY()

public:
    ADefaultWeapon();

    void StartFire(bool bIsPressed, const FVector& AimPosition);
    void SetFireRate(float NewFireRate);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ADefaultProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName{"MuzzleSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float FireRate{0.5f};

    virtual void BeginPlay() override;

private:
    FTimerHandle FiringTimer;

    UFUNCTION()
    void MakeShot(const FVector& AimPosition);
};
