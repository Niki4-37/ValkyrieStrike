// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "VehicleItemWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class VALKYRIESTRIKE_API UVehicleItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetItemData(const FVehicleItemData& Data);

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ItemSelectButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage = nullptr;

    virtual void NativeOnInitialized() override;

private:
    FVehicleItemData ItemData;

    UFUNCTION()
    void OnItemClicked();
};
