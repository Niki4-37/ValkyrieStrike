// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RespawnWidget.generated.h"

class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class VALKYRIESTRIKE_API URespawnWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* CountdownBarImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BaseColorParamName{"BaseColor"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName PercentParamName{"Percent"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SectionsParamName{"NumberOfSections"};

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    UMaterialInstanceDynamic* CountdownBarMaterial{nullptr};
    FTimerHandle RespawnTimer;
    FTimerHandle ProgressTimer;

    void OnStartRespawn(float RespawnDelay);
    void RespawnEnd();
};
