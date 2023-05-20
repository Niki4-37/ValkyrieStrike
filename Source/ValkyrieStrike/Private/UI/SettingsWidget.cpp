// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/SettingsWidget.h"
#include "Components/Button.h"
#include "GameMenu/MenuPlayerController.h"

void USettingsWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (GoBackButton)
    {
        GoBackButton->OnClicked.AddDynamic(this, &USettingsWidget::OnGoBack);
    }
}

void USettingsWidget::OnGoBack() 
{
    if (const auto MenuPlayerController = Cast<AMenuPlayerController>(GetOwningPlayer()))
    {
        MenuPlayerController->SetNewView(EMenuState::MainMenu);
    }
}
