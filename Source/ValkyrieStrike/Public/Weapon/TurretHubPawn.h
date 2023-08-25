// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameCoreTypes.h"
#include "TurretHubPawn.generated.h"

class UStaticMeshComponent;
class ADefaultProjectile;
class UBehaviorTree;
class ABaseVehicleWeapon;

class UWeaponComponent;

UCLASS()
class VALKYRIESTRIKE_API ATurretHubPawn : public APawn
{
    GENERATED_BODY()

public:
    ATurretHubPawn();

    void UpdateAimActor(AActor* AimActor, float TimerRate);
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
    void SetWeaponComponent(UWeaponComponent* NewWeaponComponent) { WeaponComp = NewWeaponComponent; }  // delete

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* PawnRootComponent;

    virtual void BeginPlay() override;

private:
    UPROPERTY(Replicated)
    UWeaponComponent* WeaponComp{nullptr};
};
