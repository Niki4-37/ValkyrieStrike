// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "WorkshopWidget.generated.h"

class UUniformGridPanel;

UCLASS()
class VALKIRIASTRIKE_API UWorkshopWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void UpdateCost(EItemPropertyType Type, int32 Cost);

protected:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* WorkshopTaskSlots;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> WorkshopTaskWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Clampmin = "1", Clampmax = "5"))
    uint8 SlotsInRow{2};

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);

    void OnWorkshopTasksUpdated(const TArray<FInteractionData>& Tasks);
};
