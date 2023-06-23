// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/VehicleConfigWidget.h"
#include "Components/Border.h"
#include "LobbyMenu/UI/OneTypeUnitsSelectWidget.h"

void UVehicleConfigWidget::SetStartParts()
{
    // make checking
    BodyPartsSelectWidget->SetStartPart();
    ChassisPartsSelectWidget->SetStartPart();
    TurretsSelectWidget->SetStartPart();
    SecondWeaponSelectWidget->SetStartPart();
}

void UVehicleConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(OneTypeUnitsSelectWidgetClass, TEXT("OneTypeUnitsSelectWidgetClass not define!"));

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

    if (TurretsSelectPosition)
    {
        TurretsSelectWidget = CreateWidget<UOneTypeUnitsSelectWidget>(GetWorld(), OneTypeUnitsSelectWidgetClass);
        if (TurretsSelectWidget)
        {
            TurretsSelectPosition->AddChild(TurretsSelectWidget);
        }
    }

    if (SecondWeaponSelectPosition)
    {
        SecondWeaponSelectWidget = CreateWidget<UOneTypeUnitsSelectWidget>(GetWorld(), OneTypeUnitsSelectWidgetClass);
        if (SecondWeaponSelectWidget)
        {
            SecondWeaponSelectPosition->AddChild(SecondWeaponSelectWidget);
        }
    }

    InitVehicleUnits();
}

void UVehicleConfigWidget::InitVehicleUnits()
{
    TArray<FVehicleUnitData*> UntsPtrs;
    VehicleUnitsTable->GetAllRows<FVehicleUnitData>("", UntsPtrs);

    for (auto VehicleUnitPtr : UntsPtrs)
    {
        if (!VehicleUnitPtr) continue;

        if (VehicleUnitPtr->UnitType == EVehicleUnitType::Body && BodyPartsSelectWidget)
        {
            BodyPartsSelectWidget->AddUnit(*VehicleUnitPtr);
        }

        if (VehicleUnitPtr->UnitType == EVehicleUnitType::Chassis && ChassisPartsSelectWidget)
        {
            ChassisPartsSelectWidget->AddUnit(*VehicleUnitPtr);
        }

        if (VehicleUnitPtr->UnitType == EVehicleUnitType::Turret && TurretsSelectWidget)
        {
            TurretsSelectWidget->AddUnit(*VehicleUnitPtr);
        }

        if (VehicleUnitPtr->UnitType == EVehicleUnitType::SecondWeapon && SecondWeaponSelectWidget)
        {
            SecondWeaponSelectWidget->AddUnit(*VehicleUnitPtr);
        }
    }
}
