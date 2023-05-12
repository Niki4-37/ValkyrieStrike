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
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            FString Test = GetOwningPlayerPawn() ? GetOwningPlayerPawn()->GetName() : "No Pawn!";
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Test);

            if (!GetOwningPlayerPawn()) return;

            const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UWeaponComponent>();
            if (!WeaponComponent)
            {
                GetWorld()->GetTimerManager().ClearTimer(FoundPawnTimer);
                return;
            }

            if (!WeaponComponent->OnItemMount.IsBoundToObject(this))
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, "IsBoundToObject");
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

            GetWorld()->GetTimerManager().ClearTimer(FoundPawnTimer);
        });

    GetWorld()->GetTimerManager().SetTimer(FoundPawnTimer, TimerDelegate, 0.1f, true);
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
