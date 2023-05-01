// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "FuelGaugeWidget.generated.h"

class UImage;
class UMaterialInstance;

UCLASS()
class VALKIRIASTRIKE_API UFuelGaugeWidget : public UUserWidget
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

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);
    void OnFuelValueChanged(EItemPropertyType Type, float Value, float MaxValue);
};
