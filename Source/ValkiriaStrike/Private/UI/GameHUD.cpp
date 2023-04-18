// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/GameHUD.h"
#include "UI/PlayerHUDWidget.h"

void AGameHUD::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void AGameHUD::CreateGameHUDWidgets()
{
    const auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    checkf(PlayerHUDWidgetClass, TEXT("PlayerHUDWidgetClass not define!"));

    CreateGameHUDWidgets();
}
