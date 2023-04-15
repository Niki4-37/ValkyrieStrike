// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UBorder;

UCLASS()
class VALKIRIASTRIKE_API UPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleFuelPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleEndurancePosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleConfigPosition;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleEndurenceWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> FuelGaugeWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> InGameVehicleConfigWidgetClass;

    virtual void NativeOnInitialized() override;
};
