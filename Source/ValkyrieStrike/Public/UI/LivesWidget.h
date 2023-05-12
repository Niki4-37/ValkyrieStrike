// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "LivesWidget.generated.h"

class UTextBlock;

UCLASS()
class VALKYRIESTRIKE_API ULivesWidget : public UBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Lives_Text;

private:
    virtual void OnNewPawn(APawn* NewPawn) override;
    void OnLivesChanged(int32 Value);
};
