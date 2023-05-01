// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "WorkshopTaskWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class VALKIRIASTRIKE_API UWorkshopTaskWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetTaskData(const FInteractionData& Data);
    const FInteractionData& GetTaskData() const { return TaskData; }
    void UpdateCost(int32 Amount);

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* CompleteTaskButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CostText;

    UPROPERTY(meta = (BindWidget))
    UImage* TaskTumb;

    virtual void NativeOnInitialized() override;

private:
    FInteractionData TaskData;

    UFUNCTION()
    void OnCompleteTask();
};
