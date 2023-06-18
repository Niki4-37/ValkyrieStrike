// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "LobbyMenu/LobbyPlayerController.h"
#include "EngineUtils.h"
#include "GameMenu/MenuCameraActor.h"
#include "Decorations/DummyVehicle.h"
#include "Net/UnrealNetwork.h"
#include "GameLevelsConfig/ValkyriePlayerState.h"
#include "Subsystems/SessionSubsystem.h"

#include "Engine.h"

void ALobbyPlayerController::SetNewView(EMenuState MenuState)
{
    if (MenuState == EMenuState::GameConfig && DummyVehicle)
    {
        SetViewTargetWithBlend(DummyVehicle, 1.f, EViewTargetBlendFunction::VTBlend_EaseIn, 0.5f, true);
    }

    if (MenuCameraActorsMap.Contains(MenuState))
    {
        SetViewTargetWithBlend(MenuCameraActorsMap[MenuState], 1.f, EViewTargetBlendFunction::VTBlend_EaseIn, 0.5f, true);
    }

    OnMenuStateChanged.Broadcast(MenuState);
}

void ALobbyPlayerController::GoToMainMenu()
{
    // GetNetMode() != NM_Client ? GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("SERVER")) : GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("CLIENT"));

    if (const auto SessionSubsystem = GetGameInstance()->GetSubsystem<USessionSubsystem>())
    {
        // GetNetMode() != NM_Client ? SessionSubsystem->DestroySession() : SessionSubsystem->EndSession();
        SessionSubsystem->DestroySession();
    }
}

void ALobbyPlayerController::VehicleItemHasSelected_OnServer_Implementation(const FVehicleItemData& VehicleItemData)
{
    if (DummyVehicle)
    {
        DummyVehicle->MountVehicleItem_OnSever(VehicleItemData);
    }

    if (const auto ValkyriePlayerState = GetPlayerState<AValkyriePlayerState>())
    {
        ValkyriePlayerState->SaveMountedItem(VehicleItemData);
    }
}

void ALobbyPlayerController::VehiclePartHasSelected_OnServer_Implementation(const FVehicleConstructPart& VehicleConstructPart)
{
    if (DummyVehicle)
    {
        DummyVehicle->MountVehiclePart_OnServer(VehicleConstructPart);
    }
}

void ALobbyPlayerController::SetLobbyVehicle(ADummyVehicle* Vehicle)
{
    DummyVehicle = Vehicle;

    GetHUD()->ShowHUD();
}

void ALobbyPlayerController::BeginPlay()
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

    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ALobbyPlayerController::SetNewView, EMenuState::GameConfig);
    GetWorldTimerManager().SetTimer(ChangeViewDelayTimer, TimerDelegate, 0.5f, false);
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ALobbyPlayerController, DummyVehicle);
}

void ALobbyPlayerController::Destroyed()
{
    if (DummyVehicle)
    {
        DummyVehicle->Destroy();
    }

    Super::Destroyed();
}
