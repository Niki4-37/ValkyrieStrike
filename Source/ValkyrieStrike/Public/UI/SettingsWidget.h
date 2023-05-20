// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

class UButton;

UCLASS()
class VALKYRIESTRIKE_API USettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* GoBackButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnGoBack();
};
