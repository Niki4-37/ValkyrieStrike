// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCoreTypes.h"
#include "WheelManagerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VALKYRIESTRIKE_API UWheelManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWheelManagerComponent();

    void SetWheelsGroups(TArray<FWheelsGroup>& InWheelGroups) { WheelsGroups = InWheelGroups; }
    void SetControlInput(float Value);
    float GetWheelDefultDistance() const { return WheelDefaultDistance; }

    /* used in Animation Blueprint */
    bool GetVelocityAverage(FVector& VelocityAverage) const;
    bool GetWheelsGroups(TArray<FWheelsGroup>& OutWheelsGroup) const;

    bool IsWheelContact() const { return bIsWheelContact; }

    void StartSendData(float SendDataRate);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TEnumAsByte<EObjectTypeQuery>> TreceForObjectTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WheelMaxSpeed{1000.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WheelRadius{20.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WheelDefaultDistance{60.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WheelMaxContactDistance{65.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WheelMinContactDistance{40.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.2", ClampMax = "3.0"))
    float WheelFrictionMultipler{1.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", ClampMax = "5.0"))
    float WheelMoveForceMultipler{1.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.5", ClampMax = "6.0"))
    float WheelSlideResist{1.f};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(Replicated)
    TArray<FWheelsGroup> WheelsGroups;

    float ControlInput;
    bool bIsWheelContact{false};
    bool bIsBreake{false};

    FTimerHandle ManageWheelsTimer;

    bool WheelSphereTrace(const FWheelData& Wheel, FHitResult& Hit);

    void ManageWheels();

    void ComputeWeelGroup(FWheelsGroup& WheelGroup);

    FVector GetSpringForce(float WheelVelocityZValue, float Dumping, float Stiffness, float ContactDistance, const FVector& ContactNormalPoint);
    FVector GetSideFriction(float WheelVelocityYValue, const FVector& RightVector);
    FVector GetMoveForce(float WheelVelocityXValue, float ContactNormalPointZValue, const FVector& ForwardVector);
};
