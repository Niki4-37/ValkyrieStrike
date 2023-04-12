// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/PlayerHUDWidget.h"
#include "Components/Border.h"

void UPlayerHUDWidget::NativeOnInitialized()
{
    checkf(VehicleEndurenceWidgetClass, TEXT("VehicleEndurenceWidgetClass not define!"));

    if (VehicleEndurancePosition)
    {
        const auto EnduranceWidget = CreateWidget(GetWorld(), VehicleEndurenceWidgetClass);
        if (EnduranceWidget)
        {
            VehicleEndurancePosition->AddChild(EnduranceWidget);
        }
    }
}
