// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/InGameVehicleUnitWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

#include "Engine.h"

void UInGameVehicleUnitWidget::SetUnitData(const FVehicleUnitData& Data)
{
    if (UnitImage)
    {
        UnitImage->SetBrushFromSoftTexture(Data.UnitThumb);
        UnitImage->SetBrushSize(FVector2D(100.f));
    }

    if (AmmoCapacityBar)
    {
        AmmoCapacityBar->SetPercent(1.f);
    }

    MaxAmmoCapacity = Data.UnitMaxValue;

    if (ReloadingProgress)
    {
        ReloadingProgress->SetVisibility(ESlateVisibility::Hidden);
        ReloadingProgress->SetPercent(0.f);
    }
    ReloadingTime = Data.ExecutionTime;
    checkf(ReloadingTime, TEXT("ReloadingTime is ZERO!"));
    UnitType = Data.UnitType;
}

void UInGameVehicleUnitWidget::UpdateAmmoCapacityBar(int32 NewValue)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Ammo: %i"), NewValue));
    MaxAmmoCapacity > 0 ? AmmoCapacityBar->SetPercent(NewValue / FMath::CeilToFloat(MaxAmmoCapacity)) : AmmoCapacityBar->SetPercent(0.f);
}

void UInGameVehicleUnitWidget::StartReloading()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(ReloadingTimer)) return;

    AmmoCapacityBar->SetVisibility(ESlateVisibility::Hidden);
    ReloadingProgress->SetVisibility(ESlateVisibility::Visible);
    ReloadingProgress->SetPercent(0.f);

    GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &UInGameVehicleUnitWidget::EndReloading, ReloadingTime);

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            const float RemainingReloadingTime = GetWorld()->GetTimerManager().GetTimerElapsed(ReloadingTimer);
            ReloadingProgress->SetPercent(RemainingReloadingTime / ReloadingTime);
        });

    GetWorld()->GetTimerManager().SetTimer(ProgressTimer, TimerDelegate, ReloadingTime / 100.f, true);
}

void UInGameVehicleUnitWidget::BeginDestroy()
{
    Super::BeginDestroy();
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
        GetWorld()->GetTimerManager().ClearTimer(ProgressTimer);
    }
}

void UInGameVehicleUnitWidget::EndReloading()
{
    GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
    GetWorld()->GetTimerManager().ClearTimer(ProgressTimer);

    AmmoCapacityBar->SetVisibility(ESlateVisibility::Visible);
    ReloadingProgress->SetVisibility(ESlateVisibility::Hidden);
}
