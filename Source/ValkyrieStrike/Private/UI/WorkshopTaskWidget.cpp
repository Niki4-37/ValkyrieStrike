// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/WorkshopTaskWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player/VehiclePlayerController.h"

void UWorkshopTaskWidget::SetTaskData(const FInteractionData& Data)
{
    TaskData = Data;

    if (CostText)
    {
        CostText->SetText(FText::AsNumber(TaskData.Amount));
    }

    if (TaskTumb)
    {
        TaskTumb->SetBrushFromSoftTexture(TaskData.ItemTumb);
        TaskTumb->SetBrushSize(FVector2D(200.f));
    }
}

void UWorkshopTaskWidget::UpdateCost(int32 Amount)
{
    if (CostText)
    {
        CostText->SetText(FText::AsNumber(Amount * TaskData.Amount));
    }
}

void UWorkshopTaskWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (CompleteTaskButton)
    {
        CompleteTaskButton->OnClicked.AddDynamic(this, &UWorkshopTaskWidget::OnCompleteTask);
    }
}

void UWorkshopTaskWidget::OnCompleteTask()
{
    const auto VehiclePlayerController = Cast<AVehiclePlayerController>(GetOwningPlayer());
    if (!VehiclePlayerController) return;
    VehiclePlayerController->MakeMaintenance_OnServer(TaskData.Type);
}
