// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "CoinsValueWidget.generated.h"

class UTextBlock;

UCLASS()
class VALKYRIESTRIKE_API UCoinsValueWidget : public UBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CoinsValue_Text;

private:
    virtual void OnNewPawn(APawn* NewPawn) override;
    void OnCoinsChanged(int32 Value);
};
