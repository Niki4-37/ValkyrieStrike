// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Player/VehiclePlayerController.h"
#include "Interfaces/GameInterface.h"

void AVehiclePlayerController::MakeMaintenance_OnServer_Implementation(EItemPropertyType Type)
{
    const auto PawnInterface = Cast<IGameInterface>(GetPawn());
    if (!PawnInterface) return;

    PawnInterface->MakeMaintenance(Type);
}

void AVehiclePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // SetInputMode(FInputModeGameOnly());
    // bShowMouseCursor = false;
    SetInputMode(FInputModeGameAndUI());
    bShowMouseCursor = true;
}

void AVehiclePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void AVehiclePlayerController::FailedToSpawnPawn() {}
