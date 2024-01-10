// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Player/VehiclePlayerController.h"
#include "Interfaces/GameInterface.h"
#include "Components/RespawnComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "ValkyrieGameInstance.h"

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

void AVehiclePlayerController::ChangeGameState_OnClient_Implementation(EValkyrieGameState NewState)
{
    ChangeGameState(NewState);
}

void AVehiclePlayerController::CheckMusicTheme_OnClient_Implementation(bool bHasAim)
{
    GetWorldTimerManager().ClearTimer(ChangeMusicTimer);

    if (bHasAim && !BattleMusicComponent->IsPlaying())
    {
        ToggleMusicTheme();
    }
    else if (!bHasAim)
    {
        GetWorldTimerManager().SetTimer(ChangeMusicTimer, this, &AVehiclePlayerController::ToggleMusicTheme, 7.f);
    }
}

void AVehiclePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>())
    {
        auto MusicToPlay = ValkyrieGameInstance->GetGameMusic(EMusicTheme::GameMusicCalm);
        CalmMusicComponent = UGameplayStatics::CreateSound2D(GetWorld(), MusicToPlay, 1.f, 1.f, 0.f, nullptr, false, false);

        MusicToPlay = ValkyrieGameInstance->GetGameMusic(EMusicTheme::GameMusicBattle);
        BattleMusicComponent = UGameplayStatics::CreateSound2D(GetWorld(), MusicToPlay, 1.f, 1.f, 0.f, nullptr, false, false);
    }

    if (CalmMusicComponent && IsLocalPlayerController())
    {
        CalmMusicComponent->FadeIn(0.5f);
    }

    FLinearColor ToColor{FColor::Black};
    PlayerCameraManager->StartCameraFade(1.f, 0.0f, 5.f, ToColor);

    RespawnComponent->OnStartRespawn.AddUObject(this, &AVehiclePlayerController::OnStartRespawn);
}

void AVehiclePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);

    ChangeGameState_OnClient(EValkyrieGameState::InProgress);
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
    bPause ? ChangeGameState_OnClient(EValkyrieGameState::Pause) : ChangeGameState_OnClient(EValkyrieGameState::InProgress);
    // Super::SetPause(bPause, CanUnpauseDelegate);
    return false;
}

void AVehiclePlayerController::OnPauseGame()
{
    ChangeGameState_OnClient(EValkyrieGameState::Pause);
}

void AVehiclePlayerController::ChangeGameState(EValkyrieGameState State)
{
    //if (State == EValkyrieGameState::InProgress)
    //{
    //    // SetInputMode(FInputModeGameOnly());
    //    // bShowMouseCursor = false;
    //    SetInputMode(FInputModeGameAndUI());
    //    bShowMouseCursor = true;
    //}
    //else
    //{
    //    SetInputMode(FInputModeUIOnly());
    //    bShowMouseCursor = true;
    //}
    SetInputMode(FInputModeGameAndUI());
    bShowMouseCursor = true;

    OnGameStateChanged.Broadcast(State);
}

void AVehiclePlayerController::OnStartRespawn(float RespawnDelay)
{
    // handled on server
    ChangeGameState_OnClient(EValkyrieGameState::Respawn);
}

void AVehiclePlayerController::ToggleMusicTheme()
{
    if (CalmMusicComponent->IsPlaying())
    {
        CalmMusicComponent->FadeOut(0.5f, 0.f);
        BattleMusicComponent->FadeIn(0.5f);
        return;
    }

    if (BattleMusicComponent->IsPlaying())
    {
        BattleMusicComponent->FadeOut(0.5f, 0.f);
        CalmMusicComponent->FadeIn(0.5f);
    }
}
