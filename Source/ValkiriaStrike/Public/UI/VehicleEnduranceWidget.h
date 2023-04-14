// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleEnduranceWidget.generated.h"

class UProgressBar;

UCLASS()
class VALKIRIASTRIKE_API UVehicleEnduranceWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    virtual void NativeOnInitialized() override;

private:
    void OnHealthChanged(float HealthPercentage);
    void OnNewPawn(APawn* NewPawn);
};