// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/VehicleConfigWidget.h"
#include "Components/HorizontalBox.h"
#include "GameMenu/UI/VehicleItemWidget.h"

void UVehicleConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitVehicleItems();
}

void UVehicleConfigWidget::InitVehicleItems()
{
    checkf(VehicleItemWidgetClass, TEXT("VehicleItemWidgetClass not define!"));

    TArray<FVehicleItemData*> ItemPtrs;
    VehicleItemsTable->GetAllRows<FVehicleItemData>("", ItemPtrs);

    if (!TurretBox || !SecondWeaponBox) return;

    for (auto VehicleItemPtr : ItemPtrs)
    {
        if (!VehicleItemPtr) continue;
        const auto VehicleItem = *VehicleItemPtr;

        const auto VehicleItemWidget = CreateWidget<UVehicleItemWidget>(GetWorld(), VehicleItemWidgetClass);
        if (!VehicleItemWidget) continue;
        VehicleItemWidget->SetItemData(VehicleItem);

        if (VehicleItem.ItemType == EVehicleItemType::Turret)
        {
            TurretBox->AddChild(VehicleItemWidget);
        }

        if (VehicleItem.ItemType == EVehicleItemType::SecondWeapon)
        {
            SecondWeaponBox->AddChild(VehicleItemWidget);
        }
    }
}
