// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameCoreTypes.h"
#include "GameHUD.generated.h"

UCLASS()
class VALKYRIESTRIKE_API AGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> SettingsWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> RespawnWidgetClass;

    virtual void BeginPlay() override;
    virtual void Destroyed() override;

private:
    UPROPERTY()
    UUserWidget* CurrentWidget;

    UPROPERTY()
    TMap<EValkyrieGameState, UUserWidget*> GameStateWidgetsMap;

    void OnGameStateChanged(EValkyrieGameState State);
};
