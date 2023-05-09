// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameCoreTypes.h"
#include "MenuHUD.generated.h"

UCLASS()
class VALKIRIASTRIKE_API AMenuHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void PostInitializeComponents() override;
    virtual void Destroyed();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> GameConfigWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EMenuState, UUserWidget*> MenuWidgetsMap;

    UPROPERTY()
    UUserWidget* ActiveWidget{nullptr};

    void OnMenuStateChanged(EMenuState NewState);
};
