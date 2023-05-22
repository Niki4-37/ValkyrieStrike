// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/UI/LobbyHUDWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "ValkyrieGameInstance.h"
#include "Components/HorizontalBox.h"
#include "LobbyMenu/UI/LevelTileWidget.h"
#include "LobbyMenu/UI/VehicleConfigWidget.h"
#include "LobbyMenu/LobbyGameModeBase.h"
#include "LobbyMenu/LobbyPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void ULobbyHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(LevelTileWidgetClass, TEXT("LevelTileWidgetClass not define!"));
    checkf(VehicleConfigWidgetClass, TEXT("VehicleConfigWidgetClass not define!"));

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ULobbyHUDWidget::OnBackClicked);
    }

    if (BeginPlayButton)
    {
        BeginPlayButton->OnClicked.AddDynamic(this, &ULobbyHUDWidget::OnBeginPlayClicked);
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

void ULobbyHUDWidget::OnBackClicked()
{
    if (const auto LobbyPlayerController = Cast<ALobbyPlayerController>(GetOwningPlayer()))
    {
        LobbyPlayerController->SetNewView(EMenuState::MainMenu);

        LobbyPlayerController->GoToMainMenu();
    }
}

void ULobbyHUDWidget::OnBeginPlayClicked()
{
    const auto LobbyGameMode = GetWorld()->GetAuthGameMode<ALobbyGameModeBase>();
    if (!LobbyGameMode) return;

    LobbyGameMode->LaunchGame(GetOwningPlayer());
}

void ULobbyHUDWidget::InitLevelItems()
{
    if (!LevelTilesBox) return;
    LevelTilesBox->ClearChildren();

    if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

    const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>();
    if (!ValkyrieGameInstance || ValkyrieGameInstance->GetLevelsData().Num() == 0) return;

    for (const auto& LevelData : ValkyrieGameInstance->GetLevelsData())
    {
        const auto LevelTileWidget = CreateWidget<ULevelTileWidget>(GetWorld(), LevelTileWidgetClass);

        if (!LevelTileWidget) continue;
        LevelTileWidget->SetLevelData(LevelData);
        LevelTileWidget->SetSelected(false);
        LevelTileWidget->OnLevelSelected.AddUObject(this, &ULobbyHUDWidget::OnLevelSelected);
        LevelTilesBox->AddChild(LevelTileWidget);
        LevelTileWidgets.Add(LevelTileWidget);
    }
}

void ULobbyHUDWidget::OnLevelSelected(const FLevelData& Data)
{
    for (auto LevelItemWidget : LevelTileWidgets)
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
