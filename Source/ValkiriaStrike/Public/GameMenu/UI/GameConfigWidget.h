// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "GameConfigWidget.generated.h"

class UButton;
class UHorizontalBox;
class ULevelItemWidget;
class UBorder;

UCLASS()
class VALKIRIASTRIKE_API UGameConfigWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleConfigPosition;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleConfigWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<ULevelItemWidget*> LevelItemWidgets;

    UFUNCTION()
    void OnBackClicked();

    void InitLevelItems();
    void OnLevelSelected(const FLevelData& Data);
};
