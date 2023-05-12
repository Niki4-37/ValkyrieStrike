// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "GameCoreTypes.h"
#include "VehicleEnduranceWidget.generated.h"

class UProgressBar;

UCLASS()
class VALKYRIESTRIKE_API UVehicleEnduranceWidget : public UBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

private:
    void OnHealthChanged(EItemPropertyType Type, float Health, float MaxHealth);
    virtual void OnNewPawn(APawn* NewPawn) override;
};
