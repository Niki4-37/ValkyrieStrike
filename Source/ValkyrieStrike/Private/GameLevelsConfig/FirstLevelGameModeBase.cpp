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
#include "BrainComponent.h"

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

    for (TPair<TSubclassOf<AActor>, float>& SpawnClassesWithChance : EnemySpawnClassesWithChance)
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
            if (/*const bool bIsOccupied = */ Pair.Value == true) continue;
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
        for (TPair<TSubclassOf<AActor>, float>& SpawnClassesWithChance : EnemySpawnClassesWithChance)
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

    if (const auto ValkyriePlayerState = VictimController->GetPlayerState<AValkyriePlayerState>())
    {
        ValkyriePlayerState->ChangeLives(-1);
        if (!ValkyriePlayerState->CanRespawn())
        {
            VictimController->GameHasEnded();
            return;
        }
        ValkyriePlayerState->SetRespawnTransform(VictimTransform);
    }

    StartRespawning(VictimController);

    if (VictimController->IsA<AAIController>() && bIsFinal)
    {
        ++Count;
        if (Count < EnemiesToWin) return;
        GameOver();
    }
}

void AFirstLevelGameModeBase::FillPlayerStartMap()
{
    if (!GetWorld()) return;
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
    if (!GetWorld()) return;
    for (TActorIterator<ASpawningActor> It(GetWorld()); It; ++It)
    {
        ASpawningActor* SpawningActor = *It;
        if (!SpawningActor) continue;
        SpawningActor->ActorHasTag(PlayerSpawnerTagName)  //
            ?
            PlayerSpawners.AddUnique(SpawningActor)  //
            :
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

    if (!ValkyriePlayerState->IsFirstDead() && PlayerSpawners.Num() /*&& PS->IsReconnecting()*/)
    {
        if (NewPlayer->GetPawn())
        {
            NewPlayer->GetPawn()->Reset();
        }

        FTransform VictimTransform = ValkyriePlayerState->GetRespawnTransform();
        FVector NewLocation;
        GetBestSpawnLocation(VictimTransform.GetLocation(), NewLocation);
        FRotator NewRotation = FRotator(0.f, VictimTransform.Rotator().Yaw, 0.f);
        VictimTransform.SetLocation(NewLocation);
        VictimTransform.SetRotation(FQuat(NewRotation));
        RestartPlayerAtTransform(NewPlayer, VictimTransform);
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
    if (!NewPlayer->IsA<AAIController>() || !bIsFinal) return;

    NewPlayer->UnPossess();

    RestartPlayerAtPlayerStart(NewPlayer, GetRandomSpawningActorByTag(FinalPhaseTagName));
}

AActor* AFirstLevelGameModeBase::GetRandomSpawningActorByTag(FName TagName)
{
    if (!SpawningActors.Num()) return nullptr;
    TArray<int32> IndexesByTag;
    int32 IndexCount{0};
    for (const auto SpawningActor : SpawningActors)
    {
        if (SpawningActor->ActorHasTag(TagName))
        {
            IndexesByTag.Add(IndexCount);
        }
        ++IndexCount;
    }
    return IndexesByTag.Num() != 0 ? SpawningActors[IndexesByTag[FMath::RandHelper(IndexesByTag.Num())]] : nullptr;
}

void AFirstLevelGameModeBase::GameOver()
{
    if (!GetWorld()) return;
    for (FConstControllerIterator ControlletIterator = GetWorld()->GetControllerIterator(); ControlletIterator; ++ControlletIterator)
    {
        if (auto Controller = ControlletIterator->Get())
        {
            Controller->GameHasEnded();
        }
    }

    // for (auto Pawn : TActorRange<APawn>(GetWorld()))
    //{
    //     if (Pawn)
    //     {
    //         Pawn->TurnOff();
    //         Pawn->DisableInput(nullptr);
    //     }
    // }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto AIController = Cast<AAIController>(It->Get());
        if (!AIController || !AIController->BrainComponent) continue;
        AIController->BrainComponent->Cleanup();

        if (const auto RespawnComponentn = AIController->FindComponentByClass<URespawnComponent>())
        {
            RespawnComponentn->UndoRespawn();
        }
    }
}

bool AFirstLevelGameModeBase::GetBestSpawnLocation(const FVector& VictimLocation, FVector& BestLocation)
{
    float BestDistance = MAX_FLT;
    for (const auto PlayerSpawner : PlayerSpawners)
    {
        const auto DistanceBetween = (PlayerSpawner->GetActorLocation() - VictimLocation).Size();
        if (DistanceBetween > BestDistance) continue;
        BestDistance = DistanceBetween;
        BestLocation = PlayerSpawner->GetActorLocation();
    }
    return true;
}
