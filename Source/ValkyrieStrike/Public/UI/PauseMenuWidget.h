// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class VALKYRIESTRIKE_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GoBackButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    void NativeOnInitialized();

private:
    UFUNCTION()
    void OnSettingClicked();

    UFUNCTION()
    void OnGoBackClicked();

    UFUNCTION()
    void OnQuitClicked();
};
