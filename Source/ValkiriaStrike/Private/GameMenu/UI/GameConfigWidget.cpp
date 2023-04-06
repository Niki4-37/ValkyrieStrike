// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/GameConfigWidget.h"
#include "Components/Button.h"
#include "GameMenu/MenuPlayerController.h"

void UGameConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UGameConfigWidget::OnBackClicked);
    }
}

void UGameConfigWidget::OnBackClicked()
{
    if (const auto PlayerController = Cast<AMenuPlayerController>(GetOwningPlayer()))
    {
        PlayerController->SetNewView(EMenuState::MainMenu);
    }
}
