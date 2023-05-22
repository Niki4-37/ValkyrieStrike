// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameCoreTypes.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ADefaultProjectile;
class UBehaviorTree;

UCLASS()
class VALKYRIESTRIKE_API ATurret : public APawn
{
    GENERATED_BODY()

public:
    ATurret();

    FOnChangeAmmoSignature OnChangeAmmoInWeapon;
    FOnOnStartReloadingSignature OnStartWeaponReloading;

    UFUNCTION(Server, unreliable)
    void Fire_OnServer(bool bCanFire);

    UFUNCTION(Server, unreliable)
    void RotateToTarget_OnServer(AActor* AimActor, float TimerRate);

    void SetupWeapon(int32 InMaxAmmoCapacity, float InReloadingTime);

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* PawnRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* TurretTower;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* TurretMuzzle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName{"MuzzleSocket"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRate{0.5f};

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    int32 AmmoCapacity{100};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ADefaultProjectile> DefaultProjectileClass;

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    FTimerHandle FireTimer;
    FTimerHandle ReloadingTimer;

    FTimerHandle SmoothRotationTimer;

    float Alpha{0.f};

    bool bIsReady{true};

    int32 MaxAmmoCapacity;
    float ReloadingTime;

    void MakeShot();
    void ReloadWeapon();
    bool IsEmpty() const { return AmmoCapacity == 0; };

    UFUNCTION(NetMulticast, unreliable)
    void RotateTurret_Multicast(const FQuat& Value);
};
