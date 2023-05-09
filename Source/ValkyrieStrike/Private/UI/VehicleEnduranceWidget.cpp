// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/VehicleEnduranceWidget.h"
#include "Components/VehicleIndicatorsComponent.h"
#include "Components/ProgressBar.h"

#include "GameFramework/PlayerState.h"

void UVehicleEnduranceWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UVehicleEnduranceWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UVehicleEnduranceWidget::OnNewPawn(APawn* NewPawn)
{
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            if (!GetOwningPlayerPawn()) return;

            const auto VehicleIndicatorsComp = GetOwningPlayerPawn()->FindComponentByClass<UVehicleIndicatorsComponent>();
            if (!VehicleIndicatorsComp || VehicleIndicatorsComp->OnItemValueChanged.IsBoundToObject(this))
            {
                GetWorld()->GetTimerManager().ClearTimer(FoundPawnTimer);
                return;
            }
            VehicleIndicatorsComp->OnItemValueChanged.AddUObject(this, &UVehicleEnduranceWidget::OnHealthChanged);
            VehicleIndicatorsComp->UpdateWidgetsInfo();
            
            GetWorld()->GetTimerManager().ClearTimer(FoundPawnTimer);
            
        });
    GetWorld()->GetTimerManager().SetTimer(FoundPawnTimer, TimerDelegate, 0.1f, true);
}

void UVehicleEnduranceWidget::OnHealthChanged(EItemPropertyType Type, float Health, float MaxHealth)
{
    if (Type != EItemPropertyType::Endurance) return;

    if (HealthBar)
    {
        MaxHealth > 0.f ? HealthBar->SetPercent(Health / MaxHealth) : HealthBar->SetPercent(0.f);
    }
}
