// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
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
    void OnHealthChanged(EItemPropertyType Type, float Health, float MaxHealth);
    void OnNewPawn(APawn* NewPawn);
};
