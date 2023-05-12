// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/LivesWidget.h"
#include "Components/TextBlock.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"

void ULivesWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto ValkiriaPS = NewPawn->GetPlayerState<AValkyriePlayerState>();
    if (!ValkiriaPS || ValkiriaPS->OnLivesChanged.IsBoundToObject(this)) return;

    ValkiriaPS->OnLivesChanged.AddUObject(this, &ULivesWidget::OnLivesChanged);
    ValkiriaPS->UpdateWidgetsInfo();
}

void ULivesWidget::OnLivesChanged(int32 Value)
{
    if (Lives_Text)
    {
        Lives_Text->SetText(FText::AsNumber(Value));
    }
}
