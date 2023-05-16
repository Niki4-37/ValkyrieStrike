// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/MenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu/MenuPlayerController.h"

void AMenuHUD::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    MenuWidgetsMap.Add(EMenuState::MainMenu, CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass));
    // MenuWidgetsMap.Add(EMenuState::GameSettings, CreateWidget<UUserWidget>(GetWorld(), GameSettingsWidgetClass));

    for (auto MenuWidgetPair : MenuWidgetsMap)
    {
        const auto MenuWidget = MenuWidgetPair.Value;
        if (!MenuWidget) continue;

        MenuWidget->AddToViewport();
        MenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (const auto MenuPC = Cast<AMenuPlayerController>(GetOwningPlayerController()))
    {
        MenuPC->OnMenuStateChanged.AddUObject(this, &AMenuHUD::OnMenuStateChanged);
    }
}

void AMenuHUD::Destroyed()
{
    Super::Destroyed();

    for (auto MenuWidgetPair : MenuWidgetsMap)
    {
        const auto MenuWidget = MenuWidgetPair.Value;
        if (!MenuWidget) continue;

        MenuWidget->RemoveFromParent();
    }
}

void AMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    checkf(MenuWidgetClass, TEXT("MenuWidgetClass not define!"));
    // checkf(GameSettingsWidgetClass, TEXT("GameConfigWidgetClass not define!"));
}

void AMenuHUD::OnMenuStateChanged(EMenuState NewState)
{
    if (ActiveWidget)
    {
        ActiveWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (MenuWidgetsMap.Contains(NewState))
    {
        ActiveWidget = MenuWidgetsMap[NewState];
    }

    if (ActiveWidget)
    {
        ActiveWidget->SetVisibility(ESlateVisibility::Visible);
    }
}
