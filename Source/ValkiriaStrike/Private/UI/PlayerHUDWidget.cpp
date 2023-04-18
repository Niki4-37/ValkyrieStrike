// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/PlayerHUDWidget.h"
#include "Components/Border.h"

void UPlayerHUDWidget::NativeOnInitialized()
{
    checkf(VehicleEndurenceWidgetClass, TEXT("VehicleEndurenceWidgetClass not define!"));
    checkf(FuelGaugeWidgetClass, TEXT("FuelGaugeWidgetClass not define!"));
    checkf(InGameVehicleConfigWidgetClass, TEXT("InGameVehicleConfigWidgetClass not define!"));

    if (VehicleEndurancePosition)
    {
        const auto EnduranceWidget = CreateWidget(GetWorld(), VehicleEndurenceWidgetClass);
        if (EnduranceWidget)
        {
            VehicleEndurancePosition->AddChild(EnduranceWidget);
        }
    }

    if (VehicleFuelPosition)
    {
        const auto FuelGaugeWidget = CreateWidget(GetWorld(), FuelGaugeWidgetClass);
        if (FuelGaugeWidget)
        {
            VehicleFuelPosition->AddChild(FuelGaugeWidget);
        }
    }

    if (VehicleConfigPosition)
    {
        const auto ConfigWidget = CreateWidget(GetWorld(), InGameVehicleConfigWidgetClass);
        if (ConfigWidget)
        {
            VehicleConfigPosition->AddChild(ConfigWidget);
        }
    }
}
