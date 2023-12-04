// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/FirstLevelGameModeBase.h"
#include "Player/VehiclePlayerController.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"
#include "UI/GameHUD.h"
#include "Player/ModularVehicleBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "AIController.h"
#include "Interactables/SpawningActor.h"
#include "Decorations/DecorationActor.h"
#include "Components/RespawnComponent.h"

#include "Engine.h"

AFirstLevelGameModeBase::AFirstLevelGameModeBase()
{
    PlayerControllerClass = AVehiclePlayerController::StaticClass();
    PlayerStateClass = AValkyriePlayerState::StaticClass();
    HUDClass = AGameHUD::StaticClass();
    DefaultPawnClass = AModularVehicleBase::StaticClass();
}

void AFirstLevelGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    FillPlayerStartMap();
    FillSpawningActorsArray();

    for (TPair<TSubclassOf<AActor>, float>& SpawnClassesWithChance : EnemieSpawnClassesWithChance)
    {
        SumOfWeights += SpawnClassesWithChance.Value;
    }
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

UClass* AFirstLevelGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (const auto ValkyriePlayerState = InController->GetPlayerState<AValkyriePlayerState>())
    {
        const auto SpawnClassPtr = ValkyriePlayerState->GetVehicleUnits().FindByPredicate([](const FVehicleUnitData& Data) { return Data.UnitType == EVehicleUnitType::Chassis; });
        if (SpawnClassPtr && SpawnClassPtr->UnitSpawnClass)
        {
            return SpawnClassPtr->UnitSpawnClass;
        }
    }

    if (InController->IsA<AAIController>())
    {
        const auto Chance = FMath::RandRange(0.f, SumOfWeights);
        float CumulativeWeights{0};
        for (TPair<TSubclassOf<AActor>, float>& SpawnClassesWithChance : EnemieSpawnClassesWithChance)
        {
            CumulativeWeights += SpawnClassesWithChance.Value;
            if (Chance < CumulativeWeights)
            {
                return SpawnClassesWithChance.Key;
            }
        }
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AFirstLevelGameModeBase::RestartPlayer(AController* NewPlayer)
{
    if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
    {
        return;
    }

    RestartPlayerWithPlayerState(NewPlayer);
    RestartPlayerWithAIController(NewPlayer);
}

void AFirstLevelGameModeBase::Killed(AController* VictimController, const FTransform& VictimTransform)
{
    if (!VictimController) return;

    StartRespawning(VictimController);

    if (const auto ValkyriePlayerState = VictimController->GetPlayerState<AValkyriePlayerState>())
    {
        ValkyriePlayerState->SetRespawnTransform(VictimTransform);
        ValkyriePlayerState->ChangeLives(-1);
    }

    if (VictimController->IsA<AAIController>() && bIsFinal)
    {
        ++Count;
        // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("EnemiesKilled: %i"), Count));
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

void AFirstLevelGameModeBase::FillSpawningActorsArray()
{
    for (TActorIterator<ASpawningActor> It(GetWorld()); It; ++It)
    {
        ASpawningActor* SpawningActor = *It;
        if (!SpawningActor) continue;
        SpawningActors.AddUnique(SpawningActor);
    }
}

void AFirstLevelGameModeBase::StartRespawning(AController* VictimController)
{
    // check AIControllers number

    const auto RespawnComponent = VictimController->FindComponentByClass<URespawnComponent>();
    if (!RespawnComponent) return;

    RespawnComponent->StartRespawning(3.f);
}

void AFirstLevelGameModeBase::RestartPlayerWithPlayerState(AController* NewPlayer)
{
    const auto ValkyriePlayerState = NewPlayer->GetPlayerState<AValkyriePlayerState>();
    if (!ValkyriePlayerState) return;

    //  const auto BrokenVehicle = GetWorld()->SpawnActorDeferred<ADecorationActor>(ADecorationActor::StaticClass(), VictimTransform);
    //  if (BrokenVehicle)
    //{
    //      BrokenVehicle->SetupDecoration(BrokenVehicleMesh);
    //      BrokenVehicle->FinishSpawning(VictimTransform);
    //}

    if (!ValkyriePlayerState->IsFirstDead() /*&& PS->IsReconnecting()*/)
    {
        NewPlayer->GetPawn()->Reset();
        if (!ValkyriePlayerState->CanRespawn())
        {
            // spectate
            return;
        }

        FTransform NewTransforn = ValkyriePlayerState->GetRespawnTransform();
        FVector NewLocation = NewTransforn.GetLocation() + FVector(300.f, 300.f, 100.f);
        FRotator NewRotation = FRotator(0.f, NewTransforn.Rotator().Yaw, 0.f);
        NewTransforn.SetLocation(NewLocation);
        NewTransforn.SetRotation(FQuat(NewRotation));
        RestartPlayerAtTransform(NewPlayer, NewTransforn);
    }
    else
    {
        AActor* StartSpot = FindPlayerStart(NewPlayer);
        if (!StartSpot)
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

void AFirstLevelGameModeBase::RestartPlayerWithAIController(AController* NewPlayer)
{
    if (!NewPlayer->IsA<AAIController>() || !SpawningActors.Num() /*|| !bIsFinal*/) return;

    NewPlayer->UnPossess();

    const auto RandomIndex = FMath::RandHelper(SpawningActors.Num());
    const auto StartSpot = SpawningActors[RandomIndex];

    RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}
