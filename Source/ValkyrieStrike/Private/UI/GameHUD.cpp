// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/GameHUD.h"
#include "UI/PlayerHUDWidget.h"

void AGameHUD::CreateGameHUDWidgets()
{
    if (!GetOwningPlayerController() || !PlayerHUDWidgetClass) return;

    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->RemoveFromParent();
        PlayerHUDWidget = nullptr;
    }

    PlayerHUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    CreateGameHUDWidgets();
}

void AGameHUD::Destroyed()
{
    Super::Destroyed();

    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->RemoveFromParent();
    }
}
