// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/VehicleConfigWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "LobbyMenu/UI/VehicleUnitWidget.h"
#include "LobbyMenu/UI/OneTypeUnitsSelectWidget.h"

void UVehicleConfigWidget::SetStartParts()
{
    if (BodyPartsSelectWidget)
    {
        BodyPartsSelectWidget->SetStartPart();
    }
}

void UVehicleConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(VehicleUnitWidgetClass, TEXT("VehicleUnitWidgetClass not define!"));

    if (BodyPartsSelectPosition)
    {
        BodyPartsSelectWidget = CreateWidget<UOneTypeUnitsSelectWidget>(GetWorld(), OneTypeUnitsSelectWidgetClass);
        if (BodyPartsSelectWidget)
        {
            BodyPartsSelectPosition->AddChild(BodyPartsSelectWidget);
        }
    }

    if (ChassisPartsSelectPosition)
    {
        ChassisPartsSelectWidget = CreateWidget<UOneTypeUnitsSelectWidget>(GetWorld(), OneTypeUnitsSelectWidgetClass);
        if (ChassisPartsSelectWidget)
        {
            ChassisPartsSelectPosition->AddChild(ChassisPartsSelectWidget);
        }
    }

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
        const auto VehicleUnitWidget = CreateWidget<UVehicleUnitWidget>(GetWorld(), VehicleUnitWidgetClass);
        if (!VehicleUnitWidget) continue;
        VehicleUnitWidget->SetUnitData(VehicleItem);

        if (VehicleItem.ItemType == EVehicleItemType::Turret)
        {
            TurretBox->AddChild(VehicleUnitWidget);
        }

        if (VehicleItem.ItemType == EVehicleItemType::SecondWeapon)
        {
            SecondWeaponBox->AddChild(VehicleUnitWidget);
        }
    }
}

void UVehicleConfigWidget::InitVehicleConstructParts()
{
    TArray<FVehicleConstructPart*> PartsPtrs;
    VehicleConstructPartsTable->GetAllRows<FVehicleConstructPart>("", PartsPtrs);

    for (auto VehiclePartPtr : PartsPtrs)
    {
        if (!VehiclePartPtr) continue;
        const auto Part = *VehiclePartPtr;

        if (Part.PartType == EVehicleConstructPartType::Body && BodyPartsSelectWidget)
        {
            BodyPartsSelectWidget->AddUnit(Part);
        }

        if (Part.PartType == EVehicleConstructPartType::Chassis && ChassisPartsSelectWidget)
        {
            ChassisPartsSelectWidget->AddUnit(Part);
        }
    }
}