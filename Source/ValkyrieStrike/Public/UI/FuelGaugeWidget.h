// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "GameCoreTypes.h"
#include "FuelGaugeWidget.generated.h"

class UImage;
class UMaterialInstance;

UCLASS()
class VALKYRIESTRIKE_API UFuelGaugeWidget : public UBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* FuelGaugeImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName NeedlePositionParamName{"NeedlePosition"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RightBorder{-0.17f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LeftBorder{0.17f};

private:
    virtual void OnNewPawn(APawn* NewPawn) override;
    void OnFuelValueChanged(EItemPropertyType Type, float Value, float MaxValue);
};
