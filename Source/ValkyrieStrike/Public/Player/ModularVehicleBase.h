// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/GameInterface.h"
#include "GameCoreTypes.h"
#include "ModularVehicleBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UWheelManagerComponent;
class UWeaponComponent;
class UVehicleIndicatorsComponent;
class UWorkshopComponent;

UCLASS()
class VALKYRIESTRIKE_API AModularVehicleBase : public APawn
{
    GENERATED_BODY()

public:
    AModularVehicleBase();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMeshComponent* Chassis;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* VehicleBody;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USpringArmComponent* SpringArm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UWheelManagerComponent* WheelManagerComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UVehicleIndicatorsComponent* VehicleIndicatorsComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UWorkshopComponent* WorkshopComponent;

    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WheelFR;
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WheelFL;
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WheelRR;
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WheelRL;

    virtual void BeginPlay() override;

public:
    virtual void PostInitializeComponents() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Restart() override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /* used in AnimationBP */
    UFUNCTION(BlueprintCallable)
    bool GetVelocityAverage(FVector& VelocityAverage) const;
    UFUNCTION(BlueprintCallable)
    float GetSmoothTurnValue() const { return SmoothTurnValue; }
    UFUNCTION(BlueprintCallable)
    bool GetWheelsGroups(TArray<FWheelsGroup>& WheelsGroups) const;

private:
    float SmoothTurnValue;
    float PawnDeltaTime;

    float MoveForvardAxis;
    float MoveSideAxis;

    void MoveForward(float Amount);
    void MoveRight(float Value);

    void WheelsTurn();

    /* replicates movement */
    FTimerHandle DataTickTimer;
    UFUNCTION(NetMulticast, unreliable)
    void SendDataTick_Multicast();
    UFUNCTION(Server, unreliable)
    void SendMoveControls_Server(float InMoveForvardAxis, float InMoveSideAxis, float InSmoothTurnValue);
    UFUNCTION(NetMulticast, unreliable)
    void SendMoveControls_Multicast(float InMoveForvardAxis, float InMoveSideAxis, float InSmoothTurnValue);
};
