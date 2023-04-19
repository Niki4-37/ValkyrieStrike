// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuGameModeBase.h"
#include "GameMenu/UI/MenuHUD.h"
#include "GameMenu/MenuPlayerController.h"
#include "GameMenu/MenuVehicleActor.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameLevelsConfig/ValkiriaPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ValkiriaGameInstance.h"

#include "Engine.h"

AMenuGameModeBase::AMenuGameModeBase()
{
    HUDClass = AMenuHUD::StaticClass();
    PlayerControllerClass = AMenuPlayerController::StaticClass();
    PlayerStateClass = AValkiriaPlayerState::StaticClass();

    bUseSeamlessTravel = true;
}

void AMenuGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    checkf(MenuVehicleActorClass, TEXT("MenuVehicleActorClass not define!"));

    for (const auto& FoundActor : TActorRange<APlayerStart>(GetWorld()))
    {
        SpawnPositionsMap.Add(FoundActor, false);
    }
}

void AMenuGameModeBase::StartPlay()
{
    Super::StartPlay();
}

void AMenuGameModeBase::LaunchGame(APlayerController* PC)
{
    const auto ValkiriaGameInstance = GetGameInstance<UValkiriaGameInstance>();
    if (!ValkiriaGameInstance) return;
    const FString SelectedLevelName = "servertravel " + ValkiriaGameInstance->GetStartupLevel().LevelName.ToString();
    UKismetSystemLibrary::ExecuteConsoleCommand(this, SelectedLevelName, PC);
}

void AMenuGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    const auto MenuPlayerController = Cast<AMenuPlayerController>(NewPlayer);
    if (MenuPlayerController)
    {
        Controllers.Add(MenuPlayerController);
        SpawnMenuVehicleActor(MenuPlayerController);
    }

    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::FromInt(Controllers.Num()));
}

void AMenuGameModeBase::SpawnMenuVehicleActor(AMenuPlayerController* Controller)
{
    if (!Controller) return;

    bool bCanSpawnVehicleActor{false};
    for (TPair<APlayerStart*, bool>& Pair : SpawnPositionsMap)
    {
        if (const bool bIsOccupied = Pair.Value == true) continue;
        auto FoundEmptyPosition = Pair.Key;
        if (!FoundEmptyPosition) continue;
        FTransform SpawnTransform = FoundEmptyPosition->GetActorTransform();
        auto VehicleActor = GetWorld()->SpawnActor<AMenuVehicleActor>(MenuVehicleActorClass, SpawnTransform);
        Controller->SetMenuVehicleActor(VehicleActor);
        bCanSpawnVehicleActor = true;
        Pair.Value = true;
        break;
    }
    if (!bCanSpawnVehicleActor)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Can't find Empty Position");
    }
}
