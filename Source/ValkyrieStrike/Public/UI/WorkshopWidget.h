// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "WorkshopWidget.generated.h"

class UUniformGridPanel;
class UWorkshopTaskWidget;

UCLASS()
class VALKYRIESTRIKE_API UWorkshopWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* WorkshopTaskSlots;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> WorkshopTaskWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Clampmin = "1", Clampmax = "5"))
    uint8 SlotsInRow{2};

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TMap<EItemPropertyType, UWorkshopTaskWidget*> TaskMap;

    FTimerHandle FoundPawnTimer;

    void OnNewPawn(APawn* NewPawn);

    void OnWorkshopTasksUpdated(const TArray<FInteractionData>& Tasks);
    void OnUpdateCost(EItemPropertyType Type, int32 Amount);
};
