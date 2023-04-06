// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameConfigWidget.generated.h"

class UButton;

UCLASS()
class VALKIRIASTRIKE_API UGameConfigWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnBackClicked();
};
