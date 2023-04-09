// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/GameConfigWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "GameMenu/MenuPlayerController.h"
#include "ValkiriaGameInstance.h"
#include "Components/HorizontalBox.h"
#include "GameMenu/UI/LevelItemWidget.h"
#include "GameMenu/UI/VehicleConfigWidget.h"
#include "GameMenu/MenuGameModeBase.h"

void UGameConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UGameConfigWidget::OnBackClicked);
    }

    if (BeginPlayButton)
    {
        BeginPlayButton->OnClicked.AddDynamic(this, &UGameConfigWidget::OnBeginPlayClicked);
        BeginPlayButton->SetIsEnabled(false);
    }

    InitLevelItems();

    if (VehicleConfigPosition)
    {
        const auto WidgetToAdd = CreateWidget<UVehicleConfigWidget>(GetOwningPlayer(), VehicleConfigWidgetClass);
        if (WidgetToAdd)
        {
            VehicleConfigPosition->AddChild(WidgetToAdd);
        }
    }
}

void UGameConfigWidget::OnBackClicked()
{
    if (const auto PlayerController = Cast<AMenuPlayerController>(GetOwningPlayer()))
    {
        PlayerController->SetNewView(EMenuState::MainMenu);
    }
}

void UGameConfigWidget::OnBeginPlayClicked()
{
    const auto MenuGM = GetWorld()->GetAuthGameMode<AMenuGameModeBase>();
    if (!MenuGM) return;

    MenuGM->LaunchGame(GetOwningPlayer());
}

void UGameConfigWidget::InitLevelItems()
{
    checkf(LevelItemWidgetClass, TEXT("LevelItemWidgetClass not define!"));
    checkf(VehicleConfigWidgetClass, TEXT("VehicleConfigWidgetClass not define!"));

    const auto ValkiriaGameInstance = GetGameInstance<UValkiriaGameInstance>();
    if (!ValkiriaGameInstance) return;

    if (!LevelItemsBox || ValkiriaGameInstance->GetLevelsData().Num() == 0) return;
    LevelItemsBox->ClearChildren();

    for (const auto& LevelData : ValkiriaGameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<ULevelItemWidget>(GetWorld(), LevelItemWidgetClass);

        if (!LevelItemWidget) continue;
        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->SetSelected(false);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &UGameConfigWidget::OnLevelSelected);
        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }
}

void UGameConfigWidget::OnLevelSelected(const FLevelData& Data)
{
    for (auto LevelItemWidget : LevelItemWidgets)
    {
        if (!LevelItemWidget) continue;
        const bool bIsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
        LevelItemWidget->SetSelected(bIsSelected);
    }

    const auto ValkiriaGameInstance = GetGameInstance<UValkiriaGameInstance>();
    if (ValkiriaGameInstance)
    {
        ValkiriaGameInstance->SetStartupLevel(Data);
        BeginPlayButton->SetIsEnabled(true);
    }
}
