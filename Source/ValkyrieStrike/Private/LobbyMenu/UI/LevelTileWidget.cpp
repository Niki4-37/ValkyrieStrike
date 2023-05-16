// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/LevelTileWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void ULevelTileWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }
    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumb);
        LevelImage->SetBrushSize(FVector2D(200.f));
    }

    if (FrameImage)
    {
        FrameImage->SetBrushSize(FVector2D(200.f));
    }
}

void ULevelTileWidget::SetSelected(bool IsSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void ULevelTileWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &ULevelTileWidget::OnLevelItemClicked);
    }
}

void ULevelTileWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}
