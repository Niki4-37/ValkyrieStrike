// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "GameCoreTypes.h"
#include "InGameVehicleConfigWidget.generated.h"

class UVerticalBox;
class UInGameVehicleUnitWidget;

UCLASS()
class VALKYRIESTRIKE_API UInGameVehicleConfigWidget : public UBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> InGameVehicleUnitWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* UnitsBox;

    virtual void NativeOnInitialized() override;

private:
    virtual void OnNewPawn(APawn* NewPawn) override;
    void OnUnitMount(const FVehicleUnitData& Data);
    void OnChangeAmmo(EVehicleUnitType Type, int32 NewValue);
    void OnStartReloading(EVehicleUnitType Type);

    UPROPERTY()
    TMap<EVehicleUnitType, UInGameVehicleUnitWidget*> UnitsMap;
};
