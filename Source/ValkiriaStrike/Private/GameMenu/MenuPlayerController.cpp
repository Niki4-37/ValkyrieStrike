// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuPlayerController.h"
#include "EngineUtils.h"
#include "GameMenu/MenuCameraActor.h"
#include "GameMenu/MenuGameModeBase.h"

void AMenuPlayerController::SetNewView(EMenuState MenuState)
{
    if (MenuCameraActorsMap.Contains(MenuState))
    {
        SetViewTargetWithBlend(MenuCameraActorsMap[MenuState], 1.f, EViewTargetBlendFunction::VTBlend_EaseIn, 0.5f, true);
    }
    OnMenuStateChanged.Broadcast(MenuState);
}

void AMenuPlayerController::MountVehicleItem(const FVehicleItemData& VehicleItemData)
{
    const auto MenuGM = Cast<AMenuGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!MenuGM) return;

    MenuGM->MountVehicleItem(VehicleItemData, this);
}

void AMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;

    for (const auto CameraActor : TActorRange<AMenuCameraActor>(GetWorld()))
    {
        MenuCameraActorsMap.Add(CameraActor->GetActorTypeForMenuState(), CameraActor);
    }

    if (MenuCameraActorsMap.Contains(EMenuState::MainMenu))
    {
        SetViewTargetWithBlend(MenuCameraActorsMap[EMenuState::MainMenu], 0.f, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
    }
    OnMenuStateChanged.Broadcast(EMenuState::MainMenu);
}
