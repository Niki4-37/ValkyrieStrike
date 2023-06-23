// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/VehicleUnitWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "LobbyMenu/LobbyPlayerController.h"

void UVehicleUnitWidget::SetUnitData(const FVehicleUnitData& Unit)
{
    UnitData = Unit;

    InitDescription(FText::FromString(Unit.UnitDescription), Unit.UnitThumb);
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

    LobbyPlayerController->VehicleUnitHasSelected_OnServer(UnitData);
}

void UVehicleUnitWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (UnitSelectButton)
    {
        UnitSelectButton->SetIsEnabled(false);
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
