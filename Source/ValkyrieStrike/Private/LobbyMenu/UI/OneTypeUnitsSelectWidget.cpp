// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/OneTypeUnitsSelectWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "LobbyMenu/UI/VehicleUnitWidget.h"

void UOneTypeUnitsSelectWidget::AddUnit(const FVehicleUnitData& Unit)
{
    if (!GetWorld() || !UnitsWrapBox) return;

    const auto VehicleUnitWidget = CreateWidget<UVehicleUnitWidget>(GetWorld(), VehicleUnitWidgetClass);
    if (!VehicleUnitWidget) return;
    VehicleUnitWidget->SetUnitData(Unit);
    UnitsWrapBox->AddChild(VehicleUnitWidget);
    TotalUnits = UnitsWrapBox->GetChildrenCount();
}

void UOneTypeUnitsSelectWidget::SetStartPart()
{
    if (!UnitsWrapBox || UnitsWrapBox->GetChildrenCount() <= 0) return;

    const auto FirstUnitWidget = Cast<UVehicleUnitWidget>(UnitsWrapBox->GetChildAt(CurrentUnitNum));
    if (!FirstUnitWidget) return;
    FirstUnitWidget->FocusOnWidget();
    FirstUnitWidget->ApplyData();
}

void UOneTypeUnitsSelectWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ForwardButton)
    {
        ForwardButton->OnClicked.AddDynamic(this, &UOneTypeUnitsSelectWidget::OnForwardButtonClicked);
    }
    if (BackwardButton)
    {
        BackwardButton->OnClicked.AddDynamic(this, &UOneTypeUnitsSelectWidget::OnBackwardButtonClicked);
    }
}

void UOneTypeUnitsSelectWidget::OnForwardButtonClicked()
{
    ToForwardUnit(true);
}

void UOneTypeUnitsSelectWidget::OnBackwardButtonClicked()
{
    ToForwardUnit(false);
}

void UOneTypeUnitsSelectWidget::ToForwardUnit(bool bIsForward)
{
    if (TotalUnits <= 0) return;
    CurrentUnitNum = (bIsForward ? ++CurrentUnitNum : --CurrentUnitNum) % TotalUnits;

    if (CurrentUnitNum < 0)
    {
        CurrentUnitNum = TotalUnits - 1;
    }

    const auto FoundChild = UnitsWrapBox->GetChildAt(CurrentUnitNum);
    if (const auto VehicleUnitWidget = Cast<UVehicleUnitWidget>(FoundChild))
    {
        VehicleUnitWidget->FocusOnWidget();
        VehicleUnitWidget->ApplyData();
    }
}
