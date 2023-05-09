// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/UI/GameConfigWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "GameMenu/MenuPlayerController.h"
#include "ValkyrieGameInstance.h"
#include "Components/HorizontalBox.h"
#include "GameMenu/UI/LevelItemWidget.h"
#include "GameMenu/UI/VehicleConfigWidget.h"
#include "GameMenu/MenuGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameConfigWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(LevelItemWidgetClass, TEXT("LevelItemWidgetClass not define!"));
    checkf(VehicleConfigWidgetClass, TEXT("VehicleConfigWidgetClass not define!"));

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UGameConfigWidget::OnBackClicked);
    }

    if (BeginPlayButton)
    {
        BeginPlayButton->OnClicked.AddDynamic(this, &UGameConfigWidget::OnBeginPlayClicked);
        UKismetSystemLibrary::IsServer(GetWorld()) ? BeginPlayButton->SetIsEnabled(false) : BeginPlayButton->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (VehicleConfigPosition)
    {
        const auto WidgetToAdd = CreateWidget<UVehicleConfigWidget>(GetOwningPlayer(), VehicleConfigWidgetClass);
        if (WidgetToAdd)
        {
            VehicleConfigPosition->AddChild(WidgetToAdd);
        }
    }

    InitLevelItems();
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
    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

    const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>();
    if (!ValkyrieGameInstance || ValkyrieGameInstance->GetLevelsData().Num() == 0) return;

    for (const auto& LevelData : ValkyrieGameInstance->GetLevelsData())
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

    const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>();
    if (ValkyrieGameInstance)
    {
        ValkyrieGameInstance->SetStartupLevel(Data);
        BeginPlayButton->SetIsEnabled(true);
    }
}
