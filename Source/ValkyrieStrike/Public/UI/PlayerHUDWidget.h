// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UBorder;

UCLASS()
class VALKYRIESTRIKE_API UPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleFuelPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleEndurancePosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleConfigPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* CoinsValuePosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* LivesPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* WorkshopPosition;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleEndurenceWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> FuelGaugeWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> InGameVehicleConfigWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> CoinsValueWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> LivesWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> WorkshopWidgetClass;

    virtual void NativeOnInitialized() override;
};
