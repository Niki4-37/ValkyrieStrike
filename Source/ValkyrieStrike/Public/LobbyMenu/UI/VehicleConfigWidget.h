// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "VehicleConfigWidget.generated.h"

class UBorder;
class UOneTypeUnitsSelectWidget;

UCLASS()
class VALKYRIESTRIKE_API UVehicleConfigWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetStartParts();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* VehicleUnitsTable;

    UPROPERTY(meta = (BindWidget))
    UBorder* BodyPartsSelectPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* ChassisPartsSelectPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* TurretsSelectPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* SecondWeaponSelectPosition;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> OneTypeUnitsSelectWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    TMap<EVehicleUnitType, UOneTypeUnitsSelectWidget*> UnitSelectWidgetsMap;

    UPROPERTY()
    UOneTypeUnitsSelectWidget* BodyPartsSelectWidget{nullptr};

    UPROPERTY()
    UOneTypeUnitsSelectWidget* ChassisPartsSelectWidget{nullptr};

    UPROPERTY()
    UOneTypeUnitsSelectWidget* TurretsSelectWidget{nullptr};

    UPROPERTY()
    UOneTypeUnitsSelectWidget* SecondWeaponSelectWidget{nullptr};

    void InitVehicleUnits();
};
