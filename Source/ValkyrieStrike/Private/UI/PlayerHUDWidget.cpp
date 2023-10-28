// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/PlayerHUDWidget.h"
#include "Components/Border.h"
#include "UI/BaseWidget.h"
#include "Components/Button.h"
#include "Player/VehiclePlayerController.h"
#include "Interfaces/GameInterface.h"

#include "Engine.h"

void UPlayerHUDWidget::NativeOnInitialized()
{
    checkf(VehicleEndurenceWidgetClass, TEXT("VehicleEndurenceWidgetClass not define!"));
    checkf(FuelGaugeWidgetClass, TEXT("FuelGaugeWidgetClass not define!"));
    checkf(InGameVehicleConfigWidgetClass, TEXT("InGameVehicleConfigWidgetClass not define!"));
    checkf(CoinsValueWidgetClass, TEXT("CoinsValueWidgetClass not define!"));
    checkf(LivesWidgetClass, TEXT("LivesWidgetClass not define!"));
    checkf(WorkshopWidgetClass, TEXT("WorkshopWidgetClass not define!"));

    if (VehicleEndurancePosition)
    {
        const auto EnduranceWidget = CreateWidget<UBaseWidget>(GetWorld(), VehicleEndurenceWidgetClass);
        if (EnduranceWidget)
        {
            InGameWidgets.Add(EnduranceWidget);
            VehicleEndurancePosition->AddChild(EnduranceWidget);
        }
    }

    if (VehicleFuelPosition)
    {
        const auto FuelGaugeWidget = CreateWidget<UBaseWidget>(GetWorld(), FuelGaugeWidgetClass);
        if (FuelGaugeWidget)
        {
            InGameWidgets.Add(FuelGaugeWidget);
            VehicleFuelPosition->AddChild(FuelGaugeWidget);
        }
    }

    if (VehicleConfigPosition)
    {
        const auto ConfigWidget = CreateWidget<UBaseWidget>(GetWorld(), InGameVehicleConfigWidgetClass);
        if (ConfigWidget)
        {
            InGameWidgets.Add(ConfigWidget);
            VehicleConfigPosition->AddChild(ConfigWidget);
        }
    }

    if (CoinsValuePosition)
    {
        const auto CoinsWidget = CreateWidget<UBaseWidget>(GetWorld(), CoinsValueWidgetClass);
        if (CoinsWidget)
        {
            InGameWidgets.Add(CoinsWidget);
            CoinsValuePosition->AddChild(CoinsWidget);
        }
    }

    if (LivesPosition)
    {
        const auto LivesWidget = CreateWidget<UBaseWidget>(GetWorld(), LivesWidgetClass);
        if (LivesWidget)
        {
            InGameWidgets.Add(LivesWidget);
            LivesPosition->AddChild(LivesWidget);
        }
    }

    if (WorkshopPosition)
    {
        const auto WorkshopWidget = CreateWidget<UBaseWidget>(GetWorld(), WorkshopWidgetClass);
        if (WorkshopWidget)
        {
            InGameWidgets.Add(WorkshopWidget);
            WorkshopPosition->AddChild(WorkshopWidget);
        }
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::OnPauseClicked);
    }

    if (ReverseGearButton)
    {
        ReverseGearButton->OnPressed.AddDynamic(this, &UPlayerHUDWidget::OnReversePressed);
        ReverseGearButton->OnReleased.AddDynamic(this, &UPlayerHUDWidget::OnReverseReleased);
    }

    if (FireButton)
    {
        FireButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::OnFireClicked);
    }

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UPlayerHUDWidget::RemoveFromParent()
{
    if (GetWorld() && GetWorld()->GetTimerManager().TimerExists(FindingPawnTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(FindingPawnTimer);
    }
    Super::RemoveFromParent();
}

void UPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    // add fails counter
    // add check to avoid endless repeat
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            FString Test = GetOwningPlayerPawn() ? GetOwningPlayerPawn()->GetName() : "No Pawn!";
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Test);

            if (GetOwningPlayer()->GetSpectatorPawn())
            {
                GetWorld()->GetTimerManager().ClearTimer(FindingPawnTimer);
                return;
            }

            if (!GetOwningPlayerPawn()) return;

            for (auto& InGameWidget : InGameWidgets)
            {
                if (!InGameWidget) continue;
                InGameWidget->OnNewPawn(GetOwningPlayerPawn());
            }

            GetWorld()->GetTimerManager().ClearTimer(FindingPawnTimer);
        });

    GetWorld()->GetTimerManager().SetTimer(FindingPawnTimer, TimerDelegate, 0.1f, true);
}

void UPlayerHUDWidget::OnPauseClicked()
{
    if (!GetOwningPlayer()) return;
    GetOwningPlayer()->SetPause(true);
}

void UPlayerHUDWidget::OnReversePressed()
{
    const auto PlayerInterface = Cast<IGameInterface>(GetOwningPlayerPawn());
    if (!PlayerInterface) return;

    PlayerInterface->UseReverseGear(true);
}

void UPlayerHUDWidget::OnReverseReleased()
{
    const auto PlayerInterface = Cast<IGameInterface>(GetOwningPlayerPawn());
    if (!PlayerInterface) return;

    PlayerInterface->UseReverseGear(false);
}

void UPlayerHUDWidget::OnFireClicked()
{
    const auto PlayerInterface = Cast<IGameInterface>(GetOwningPlayerPawn());
    if (!PlayerInterface) return;

    PlayerInterface->ShootFromWeapon();
}
