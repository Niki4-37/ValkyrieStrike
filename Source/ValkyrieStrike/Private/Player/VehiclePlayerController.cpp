// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Player/VehiclePlayerController.h"
#include "Interfaces/GameInterface.h"
#include "Components/RespawnComponent.h"


AVehiclePlayerController::AVehiclePlayerController() 
{
    RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnComponent");
}

void AVehiclePlayerController::MakeMaintenance_OnServer_Implementation(EItemPropertyType Type)
{
    const auto PawnInterface = Cast<IGameInterface>(GetPawn());
    if (!PawnInterface) return;

    PawnInterface->MakeMaintenance(Type);
}

void AVehiclePlayerController::ChangeGameState(EValkyrieGameState State)
{
    if (State == EValkyrieGameState::InProgress)
    {
        // SetInputMode(FInputModeGameOnly());
        // bShowMouseCursor = false;
        SetInputMode(FInputModeGameAndUI());
        bShowMouseCursor = true;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }

    OnGameStateChanged.Broadcast(State);
}

void AVehiclePlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AVehiclePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void AVehiclePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent) return;
    InputComponent->BindAction("PauseGame", IE_Pressed, this, &AVehiclePlayerController::OnPauseGame).bExecuteWhenPaused = true;
}

void AVehiclePlayerController::BeginPlayingState()
{
    Super::BeginPlayingState();
}

void AVehiclePlayerController::FailedToSpawnPawn() {}

bool AVehiclePlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate)
{
    bPause ? ChangeGameState(EValkyrieGameState::Pause) : ChangeGameState(EValkyrieGameState::InProgress);
    //Super::SetPause(bPause, CanUnpauseDelegate);
    return false;
}

void AVehiclePlayerController::OnPauseGame()
{
    ChangeGameState(EValkyrieGameState::Pause);
}
