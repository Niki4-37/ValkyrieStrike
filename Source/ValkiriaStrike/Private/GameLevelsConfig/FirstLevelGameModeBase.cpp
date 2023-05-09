// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/FirstLevelGameModeBase.h"
#include "Player/VehiclePlayerController.h"
#include "GameLevelsConfig/ValkiriaPlayerState.h"
#include "UI/GameHUD.h"
#include "Player/DefaultWeeledVehicle.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

#include "Decorations/DecorationActor.h"

AFirstLevelGameModeBase::AFirstLevelGameModeBase()
{

    PlayerControllerClass = AVehiclePlayerController::StaticClass();
    PlayerStateClass = AValkiriaPlayerState::StaticClass();
    HUDClass = AGameHUD::StaticClass();
    DefaultPawnClass = ADefaultWeeledVehicle::StaticClass();
}

void AFirstLevelGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    FillPlayerStartMap();
}

void AFirstLevelGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
    Super::HandleSeamlessTravelPlayer(C);
}

AActor* AFirstLevelGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    if (Player && Player->IsA<APlayerController>())
    {
        for (TPair<AActor*, bool>& Pair : PlayerStartMap)
        {
            if (const bool bIsOccupied = Pair.Value == true) continue;
            auto FoundEmptyPosition = Pair.Key;
            if (!FoundEmptyPosition) continue;
            Pair.Value = true;
            return FoundEmptyPosition;
        }
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}

void AFirstLevelGameModeBase::RestartPlayer(AController* NewPlayer)
{
    if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
    {
        return;
    }

    const auto ValkiriaPlayerState = NewPlayer->GetPlayerState<AValkiriaPlayerState>();

    if (ValkiriaPlayerState && !ValkiriaPlayerState->IsFirstDead() /*&& PS->IsReconnecting()*/)
    {
        FTransform NewTransforn = ValkiriaPlayerState->GetRespawnTransform();
        FVector NewLocation = NewTransforn.GetLocation() + FVector(300.f, 300.f, 100.f);
        NewTransforn.SetLocation(NewLocation);
        RestartPlayerAtTransform(NewPlayer, NewTransforn);
    }
    else
    {
        AActor* StartSpot = FindPlayerStart(NewPlayer);

        if (StartSpot == nullptr)
        {
            if (NewPlayer->StartSpot != nullptr)
            {
                StartSpot = NewPlayer->StartSpot.Get();
                UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
            }
        }

        RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
    }
}

void AFirstLevelGameModeBase::Killed(AController* VictimController, const FTransform& VictimTransform)
{
    if (VictimController && VictimController->IsA<APlayerController>())
    {
        auto BrokenVehicle = GetWorld()->SpawnActorDeferred<ADecorationActor>(ADecorationActor::StaticClass(), VictimTransform);
        if (BrokenVehicle)
        {
            BrokenVehicle->SetupDecoration(BrokenVehicleMesh);
            BrokenVehicle->FinishSpawning(VictimTransform);
        }

        if (const auto ValkiriaPlayerState = VictimController->GetPlayerState<AValkiriaPlayerState>())
        {
            ValkiriaPlayerState->SetRespawnTransform(VictimTransform);
            ValkiriaPlayerState->ChangeLives(-1);
            VictimController->GetPawn()->Reset();
            if (ValkiriaPlayerState->CanRespawn())
            {
                RestartPlayer(VictimController);
            }
        }
    }
}

void AFirstLevelGameModeBase::FillPlayerStartMap()
{
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* Start = *It;
        const auto StartActor = Cast<AActor>(Start);
        if (!StartActor) continue;
        PlayerStartMap.Add(StartActor, false);
    }
}