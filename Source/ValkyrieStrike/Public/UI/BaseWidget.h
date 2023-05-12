// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

UCLASS()
class VALKYRIESTRIKE_API UBaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void OnNewPawn(APawn* NewPawn);
};
