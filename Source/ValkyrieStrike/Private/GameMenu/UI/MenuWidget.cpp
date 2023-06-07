// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/MenuWidget.h"
#include "Components/Button.h"
#include "ValkyrieGameInstance.h"
#include "GameMenu/UI/JoinSessionWidget.h"
#include "Components/VerticalBox.h"
#include "Components/CircularThrobber.h"
#include "Subsystems/SessionSubsystem.h"
#include "GameMenu/MenuPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (FindSessionButton)
    {
        FindSessionButton->OnClicked.AddDynamic(this, &ThisClass::OnFindGame);
    }
    if (CreateSessionButton)
    {
        CreateSessionButton->OnClicked.AddDynamic(this, &ThisClass::OnCreateGame);
    }
    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &ThisClass::OnOpenSettings);
    }
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuit);
    }
    if (FindingSessionProcess)
    {
        FindingSessionProcess->SetVisibility(ESlateVisibility::Collapsed);
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

    if (FindingSessionProcess)
    {
        FindingSessionProcess->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMenuWidget::OnFoundSessionData(const FString& SessionID, int32 ConnectionNum, int32 MaxPlayers)
{
    if (!JionSessionWidgetClass || !FoundSessionsBox) return;

    if (FindingSessionProcess)
    {
        FindingSessionProcess->SetVisibility(ESlateVisibility::Collapsed);
    }

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

void UMenuWidget::OnOpenSettings()
{
    if (const auto PlayerController = Cast<AMenuPlayerController>(GetOwningPlayer()))
    {
        PlayerController->SetNewView(EMenuState::GameSettings);
    }
}

void UMenuWidget::OnQuit()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
