// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/InGameVehicleConfigWidget.h"
#include "Components/WeaponComponent.h"
#include "Components/HorizontalBox.h"
#include "Weapon/BaseVehicleWeapon.h"
#include "UI/InGameVehicleUnitWidget.h"

void UInGameVehicleConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(InGameVehicleUnitWidgetClass, TEXT("InGameVehicleUnitWidgetClass not define!"));
}

void UInGameVehicleConfigWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto WeaponComponent = NewPawn->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent) return;

    if (!WeaponComponent->OnUnitMount.IsBoundToObject(this))
    {
        WeaponComponent->OnUnitMount.AddUObject(this, &UInGameVehicleConfigWidget::OnUnitMount);
    }

    for (const auto Weapon : WeaponComponent->GetVehicleWeapons())
    {
        if (!Weapon->OnChangeAmmoInWeapon.IsBoundToObject(this))
        {
            Weapon->OnChangeAmmoInWeapon.AddUObject(this, &UInGameVehicleConfigWidget::OnChangeAmmo);
        }
        if (!Weapon->OnStartWeaponReloading.IsBoundToObject(this))
        {
            Weapon->OnStartWeaponReloading.AddUObject(this, &UInGameVehicleConfigWidget::OnStartReloading);
        }
    }

    WeaponComponent->UpdateWidgets();
}

void UInGameVehicleConfigWidget::OnUnitMount(const FVehicleUnitData& Data)
{
    if (!GetWorld()) return;
    const auto UnitWidget = CreateWidget<UInGameVehicleUnitWidget>(GetWorld(), InGameVehicleUnitWidgetClass);
    if (UnitsBox && UnitWidget)
    {
        UnitWidget->SetUnitData(Data);

        for (auto Child : UnitsBox->GetAllChildren())
        {
            const auto VehicleUnitWidget = Cast<UInGameVehicleUnitWidget>(Child);
            if (!VehicleUnitWidget) continue;
            if (VehicleUnitWidget->GetUnitType() != Data.UnitType) continue;
            UnitsBox->RemoveChild(Child);
        }

        UnitsBox->AddChild(UnitWidget);
        UnitsMap.FindOrAdd(Data.UnitType) = UnitWidget;
    }
}

void UInGameVehicleConfigWidget::OnChangeAmmo(EVehicleUnitType Type, int32 NewValue)
{
    if (!UnitsMap.Contains(Type)) return;
    UnitsMap.FindChecked(Type)->UpdateAmmoCapacityBar(NewValue);
}

void UInGameVehicleConfigWidget::OnStartReloading(EVehicleUnitType Type)
{
    if (!UnitsMap.Contains(Type)) return;
    UnitsMap.FindChecked(Type)->StartReloading();
}
