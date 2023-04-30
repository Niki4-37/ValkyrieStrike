// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/WorkshopWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Player/DefaultWeeledVehicle.h"
#include "UI/WorkshopTaskWidget.h"

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
    if (!NewPawn) return;

    const auto Vehicle = Cast<ADefaultWeeledVehicle>(NewPawn);
    if (Vehicle && !Vehicle->OnWorkshopTasksUpdated.IsBoundToObject(this))
    {
        Vehicle->OnWorkshopTasksUpdated.AddUObject(this, &UWorkshopWidget::OnWorkshopTasksUpdated);
    }
}

void UWorkshopWidget::OnWorkshopTasksUpdated(const TArray<FInteractionData>& Tasks)
{
    checkf(WorkshopTaskWidgetClass, TEXT("WorkshopTaskWidgetClass not define!"));

    if (!WorkshopTaskSlots) return;
    WorkshopTaskSlots->ClearChildren();

    uint8 SlotIndex{0};
    for (auto& Task : Tasks)
    {
        if (Task.Type == EItemPropertyType::Money || Task.Type == EItemPropertyType::NoType) continue;
        const auto NewTask = CreateWidget<UWorkshopTaskWidget>(GetWorld(), WorkshopTaskWidgetClass);
        if (!NewTask) continue;
        NewTask->SetTaskData(Task);
        auto GridObject = WorkshopTaskSlots->AddChildToUniformGrid(NewTask, SlotIndex / SlotsInRow, SlotIndex % SlotsInRow);
        if (!GridObject) continue;
        GridObject->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
        GridObject->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
        ++SlotIndex;
    }
}