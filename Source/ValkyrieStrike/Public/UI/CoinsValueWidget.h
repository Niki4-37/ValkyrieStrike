// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinsValueWidget.generated.h"

class UTextBlock;

UCLASS()
class VALKYRIESTRIKE_API UCoinsValueWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CoinsValue_Text;

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);
    void OnCoinsChanged(int32 Value);
};
