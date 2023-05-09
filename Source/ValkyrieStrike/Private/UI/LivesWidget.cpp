// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/LivesWidget.h"
#include "Components/TextBlock.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"

void ULivesWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BindDelegate();
}

void ULivesWidget::BindDelegate()
{
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            if (!GetOwningPlayerPawn()) return;

            const auto ValkiriaPS = GetOwningPlayer()->GetPlayerState<AValkyriePlayerState>();
            if (!ValkiriaPS || ValkiriaPS->OnLivesChanged.IsBoundToObject(this))
            {
                GetWorld()->GetTimerManager().ClearTimer(FoundPlayerStateTimer);
                return;
            }
            ValkiriaPS->OnLivesChanged.AddUObject(this, &ULivesWidget::OnLivesChanged);
            ValkiriaPS->UpdateWidgetsInfo();

            GetWorld()->GetTimerManager().ClearTimer(FoundPlayerStateTimer);
        });

    GetWorld()->GetTimerManager().SetTimer(FoundPlayerStateTimer, TimerDelegate, 0.1f, true);
}

void ULivesWidget::OnLivesChanged(int32 Value)
{
    if (Lives_Text)
    {
        Lives_Text->SetText(FText::AsNumber(Value));
    }
}
