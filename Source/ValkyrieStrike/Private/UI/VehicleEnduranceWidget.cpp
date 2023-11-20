// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/VehicleEnduranceWidget.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "Components/Image.h"

void UVehicleEnduranceWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;

    const auto VehicleIndicatorsComp = NewPawn->FindComponentByClass<UVehicleIndicatorsComponent>();
    if (!VehicleIndicatorsComp || VehicleIndicatorsComp->OnItemValueChanged.IsBoundToObject(this)) return;

    VehicleIndicatorsComp->OnItemValueChanged.AddUObject(this, &UVehicleEnduranceWidget::OnHealthChanged);
    VehicleIndicatorsComp->UpdateWidgetsInfo();

    if (HealthBarImage && HealthBarImage->GetDynamicMaterial())
    {
        HealthBarMaterial = HealthBarImage->GetDynamicMaterial();
        HealthBarMaterial->SetScalarParameterValue(PercentParamName, 1.f);
    }
}

void UVehicleEnduranceWidget::OnHealthChanged(EItemPropertyType Type, float Health, float MaxHealth)
{
    if (Type != EItemPropertyType::Endurance) return;

    if (HealthBarMaterial)
    {
        const float PercentageToSet = MaxHealth > 0 ? Health / MaxHealth : 0.f;
        HealthBarMaterial->SetScalarParameterValue(PercentParamName, PercentageToSet);
    }
}
