// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Interfaces/GameInterface.h"
#include "GameCoreTypes.h"
#include "DefaultWeeledVehicle.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ATurret;
class UWeaponComponent;
class UVehicleIndicatorsComponent;

UCLASS()
class VALKIRIASTRIKE_API ADefaultWeeledVehicle : public AWheeledVehicle, public IGameInterface
{
    GENERATED_BODY()

public:
    ADefaultWeeledVehicle();

    /** Interface function */
    virtual bool AddAmount(const FInteractionData& Data) override;

protected:
    UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComp;

    UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UVehicleIndicatorsComponent* VehicleIndicatorsComp;

    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
    bool bInReverseGear;

public:
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float Delta) override;
    virtual void BeginPlay() override;

private:
    /* Are we on a 'slippery' surface */
    bool bIsLowFriction;

    bool bIsAutoMoveForward{true};

    /** Handle pressing forwards */
    void MoveForward(float Val);
    /** Handle pressing right */
    void MoveRight(float Val);

    /** Handle handbrake pressed */
    void OnHandbrakePressed(bool bIsUsed);

    void OnFireEvent(bool bIsEnabled);
};
