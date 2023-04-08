// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuGameModeBase.h"
#include "GameMenu/UI/MenuHUD.h"
#include "GameMenu/MenuPlayerController.h"
#include "GameMenu/MenuVehicleActor.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

AMenuGameModeBase::AMenuGameModeBase()
{
    HUDClass = AMenuHUD::StaticClass();
    PlayerControllerClass = AMenuPlayerController::StaticClass();
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

void AMenuGameModeBase::MountVehicleItem(UClass* Class, EVehicleItemType Type)
{
    if (!MenuVehicleActor) return;

    MenuVehicleActor->MountItemOnVehicle(Class, Type);
}

void AMenuGameModeBase::SpawnMenuVehicleActor()
{
    if (!SpawnPositions[0]) return;
    FTransform SpawnTransform = SpawnPositions[0]->GetActorTransform();

    MenuVehicleActor = GetWorld()->SpawnActor<AMenuVehicleActor>(MenuVehicleActorClass, SpawnTransform);
}
