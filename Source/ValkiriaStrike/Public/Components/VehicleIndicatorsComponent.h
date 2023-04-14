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

    FOnFuelValueChangedSignature OnFuelValueChanged;

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)
    float FuelValue;
    float MaxFuelValue{100.f};

    FTimerHandle FuelChangeTimer;

    void ChangeFuelValue();
};
