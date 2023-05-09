// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LivesWidget.generated.h"

class UTextBlock;

UCLASS()
class VALKIRIASTRIKE_API ULivesWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Lives_Text;

    virtual void NativeOnInitialized() override;

private:
    FTimerHandle FoundPlayerStateTimer;

    void BindDelegate();
    void OnLivesChanged(int32 Value);
};
