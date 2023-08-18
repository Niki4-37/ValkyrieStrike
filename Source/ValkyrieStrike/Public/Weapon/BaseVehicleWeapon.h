// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "BaseVehicleWeapon.generated.h"

UCLASS()
class VALKYRIESTRIKE_API ABaseVehicleWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseVehicleWeapon();

    void SetStaticMesh(EUnitComponentType Type, UStaticMesh* NewMesh, FName SocketName);

    void DefineSidePosition(float DotProduct);
    void DefineSideMode(float InAcos);

    void SetWeaponMovable(bool bEnable);
    void SetGunMovable(bool bEnable) { bGunCanMove = bEnable; }

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* Platform;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* Gun;

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    bool bPlatformCanMove{true};
    bool bGunCanMove{true};
    UPROPERTY(Replicated)
    bool bIsSideMode{false};
    UPROPERTY(Replicated)
    float SidePositionModifier{1.f};

    FTimerHandle RotationTimer;

    UFUNCTION(NetMulticast, unreliable)
    void RotateToTarget();
};
