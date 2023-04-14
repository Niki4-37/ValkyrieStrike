// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/FuelGaugeWidget.h"
#include "Materials/MaterialInstance.h"
#include "Components/Image.h"
#include "Components/VehicleIndicatorsComponent.h"

void UFuelGaugeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFuelGaugeWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UFuelGaugeWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto VehicleIndicatorsComp = NewPawn->FindComponentByClass<UVehicleIndicatorsComponent>();
    if (VehicleIndicatorsComp && !VehicleIndicatorsComp->OnFuelValueChanged.IsBoundToObject(this))
    {
        VehicleIndicatorsComp->OnFuelValueChanged.AddUObject(this, &UFuelGaugeWidget::OnFuelValueChanged);
    }
}

void UFuelGaugeWidget::OnFuelValueChanged(float Value)
{
    if (!FuelGaugeImage) return;

    const auto FuelGaugeMat = FuelGaugeImage->GetDynamicMaterial();
    if (!FuelGaugeMat) return;

    const auto Position = FMath::Lerp(MinValue, MaxValue, Value);

    FuelGaugeMat->SetScalarParameterValue(NeedlePositionParamName, Position);
}
