// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCoreTypes.h"
#include "LobbyHUDWidget.generated.h"

class UButton;
class UHorizontalBox;
class UBorder;
class ULevelTileWidget;

UCLASS()
class VALKYRIESTRIKE_API ULobbyHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetStartParts();

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BeginPlayButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelTilesBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> LevelTileWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UBorder* VehicleConfigPosition;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> VehicleConfigWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<ULevelTileWidget*> LevelTileWidgets;

    UFUNCTION()
    void OnBackClicked();

    UFUNCTION()
    void OnBeginPlayClicked();

    void InitLevelItems();
    void OnLevelSelected(const FLevelData& Data);
};
