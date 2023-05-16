// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/LobbyGameModeBase.h"
#include "LobbyMenu/LobbyPlayerController.h"
#include "LobbyMenu/UI/LobbyHUD.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"
#include "ValkyrieGameInstance.h"
#include "Decorations/DummyVehicle.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

ALobbyGameModeBase::ALobbyGameModeBase()
{
    HUDClass = ALobbyHUD::StaticClass();
    PlayerControllerClass = ALobbyPlayerController::StaticClass();
    PlayerStateClass = AValkyriePlayerState::StaticClass();

    bUseSeamlessTravel = true;
}

void ALobbyGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    checkf(DummyVehicleClass, TEXT("DummyVehicleClass not define!"));

    for (const auto& FoundActor : TActorRange<APlayerStart>(GetWorld()))
    {
        SpawnPositionsMap.Add(FoundActor, false);
    }
}

void ALobbyGameModeBase::LaunchGame(APlayerController* PC)
{
    const auto ValkyrieGameInstance = GetGameInstance<UValkyrieGameInstance>();
    if (!ValkyrieGameInstance) return;
    FString TravelURL = ValkyrieGameInstance->GetStartupLevel().LevelName.ToString() + "?listen";
    GetWorld()->ServerTravel(TravelURL /*, false*/);
}

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    const auto MenuPlayerController = Cast<ALobbyPlayerController>(NewPlayer);
    if (MenuPlayerController)
    {
        Controllers.Add(MenuPlayerController);
        SpawnLobbyVehicle(MenuPlayerController);
    }
}

void ALobbyGameModeBase::SpawnLobbyVehicle(ALobbyPlayerController* Controller)
{
    if (!Controller) return;

    bool bCanSpawnVehicleActor{false};
    for (TPair<APlayerStart*, bool>& Pair : SpawnPositionsMap)
    {
        if (const bool bIsOccupied = Pair.Value == true) continue;
        auto FoundEmptyPosition = Pair.Key;
        if (!FoundEmptyPosition) continue;
        FTransform SpawnTransform = FoundEmptyPosition->GetActorTransform();
        auto VehicleActor = GetWorld()->SpawnActorDeferred<ADummyVehicle>(DummyVehicleClass, SpawnTransform);
        if (!VehicleActor) continue;
        VehicleActor->SetupDecoration(DummyVehicleMesh);
        VehicleActor->FinishSpawning(SpawnTransform);
        Controller->SetLobbyVehicle(VehicleActor);
        bCanSpawnVehicleActor = true;
        Pair.Value = true;
        break;
    }
    if (!bCanSpawnVehicleActor)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Can't find Empty Position");
    }
}
