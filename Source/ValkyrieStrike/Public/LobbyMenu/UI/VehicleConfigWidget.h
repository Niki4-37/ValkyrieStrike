// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "VehicleConfigWidget.generated.h"

class UButton;
class UHorizontalBox;

UCLASS()
class VALKYRIESTRIKE_API UVehicleConfigWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* VehicleItemsTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* VehicleConstructPartsTable;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* BodyBox;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* ChassisBox;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* TurretBox;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* SecondWeaponBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleUnitWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void InitVehicleItems();
    void InitVehicleConstructParts();
};
