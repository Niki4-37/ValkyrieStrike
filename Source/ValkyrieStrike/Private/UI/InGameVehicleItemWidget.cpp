// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/InGameVehicleItemWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UInGameVehicleItemWidget::SetItemData(const FVehicleItemData& Data)
{
    if (ItemImage)
    {
        ItemImage->SetBrushFromSoftTexture(Data.ItemTumb);
        ItemImage->SetBrushSize(FVector2D(100.f));
    }

    if (AmmoCapacityBar)
    {
        AmmoCapacityBar->SetPercent(1.f);
    }

    MaxAmmoCapacity = Data.MaxAmmoCapacity;

    if (ReloadingProgress)
    {
        ReloadingProgress->SetVisibility(ESlateVisibility::Hidden);
        ReloadingProgress->SetPercent(0.f);
    }
    ReloadingTime = Data.ReloadingTime;
    checkf(ReloadingTime, TEXT("ReloadingTime is ZERO!"));
    ItemType = Data.ItemType;
}

void UInGameVehicleItemWidget::UpdateAmmoCapacityBar(int32 NewValue)
{
    MaxAmmoCapacity > 0 ? AmmoCapacityBar->SetPercent(NewValue / FMath::CeilToFloat(MaxAmmoCapacity)) : AmmoCapacityBar->SetPercent(0.f);
}

void UInGameVehicleItemWidget::StartReloading()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(ReloadingTimer)) return;

    AmmoCapacityBar->SetVisibility(ESlateVisibility::Hidden);
    ReloadingProgress->SetVisibility(ESlateVisibility::Visible);
    ReloadingProgress->SetPercent(0.f);

    GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &UInGameVehicleItemWidget::EndReloading, ReloadingTime);

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            const float RemainingReloadingTime = GetWorld()->GetTimerManager().GetTimerElapsed(ReloadingTimer);
            ReloadingProgress->SetPercent(RemainingReloadingTime / ReloadingTime);
        });

    GetWorld()->GetTimerManager().SetTimer(ProgressTimer, TimerDelegate, ReloadingTime / 100.f, true);
}

void UInGameVehicleItemWidget::BeginDestroy()
{
    Super::BeginDestroy();
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
        GetWorld()->GetTimerManager().ClearTimer(ProgressTimer);
    }
}

void UInGameVehicleItemWidget::EndReloading()
{
    GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
    GetWorld()->GetTimerManager().ClearTimer(ProgressTimer);

    AmmoCapacityBar->SetVisibility(ESlateVisibility::Visible);
    ReloadingProgress->SetVisibility(ESlateVisibility::Hidden);
}
