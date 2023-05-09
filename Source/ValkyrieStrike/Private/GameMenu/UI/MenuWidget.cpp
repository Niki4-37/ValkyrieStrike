// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/MenuWidget.h"
#include "Components/Button.h"
#include "GameMenu/MenuPlayerController.h"

void UMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
    }
}

void UMenuWidget::OnStartGame()
{
    if (const auto PlayerController = Cast<AMenuPlayerController>(GetOwningPlayer()))
    {
        PlayerController->SetNewView(EMenuState::GameConfig);
    }
}
