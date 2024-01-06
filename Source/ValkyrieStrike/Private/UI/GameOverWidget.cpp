// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.


#include "UI/GameOverWidget.h"
#include "Components/Button.h"
#include "Player/VehiclePlayerController.h"
#include "Subsystems/SessionSubsystem.h"

void UGameOverWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnMainMenuClicked);
    }
}

void UGameOverWidget::OnMainMenuClicked() 
{
    if (const auto SessionSub = GetGameInstance()->GetSubsystem<USessionSubsystem>())
    {
        SessionSub->DestroySession();
    }
}
