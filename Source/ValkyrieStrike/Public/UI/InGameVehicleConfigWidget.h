// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "InGameVehicleConfigWidget.generated.h"

class UVerticalBox;
class UInGameVehicleItemWidget;

UCLASS()
class VALKYRIESTRIKE_API UInGameVehicleConfigWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> InGameVehicleItemWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ItemsBox;

    virtual void NativeOnInitialized() override;

private:
    FTimerHandle FoundPawnTimer;

    void OnNewPawn(APawn* NewPawn);
    void OnItemMount(const FVehicleItemData& Data);
    void OnChangeAmmo(EVehicleItemType Type, int32 NewValue);
    void OnStartReloading(EVehicleItemType Type);

    UPROPERTY()
    TMap<EVehicleItemType, UInGameVehicleItemWidget*> ItemsMap;
};
