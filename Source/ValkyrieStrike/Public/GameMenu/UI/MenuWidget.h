// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "MenuWidget.generated.h"

class UButton;

UCLASS()
class VALKYRIESTRIKE_API UMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStartGame();
};
