// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/InGameVehicleUnitWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstance.h"

void UInGameVehicleUnitWidget::SetUnitData(const FVehicleUnitData& Data)
{
    if (UnitImage)
    {
        UnitImage->SetBrushFromSoftTexture(Data.UnitThumb);
        UnitImage->SetBrushSize(FVector2D(100.f));
    }

    MaxAmmoCapacity = Data.UnitMaxValue;

    ReloadingTime = Data.ExecutionTime;
    checkf(ReloadingTime, TEXT("ReloadingTime is ZERO!"));
    UnitType = Data.UnitType;

    if (CapacityBarImage && CapacityBarImage->GetDynamicMaterial())
    {
        CapacityBarImage->SetBrushSize(FVector2D(150.f));
        CircleBarMaterial = CapacityBarImage->GetDynamicMaterial();
        CircleBarMaterial->SetScalarParameterValue(AmmoCapacityParamName, MaxAmmoCapacity);
    }
}

void UInGameVehicleUnitWidget::UpdateAmmoCapacityBar(int32 NewValue)
{
    if (!CircleBarMaterial) return;
    const float PercentageToSet = MaxAmmoCapacity > 0 ? NewValue / FMath::CeilToFloat(MaxAmmoCapacity) : 0.f;
    CircleBarMaterial->SetScalarParameterValue(PercentParamName, PercentageToSet);
}

void UInGameVehicleUnitWidget::StartReloading()
{
    if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(ReloadingTimer) || !CircleBarMaterial) return;

    CircleBarMaterial->SetVectorParameterValue(BaseColorParamName, FLinearColor(FVector(0.f, 0.9f, 1.f)));
    CircleBarMaterial->SetScalarParameterValue(PercentParamName, 0.f);

    GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &UInGameVehicleUnitWidget::EndReloading, ReloadingTime);

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            if (!CircleBarMaterial || GetWorld()->GetTimerManager().IsTimerPending(ReloadingTimer)) return;
            const float RemainingReloadingTime = GetWorld()->GetTimerManager().GetTimerElapsed(ReloadingTimer);
            CircleBarMaterial->SetScalarParameterValue(PercentParamName, RemainingReloadingTime / ReloadingTime);
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
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
        GetWorld()->GetTimerManager().ClearTimer(ProgressTimer);
    }

    if (!CircleBarMaterial) return;
    CircleBarMaterial->SetVectorParameterValue(BaseColorParamName, FLinearColor(FVector(0.f, 1.f, 0.f)));
}
