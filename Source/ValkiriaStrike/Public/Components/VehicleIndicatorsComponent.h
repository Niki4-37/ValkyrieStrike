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

    /* used in widget */
    FOnFuelValueChangedSignature OnFuelValueChanged;
    FOnCoinsChangedSignature OnCoinsChanged;

    void AddCoins(int32 Value);
    bool AddFuel(int32 Value);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)
    float FuelValue;
    float MaxFuelValue{100.f};

    UPROPERTY(Replicated)
    int32 Coins;

    FTimerHandle FuelChangeTimer;

    void SpendFuel();
};
