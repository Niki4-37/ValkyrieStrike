// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "VehicleConfigWidget.generated.h"

class UButton;
class UHorizontalBox;
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
    UDataTable* VehicleItemsTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* VehicleConstructPartsTable;

    UPROPERTY(meta = (BindWidget))
    UBorder* BodyPartsSelectPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* ChassisPartsSelectPosition;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* TurretBox;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* SecondWeaponBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleUnitWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> OneTypeUnitsSelectWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    UOneTypeUnitsSelectWidget* BodyPartsSelectWidget{nullptr};

    UPROPERTY()
    UOneTypeUnitsSelectWidget* ChassisPartsSelectWidget{nullptr};

    void InitVehicleItems();
    void InitVehicleConstructParts();
};
