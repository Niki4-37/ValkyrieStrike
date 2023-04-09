// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/MenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu/MenuPlayerController.h"

void AMenuHUD::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    checkf(MenuWidgetClass, TEXT("MenuWidgetClass not define!"));
    checkf(GameConfigWidgetClass, TEXT("GameConfigWidgetClass not define!"));

    MenuWidgetsMap.Add(EMenuState::MainMenu, CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass));
    MenuWidgetsMap.Add(EMenuState::GameConfig, CreateWidget<UUserWidget>(GetWorld(), GameConfigWidgetClass));

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

void AMenuHUD::BeginPlay()
{
    Super::BeginPlay();
}

void AMenuHUD::OnMenuStateChanged(EMenuState NewState)
{
    UE_LOG(LogTemp, Display, TEXT("EMenuState: %s"), *UEnum::GetValueAsString(NewState));

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
