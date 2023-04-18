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

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UInGameVehicleConfigWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }

    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "InGameVehicleConfigWidget: NativeOnInitialized");
}

void UInGameVehicleConfigWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto WeaponComponent = NewPawn->FindComponentByClass<UWeaponComponent>();
    if (WeaponComponent && !WeaponComponent->OnItemMount.IsBoundToObject(this))
    {
        WeaponComponent->OnItemMount.AddUObject(this, &UInGameVehicleConfigWidget::OnItemMount);
    }

    if (WeaponComponent && !WeaponComponent->OnWeaponMakeShot.IsBoundToObject(this))
    {
        WeaponComponent->OnWeaponMakeShot.AddUObject(this, &UInGameVehicleConfigWidget::OnWeaponMakeShot);
    }
}

void UInGameVehicleConfigWidget::OnItemMount(const FVehicleItemData& Data)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Data.ItemName.ToString());

    const auto ItemWidget = CreateWidget<UInGameVehicleItemWidget>(GetWorld(), InGameVehicleItemWidgetClass);
    if (ItemsBox && ItemWidget)
    {
        ItemWidget->SetItemData(Data);
        ItemsBox->AddChild(ItemWidget);
        ItemsMap.FindOrAdd(Data.ItemType, ItemWidget);
    }
}

void UInGameVehicleConfigWidget::OnWeaponMakeShot(EVehicleItemType Type, int32 NewValue)
{
    ItemsMap.FindChecked(Type)->UpdateAmmoCapacityBar(NewValue);
}
