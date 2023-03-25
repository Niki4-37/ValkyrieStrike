// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "DefaultWeeledVehicle.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ATurret;

UCLASS()
class VALKIRIASTRIKE_API ADefaultWeeledVehicle : public AWheeledVehicle
{
    GENERATED_BODY()

public:
    ADefaultWeeledVehicle();

protected:
    /** Spring arm that will offset the camera */
    UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComp;

    /** Camera component that will be our viewpoint */
    UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ATurret> TurretClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName TurretSocketName{"TurretSocket"};

    /** Are we in reverse gear */
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

    UPROPERTY()
    ATurret* VehicleTurret{nullptr};

    /** Handle pressing forwards */
    void MoveForward(float Val);
    /** Handle pressing right */
    void MoveRight(float Val);

    /** Handle handbrake pressed */
    void OnHandbrakePressed(bool bIsUsed);

    void OnFireEvent(bool bIsEnabled);
};
