// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/InGameVehicleConfigWidget.h"
#include "Components/WeaponComponent.h"
#include "Components/VerticalBox.h"
#include "UI/InGameVehicleItemWidget.h"

#include "Engine.h"

void UInGameVehicleConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(InGameVehicleItemWidgetClass, TEXT("InGameVehicleItemWidgetClass not define!"));
}

void UInGameVehicleConfigWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto WeaponComponent = NewPawn->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent) return;

    if (!WeaponComponent->OnItemMount.IsBoundToObject(this))
    {
        WeaponComponent->OnItemMount.AddUObject(this, &UInGameVehicleConfigWidget::OnItemMount);
    }

    if (!WeaponComponent->OnChangeAmmo.IsBoundToObject(this))
    {
        WeaponComponent->OnChangeAmmo.AddUObject(this, &UInGameVehicleConfigWidget::OnChangeAmmo);
    }

    if (!WeaponComponent->OnStartReloading.IsBoundToObject(this))
    {
        WeaponComponent->OnStartReloading.AddUObject(this, &UInGameVehicleConfigWidget::OnStartReloading);
    }

    WeaponComponent->UpdateWidgets();
}

void UInGameVehicleConfigWidget::OnItemMount(const FVehicleItemData& Data)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, Data.ItemName.ToString());

    const auto ItemWidget = CreateWidget<UInGameVehicleItemWidget>(GetWorld(), InGameVehicleItemWidgetClass);
    if (ItemsBox && ItemWidget)
    {
        ItemWidget->SetItemData(Data);

        for (auto Child : ItemsBox->GetAllChildren())
        {
            const auto VehicleItemWidget = Cast<UInGameVehicleItemWidget>(Child);
            if (!VehicleItemWidget) continue;
            if (VehicleItemWidget->GetItemType() != Data.ItemType) continue;
            ItemsBox->RemoveChild(Child);
        }

        ItemsBox->AddChild(ItemWidget);
        ItemsMap.FindOrAdd(Data.ItemType) = ItemWidget;
    }
}

void UInGameVehicleConfigWidget::OnChangeAmmo(EVehicleItemType Type, int32 NewValue)
{
    if (!ItemsMap.Contains(Type)) return;
    ItemsMap.FindChecked(Type)->UpdateAmmoCapacityBar(NewValue);
}

void UInGameVehicleConfigWidget::OnStartReloading(EVehicleItemType Type)
{
    if (!ItemsMap.Contains(Type)) return;
    ItemsMap.FindChecked(Type)->StartReloading();
}
