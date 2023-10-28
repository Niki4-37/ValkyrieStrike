// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "InGameVehicleUnitWidget.generated.h"

class UImage;

UCLASS()
class VALKYRIESTRIKE_API UInGameVehicleUnitWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetUnitData(const FVehicleUnitData& Data);
    void UpdateAmmoCapacityBar(int32 NewValue);
    void StartReloading();
    EVehicleUnitType GetUnitType() const { return UnitType; }

    virtual void BeginDestroy() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* UnitImage;

    UPROPERTY(meta = (BindWidget))
    UImage* CapacityBarImage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BaseColorParamName{"BaseColor"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName PercentParamName{"Percent"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AmmoCapacityParamName{"NumberOfSections"};

private:
    UPROPERTY()
    UMaterialInstanceDynamic* CircleBarMaterial;

    int32 MaxAmmoCapacity;
    float ReloadingTime;
    EVehicleUnitType UnitType;
    FTimerHandle ReloadingTimer;
    FTimerHandle ProgressTimer;

    void EndReloading();
};
