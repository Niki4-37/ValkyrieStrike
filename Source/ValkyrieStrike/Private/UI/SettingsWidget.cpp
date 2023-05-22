// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/SettingsWidget.h"
#include "Components/Button.h"
#include "GameMenu/MenuPlayerController.h"
#include "Player/VehiclePlayerController.h"

void USettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GoBackButton)
    {
        GoBackButton->OnClicked.AddDynamic(this, &USettingsWidget::OnGoBackClicked);
    }
}

void USettingsWidget::OnGoBackClicked()
{
    if (const auto MenuPlayerController = Cast<AMenuPlayerController>(GetOwningPlayer()))
    {
        MenuPlayerController->SetNewView(EMenuState::MainMenu);
    }

    if (const auto VehiclePlayerController = Cast<AVehiclePlayerController>(GetOwningPlayer()))
    {
        VehiclePlayerController->ChangeGameState(EValkyrieGameState::Pause);
    }
}
