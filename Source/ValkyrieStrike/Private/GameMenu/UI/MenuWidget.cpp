// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/MenuWidget.h"
#include "Components/Button.h"
#include "ValkyrieGameInstance.h"
#include "GameMenu/UI/JoinSessionWidget.h"
#include "Components/VerticalBox.h"
#include "Subsystems/SessionSubsystem.h"

void UMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (FindSessionButton)
    {
        FindSessionButton->OnClicked.AddDynamic(this, &UMenuWidget::OnFindGame);
    }

    if (CreateSessionButton)
    {
        CreateSessionButton->OnClicked.AddDynamic(this, &UMenuWidget::OnCreateGame);
    }

    if (GetGameInstance()->GetSubsystem<USessionSubsystem>())
    {
        GetGameInstance()->GetSubsystem<USessionSubsystem>()->OnFoundSessionData.AddUObject(this, &UMenuWidget::OnFoundSessionData);
    }
}

void UMenuWidget::OnFindGame()
{
    if (const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>())
    {
        ValkyrieGameInstance->FindGame();
    }

    if (FoundSessionsBox)
    {
        FoundSessionsBox->ClearChildren();
    }
}

void UMenuWidget::OnFoundSessionData(const FString& SessionID, int32 ConnectionNum, int32 MaxPlayers)
{
    if (!JionSessionWidgetClass || !FoundSessionsBox) return;
    const auto SessionWidget = CreateWidget<UJoinSessionWidget>(GetOwningPlayer(), JionSessionWidgetClass);
    if (!SessionWidget) return;
    SessionWidget->InitWidget(SessionID, ConnectionNum, MaxPlayers);
    FoundSessionsBox->AddChild(SessionWidget);
}

void UMenuWidget::OnCreateGame()
{
    if (const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>())
    {
        ValkyrieGameInstance->CreateGame();
    }
}
