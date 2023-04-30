// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/VehicleEnduranceWidget.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "Components/ProgressBar.h"

void UVehicleEnduranceWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UVehicleEnduranceWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UVehicleEnduranceWidget::OnHealthChanged(float Health, float MaxHealth)
{
    if (HealthBar)
    {
        MaxHealth > 0.f ? HealthBar->SetPercent(Health / MaxHealth) : HealthBar->SetPercent(0.f);
    }
}

void UVehicleEnduranceWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto VehicleIndicatorsComp = NewPawn->FindComponentByClass<UVehicleIndicatorsComponent>();
    if (VehicleIndicatorsComp && !VehicleIndicatorsComp->OnHealthChanged.IsBoundToObject(this))
    {
        VehicleIndicatorsComp->OnHealthChanged.AddUObject(this, &UVehicleEnduranceWidget::OnHealthChanged);
    }
}
