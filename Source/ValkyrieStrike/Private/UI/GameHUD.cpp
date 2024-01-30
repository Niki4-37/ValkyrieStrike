// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "UI/GameHUD.h"
#include "UI/PlayerHUDWidget.h"
#include "Player/VehiclePlayerController.h"

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());

    GameStateWidgetsMap.Add(EValkyrieGameState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameStateWidgetsMap.Add(EValkyrieGameState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass));
    GameStateWidgetsMap.Add(EValkyrieGameState::GameSettings, CreateWidget<UUserWidget>(GetWorld(), SettingsWidgetClass));
    GameStateWidgetsMap.Add(EValkyrieGameState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));
    GameStateWidgetsMap.Add(EValkyrieGameState::Respawn, CreateWidget<UUserWidget>(GetWorld(), RespawnWidgetClass));

    for (TPair<EValkyrieGameState, UUserWidget*> Pair : GameStateWidgetsMap)
    {
        if (!Pair.Value) continue;
        Pair.Value->AddToViewport();
        Pair.Value->SetVisibility(ESlateVisibility::Hidden);
    }

    if (const auto PlayerController = Cast<AVehiclePlayerController>(GetOwningPlayerController()))
    {
        PlayerController->OnGameStateChanged.AddUObject(this, &AGameHUD::OnGameStateChanged);
        PlayerController->ChangeGameState_OnClient(EValkyrieGameState::InProgress);
    }
}

void AGameHUD::Destroyed()
{
    Super::Destroyed();

    for (TPair<EValkyrieGameState, UUserWidget*> Pair : GameStateWidgetsMap)
    {
        if (!Pair.Value) continue;
        Pair.Value->RemoveFromParent();
    }
}

void AGameHUD::OnGameStateChanged(EValkyrieGameState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GameStateWidgetsMap.Contains(State))
    {
        CurrentWidget = GameStateWidgetsMap[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}
