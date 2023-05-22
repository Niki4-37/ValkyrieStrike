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
        SpawningMap.Add(FoundActor, nullptr);
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

    if (const auto FoundEmptyPosition = *SpawningMap.FindKey(nullptr))
    {
        SpawningMap.Add(FoundEmptyPosition, NewPlayer);
        SpawnLobbyVehicle(FoundEmptyPosition, NewPlayer);
    }
}

void ALobbyGameModeBase::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (const auto PlayerController = Cast<APlayerController>(Exiting))
    {
        if (const auto FoundPosition = *SpawningMap.FindKey(PlayerController))
        {
            SpawningMap.Add(FoundPosition, nullptr);
        }
    }
}

void ALobbyGameModeBase::SpawnLobbyVehicle(APlayerStart* Position, APlayerController* Controller)
{
    if (!Position || !Controller) return;

    FTransform SpawnTransform = Position->GetActorTransform();
    auto VehicleActor = GetWorld()->SpawnActorDeferred<ADummyVehicle>(DummyVehicleClass, SpawnTransform);
    if (!VehicleActor)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, "Can't find Empty Position");
        return;
    }
    VehicleActor->SetupDecoration(DummyVehicleMesh);
    VehicleActor->FinishSpawning(SpawnTransform);

    const auto LobbyPlayerController = Cast<ALobbyPlayerController>(Controller);
    if (LobbyPlayerController)
    {
        LobbyPlayerController->SetLobbyVehicle(VehicleActor);
    }
}
