// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/WorkshopWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/WorkshopTaskWidget.h"
#include "Components/WorkshopComponent.h"

void UWorkshopWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn) return;
    const auto WorkshopComponent = NewPawn->FindComponentByClass<UWorkshopComponent>();
    if (!WorkshopComponent) return;
    if (!WorkshopComponent->OnTasksUpdated.IsBoundToObject(this))
    {
        WorkshopComponent->OnTasksUpdated.AddUObject(this, &UWorkshopWidget::OnWorkshopTasksUpdated);
        OnWorkshopTasksUpdated(TArray<FInteractionData>{});
    }
    if (!WorkshopComponent->OnUpdateCost.IsBoundToObject(this))
    {
        WorkshopComponent->OnUpdateCost.AddUObject(this, &UWorkshopWidget::OnUpdateCost);
    }
}

void UWorkshopWidget::OnWorkshopTasksUpdated(const TArray<FInteractionData>& Tasks)
{
    checkf(WorkshopTaskWidgetClass, TEXT("WorkshopTaskWidgetClass not define!"));

    if (!WorkshopTaskSlots || !Tasks.Num())
    {
        GetOwningPlayer()->bShowMouseCursor = false;

        SetVisibility(ESlateVisibility::Collapsed);
        return;
    }

    GetOwningPlayer()->bShowMouseCursor = true;
    SetVisibility(ESlateVisibility::Visible);
    

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