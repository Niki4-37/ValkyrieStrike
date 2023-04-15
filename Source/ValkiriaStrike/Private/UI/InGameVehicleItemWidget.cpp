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
}

void UInGameVehicleItemWidget::UpdateAmmoCapacityBar(int32 NewValue)
{
    MaxAmmoCapacity > 0 ? AmmoCapacityBar->SetPercent(NewValue / static_cast<float> (MaxAmmoCapacity)) : AmmoCapacityBar->SetPercent(0.f);
}
