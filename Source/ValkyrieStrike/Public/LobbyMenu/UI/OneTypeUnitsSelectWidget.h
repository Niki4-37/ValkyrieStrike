// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoretypes.h"
#include "OneTypeUnitsSelectWidget.generated.h"

class UButton;
class UWrapBox;

UCLASS()
class VALKYRIESTRIKE_API UOneTypeUnitsSelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void AddUnit(const FVehicleItemData& Item);
    void AddUnit(const FVehicleConstructPart& Part);

    void SetStartPart();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleUnitWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UWrapBox* UnitsWrapBox;

    UPROPERTY(meta = (BindWidget))
    UButton* ForwardButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BackwardButton;

    virtual void NativeOnInitialized() override;

private:
    int32 CurrentUnitNum{0};
    int32 TotalUnits{0};

    UFUNCTION()
    void OnForwardButtonClicked();

    UFUNCTION()
    void OnBackwardButtonClicked();

    void ToForwardUnit(bool bIsForward);
};
