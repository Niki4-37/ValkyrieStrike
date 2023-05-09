// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UCLASS()
class VALKIRIASTRIKE_API AGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    void CreateGameHUDWidgets();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UUserWidget* PlayerHUDWidget;
};
