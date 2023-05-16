// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/LobbyHUD.h"
#include "Blueprint/UserWidget.h"

void ALobbyHUD::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    LobbyHUDWidget = CreateWidget<UUserWidget>(GetWorld(), LobbyHUDWidgetClass);
    if (LobbyHUDWidget)
    {
        LobbyHUDWidget->AddToViewport();
    }
}

void ALobbyHUD::Destroyed()
{
    Super::Destroyed();

    if (LobbyHUDWidget)
    {
        LobbyHUDWidget->RemoveFromParent();
    }
}

void ALobbyHUD::BeginPlay()
{
    Super::BeginPlay();

    checkf(LobbyHUDWidgetClass, TEXT("LobbyHUDWidgetClass not define!"));
}
