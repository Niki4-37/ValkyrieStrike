// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/PlayerHUDWidget.h"
#include "Components/Border.h"

void UPlayerHUDWidget::NativeOnInitialized()
{
    checkf(VehicleEndurenceWidgetClass, TEXT("VehicleEndurenceWidgetClass not define!"));
    checkf(FuelGaugeWidgetClass, TEXT("FuelGaugeWidgetClass not define!"));
    checkf(InGameVehicleConfigWidgetClass, TEXT("InGameVehicleConfigWidgetClass not define!"));
    checkf(CoinsValueWidgetClass, TEXT("CoinsValueWidgetClass not define!"));
    checkf(WorkshopWidgetClass, TEXT("WorkshopWidgetClass not define!"));

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

    if (CoinsValuePosition)
    {
        const auto CoinsWidget = CreateWidget(GetWorld(), CoinsValueWidgetClass);
        if (CoinsWidget)
        {
            CoinsValuePosition->AddChild(CoinsWidget);
        }
    }

    if (WorkshopPosition)
    {
        const auto WorkshopWidget = CreateWidget(GetWorld(), WorkshopWidgetClass);
        if (WorkshopWidget)
        {
            WorkshopPosition->AddChild(WorkshopWidget);
        }
    }
}
