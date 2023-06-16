// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/VehicleConfigWidget.h"
#include "Components/HorizontalBox.h"
#include "LobbyMenu/UI/VehicleItemWidget.h"

void UVehicleConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(VehicleItemWidgetClass, TEXT("VehicleItemWidgetClass not define!"));

    InitVehicleConstructParts();
    InitVehicleItems();
}

void UVehicleConfigWidget::InitVehicleItems()
{
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

void UVehicleConfigWidget::InitVehicleConstructParts()
{
    TArray<FVehicleConstructPart*> PartsPtrs;
    VehicleConstructPartsTable->GetAllRows<FVehicleConstructPart>("", PartsPtrs);

    if (!BodyBox || !ChassisBox) return;

    for (auto VehiclePartPtr : PartsPtrs)
    {
        if (!VehiclePartPtr) continue;
        const auto Part = *VehiclePartPtr;
        const auto VehiclePartWidget = CreateWidget<UVehicleItemWidget>(GetWorld(), VehicleItemWidgetClass);
        if (!VehiclePartWidget) continue;
        VehiclePartWidget->SetItemData(Part);

        if (Part.PartType == EVehicleConstructPartType::Body)
        {
            BodyBox->AddChild(VehiclePartWidget);
        }

        if (Part.PartType == EVehicleConstructPartType::Chassis)
        {
            ChassisBox->AddChild(VehiclePartWidget);
        }
    }
}