// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/VehicleItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "LobbyMenu/LobbyPlayerController.h"

void UVehicleItemWidget::SetItemData(const FVehicleItemData& Data)
{
    ItemData = Data;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.ItemName));
    }
    if (ItemImage)
    {
        ItemImage->SetBrushFromTexture(Data.ItemTumb);
        ItemImage->SetBrushSize(FVector2D(200.f));
    }
}

void UVehicleItemWidget::SetItemData(const FVehicleConstructPart& Part)
{
    ConstructPart = Part;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromString(Part.PartName));
    }
    if (ItemImage)
    {
        ItemImage->SetBrushFromTexture(Part.PartTumb);
        ItemImage->SetBrushSize(FVector2D(200.f));
    }
}

void UVehicleItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ItemSelectButton)
    {
        ItemSelectButton->OnClicked.AddDynamic(this, &UVehicleItemWidget::OnItemClicked);
    }
}

void UVehicleItemWidget::OnItemClicked()
{
    const auto LobbyPlayerController = Cast<ALobbyPlayerController>(GetOwningPlayer());
    if (!LobbyPlayerController) return;

    if (ItemData.ItemName != NAME_None)
    {
        LobbyPlayerController->VehicleItemHasSelected_OnServer(ItemData);
    }
    else
    {
        LobbyPlayerController->VehiclePartHasSelected_OnServer(ConstructPart);
    }
}
