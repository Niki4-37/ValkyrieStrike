// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "GameCoreTypes.h"
#include "InGameVehicleConfigWidget.generated.h"

class UVerticalBox;
class UInGameVehicleItemWidget;

UCLASS()
class VALKYRIESTRIKE_API UInGameVehicleConfigWidget : public UBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> InGameVehicleItemWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ItemsBox;

    virtual void NativeOnInitialized() override;

private:
    virtual void OnNewPawn(APawn* NewPawn) override;
    void OnItemMount(const FVehicleItemData& Data);
    void OnChangeAmmo(EVehicleItemType Type, int32 NewValue);
    void OnStartReloading(EVehicleItemType Type);

    UPROPERTY()
    TMap<EVehicleItemType, UInGameVehicleItemWidget*> ItemsMap;
};
