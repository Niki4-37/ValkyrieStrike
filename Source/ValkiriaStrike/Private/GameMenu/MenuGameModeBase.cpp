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
        SpawnPositions.Add(FoundActor);
    }
}

void AMenuGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnMenuVehicleActor();
}

void AMenuGameModeBase::MountVehicleItem(UClass* Class, EVehicleItemType Type, APlayerController* PC)
{
    if (MenuVehicleActor)
    {
        MenuVehicleActor->MountItemOnVehicle(Class, Type);
    }
    if (const auto MenuPC = Cast<AMenuPlayerController>(PC))
    {
        if (const auto ValkiriaPlayerState = MenuPC->GetPlayerState<AValkiriaPlayerState>())
        {
            ValkiriaPlayerState->SaveMountedItem(Class, Type);
        }
    }
}

void AMenuGameModeBase::LaunchGame(APlayerController* PC)
{
    const auto ValkiriaGameInstance = GetGameInstance<UValkiriaGameInstance>();
    if (!ValkiriaGameInstance) return;
    const FString SelectedLevelName = "servertravel " + ValkiriaGameInstance->GetStartupLevel().LevelName.ToString();
    UKismetSystemLibrary::ExecuteConsoleCommand(this, SelectedLevelName, PC);
}

void AMenuGameModeBase::SpawnMenuVehicleActor()
{
    if (!SpawnPositions[0]) return;
    FTransform SpawnTransform = SpawnPositions[0]->GetActorTransform();

    MenuVehicleActor = GetWorld()->SpawnActor<AMenuVehicleActor>(MenuVehicleActorClass, SpawnTransform);
}
