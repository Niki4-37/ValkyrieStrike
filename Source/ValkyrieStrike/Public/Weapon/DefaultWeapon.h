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

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ADefaultProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName{"MuzzleSocket"};

    virtual void BeginPlay() override;

private:
    FTimerHandle FiringTimer;

    UFUNCTION()
    void MakeShot(const FVector& AimPosition);
};
