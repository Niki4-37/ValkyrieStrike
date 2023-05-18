// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/JoinSessionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ValkyrieGameInstance.h"

void UJoinSessionWidget::InitWidget(const FString& SessionID, int32 ConnectionNum, int32 MaxPlayers)
{
    if (SessionInfo)
    {
        const FString SessionInfoStr = SessionID.Mid(0, 15) + FString::FromInt(ConnectionNum) + "/" + FString::FromInt(MaxPlayers);
        SessionInfo->SetText(FText::FromString(SessionInfoStr));
    }
}

void UJoinSessionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (JoinSessionButton)
    {
        JoinSessionButton->OnClicked.AddDynamic(this, &UJoinSessionWidget::OnJoinSession);
    }
}

void UJoinSessionWidget::OnJoinSession()
{
    if (const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>())
    {
        ValkyrieGameInstance->JoinGame();
    }
}
