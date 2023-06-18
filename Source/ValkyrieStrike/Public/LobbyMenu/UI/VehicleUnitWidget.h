// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "VehicleUnitWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class VALKYRIESTRIKE_API UVehicleUnitWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetUnitData(const FVehicleItemData& Data);
    void SetUnitData(const FVehicleConstructPart& Part);

    void FocusOnWidget();
    void ApplyData();

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* UnitSelectButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* UnitTextBlock;

    UPROPERTY(meta = (BindWidget))
    UImage* UnitImage = nullptr;

    virtual void NativeOnInitialized() override;

private:
    FVehicleItemData ItemData;

    FVehicleConstructPart ConstructPart;

    UFUNCTION()
    void OnItemClicked();

    void InitDescription(FText UnitDescriptionText, UTexture2D* UnitThumb);
};
