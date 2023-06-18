// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/VehicleUnitWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "LobbyMenu/LobbyPlayerController.h"

void UVehicleUnitWidget::SetUnitData(const FVehicleItemData& Data)
{
    ItemData = Data;

    InitDescription(FText::FromName(Data.ItemName), Data.ItemThumb);
}

void UVehicleUnitWidget::SetUnitData(const FVehicleConstructPart& Part)
{
    ConstructPart = Part;

    InitDescription(FText::FromString(Part.PartName), Part.PartThumb);
}

void UVehicleUnitWidget::FocusOnWidget()
{
    if (UnitSelectButton)
    {
        UnitSelectButton->SetFocus();
    }
}

void UVehicleUnitWidget::ApplyData()
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

void UVehicleUnitWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (UnitSelectButton)
    {
        UnitSelectButton->SetIsEnabled(false);
        // UnitSelectButton->OnClicked.AddDynamic(this, &UVehicleUnitWidget::OnItemClicked);
    }
}

void UVehicleUnitWidget::OnItemClicked()
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

void UVehicleUnitWidget::InitDescription(FText UnitDescriptionText, UTexture2D* UnitThumb)
{
    if (UnitTextBlock)
    {
        UnitTextBlock->SetText(UnitDescriptionText);
    }
    if (UnitImage)
    {
        UnitImage->SetBrushFromTexture(UnitThumb);
        UnitImage->SetBrushSize(FVector2D(200.f));
    }
}
