// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "MenuWidget.generated.h"

class UButton;
class UVerticalBox;
class UCircularThrobber;

UCLASS()
class VALKYRIESTRIKE_API UMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* CreateSessionButton;

    UPROPERTY(meta = (BindWidget))
    UButton* FindSessionButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* FoundSessionsBox;

    UPROPERTY(meta = (BindWidget))
    UCircularThrobber* FindingSessionProcess;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> JionSessionWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnFindGame();

    void OnFoundSessionData(const FString& SessionID, int32 ConnectionNum, int32 MaxPlayers);

    UFUNCTION()
    void OnCreateGame();

    UFUNCTION()
    void OnOpenSettings();

    UFUNCTION()
    void OnQuit();
};
