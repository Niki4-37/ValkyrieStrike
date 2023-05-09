// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/WorkshopWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/WorkshopTaskWidget.h"
#include "Components/WorkshopComponent.h"

void UWorkshopWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UWorkshopWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UWorkshopWidget::OnNewPawn(APawn* NewPawn)
{
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda(
        [&]()
        {
            if (!GetOwningPlayerPawn()) return;

            const auto WorkshopComponent = GetOwningPlayerPawn()->FindComponentByClass<UWorkshopComponent>();
            if (!WorkshopComponent) return;

            WorkshopComponent->OnTasksUpdated.AddUObject(this, &UWorkshopWidget::OnWorkshopTasksUpdated);
            WorkshopComponent->OnUpdateCost.AddUObject(this, &UWorkshopWidget::OnUpdateCost);

            GetWorld()->GetTimerManager().ClearTimer(FoundPawnTimer);
        });
    GetWorld()->GetTimerManager().SetTimer(FoundPawnTimer, TimerDelegate, 0.1f, true);
}

void UWorkshopWidget::OnWorkshopTasksUpdated(const TArray<FInteractionData>& Tasks)
{
    checkf(WorkshopTaskWidgetClass, TEXT("WorkshopTaskWidgetClass not define!"));

    if (!WorkshopTaskSlots) return;
    WorkshopTaskSlots->ClearChildren();
    TaskMap.Empty();

    uint8 SlotIndex{0};
    for (auto& Task : Tasks)
    {
        if (Task.Type == EItemPropertyType::Money || Task.Type == EItemPropertyType::NoType) continue;
        const auto NewTask = CreateWidget<UWorkshopTaskWidget>(GetWorld(), WorkshopTaskWidgetClass);
        if (!NewTask) continue;
        TaskMap.Add(Task.Type, NewTask);
        NewTask->SetTaskData(Task);
        auto GridObject = WorkshopTaskSlots->AddChildToUniformGrid(NewTask, SlotIndex / SlotsInRow, SlotIndex % SlotsInRow);
        if (!GridObject) continue;
        GridObject->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
        GridObject->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
        ++SlotIndex;
    }
}

void UWorkshopWidget::OnUpdateCost(EItemPropertyType Type, int32 Amount)
{
    if (TaskMap.Contains(Type))
    {
        TaskMap.FindChecked(Type)->UpdateCost(Amount);
    }
}