// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/VehicleEnduranceWidget.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "Components/ProgressBar.h"

void UVehicleEnduranceWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;

    const auto VehicleIndicatorsComp = NewPawn->FindComponentByClass<UVehicleIndicatorsComponent>();
    if (!VehicleIndicatorsComp || VehicleIndicatorsComp->OnItemValueChanged.IsBoundToObject(this)) return;

    VehicleIndicatorsComp->OnItemValueChanged.AddUObject(this, &UVehicleEnduranceWidget::OnHealthChanged);
    VehicleIndicatorsComp->UpdateWidgetsInfo();
}

void UVehicleEnduranceWidget::OnHealthChanged(EItemPropertyType Type, float Health, float MaxHealth)
{
    if (Type != EItemPropertyType::Endurance) return;

    if (HealthBar)
    {
        MaxHealth > 0.f ? HealthBar->SetPercent(Health / MaxHealth) : HealthBar->SetPercent(0.f);
    }
}
