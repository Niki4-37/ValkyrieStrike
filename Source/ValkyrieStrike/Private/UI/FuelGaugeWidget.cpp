// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/FuelGaugeWidget.h"
#include "Materials/MaterialInstance.h"
#include "Components/Image.h"
#include "Components/VehicleIndicatorsComponent.h"

void UFuelGaugeWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto VehicleIndicatorsComp = NewPawn->FindComponentByClass<UVehicleIndicatorsComponent>();
    if (!VehicleIndicatorsComp || VehicleIndicatorsComp->OnItemValueChanged.IsBoundToObject(this)) return;

    VehicleIndicatorsComp->OnItemValueChanged.AddUObject(this, &UFuelGaugeWidget::OnFuelValueChanged);
}

void UFuelGaugeWidget::OnFuelValueChanged(EItemPropertyType Type, float Value, float MaxValue)
{
    if (!FuelGaugeImage || Type != EItemPropertyType::Fuel) return;

    const auto FuelGaugeMat = FuelGaugeImage->GetDynamicMaterial();
    if (!FuelGaugeMat) return;

    const float ValueToSet = MaxValue > 0.f ? Value / MaxValue : 0.f;
    const auto Position = FMath::Lerp(LeftBorder, RightBorder, ValueToSet);

    FuelGaugeMat->SetScalarParameterValue(NeedlePositionParamName, Position);
}
