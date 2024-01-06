// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "Player/VehiclePlayerController.h"
#include "Subsystems/SessionSubsystem.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSettingClicked);
    }
    if (GoBackButton)
    {
        GoBackButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnGoBackClicked);
    }
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
    }
}

void UPauseMenuWidget::OnSettingClicked()
{
    if (const auto PlayerController = Cast<AVehiclePlayerController>(GetOwningPlayer()))
    {
        PlayerController->ChangeGameState_OnClient(EValkyrieGameState::GameSettings);
    }
}

void UPauseMenuWidget::OnGoBackClicked()
{
    if (!GetOwningPlayer()) return;
    GetOwningPlayer()->SetPause(false);
}

void UPauseMenuWidget::OnQuitClicked()
{
    if (const auto SessionSub = GetGameInstance()->GetSubsystem<USessionSubsystem>())
    {
        SessionSub->DestroySession();
    }
}
