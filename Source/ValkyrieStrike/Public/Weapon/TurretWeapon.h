// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseVehicleWeapon.h"
#include "TurretWeapon.generated.h"

UCLASS()
class VALKYRIESTRIKE_API ATurretWeapon : public ABaseVehicleWeapon
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ADefaultProjectile> DefaultProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRate{0.5f};

    virtual void BeginPlay() override;
    virtual void RotateToTarget(AActor* Target) override;
    virtual void MakeShot() override;

private:
    FTimerHandle FireTimer;  // for turret
};
