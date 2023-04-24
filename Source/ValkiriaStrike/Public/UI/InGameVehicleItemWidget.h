// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "InGameVehicleItemWidget.generated.h"

class UImage;
class UProgressBar;

UCLASS()
class VALKIRIASTRIKE_API UInGameVehicleItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetItemData(const FVehicleItemData& Data);
    void UpdateAmmoCapacityBar(int32 NewValue);
    void StartReloading();

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* AmmoCapacityBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ReloadingProgress;

private:
    int32 MaxAmmoCapacity;
    float ReloadingTime;
    FTimerHandle ReloadingTimer;
    FTimerHandle ProgressTimer;

    void EndReloading();
};
