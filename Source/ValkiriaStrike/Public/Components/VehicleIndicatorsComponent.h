// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "VehicleIndicatorsComponent.generated.h"

UCLASS()
class VALKIRIASTRIKE_API UVehicleIndicatorsComponent : public UHealthComponent
{
    GENERATED_BODY()

public:
    UVehicleIndicatorsComponent();

    bool AddFuel(int32 Value);

    UFUNCTION(Server, unreliable)
    void SetFuelConsumptionModifier_OnServer(float Percentage);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)
    float FuelValue{0.f};
    float MaxFuelValue{100.f};
    UPROPERTY(Replicated)
    float FuelConsumptionModifier{1.f};

    FTimerHandle FuelChangeTimer;

    void SpendFuel();

    UFUNCTION(NetMulticast, reliable)
    void OnFuelValueChanged_Multicast(float Value, float MaxValue);
};
