// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuPlayerController.h"
#include "EngineUtils.h"
#include "GameMenu/MenuCameraActor.h"
#include "GameMenu/MenuGameModeBase.h"
#include "GameMenu/MenuVehicleActor.h"
#include "Net/UnrealNetwork.h"

#include "GameLevelsConfig/ValkyriePlayerState.h"

void AMenuPlayerController::SetNewView(EMenuState MenuState)
{
    if (MenuState == EMenuState::GameConfig && MenuVehicleActor)
    {
        SetViewTargetWithBlend(MenuVehicleActor, 1.f, EViewTargetBlendFunction::VTBlend_EaseIn, 0.5f, true);
    }

    if (MenuCameraActorsMap.Contains(MenuState))
    {
        SetViewTargetWithBlend(MenuCameraActorsMap[MenuState], 1.f, EViewTargetBlendFunction::VTBlend_EaseIn, 0.5f, true);
    }

    OnMenuStateChanged.Broadcast(MenuState);
}

void AMenuPlayerController::VehicleItemHasSelected_OnServer_Implementation(const FVehicleItemData& VehicleItemData)
{
    if (MenuVehicleActor)
    {
        MenuVehicleActor->MountVehicleItem_OnSever(VehicleItemData);
    }

    if (const auto ValkyriePlayerState = GetPlayerState<AValkyriePlayerState>())
    {
        ValkyriePlayerState->SaveMountedItem(VehicleItemData);
    }
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

void AMenuPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMenuPlayerController, MenuVehicleActor);
}
