// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UBorder;

UCLASS()
class VALKIRIASTRIKE_API UPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleFuelPosition;

    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleEndurancePosition;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleEndurenceWidgetClass;

    virtual void NativeOnInitialized() override;
};
