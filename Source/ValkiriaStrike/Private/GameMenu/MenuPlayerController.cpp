// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuPlayerController.h"
#include "EngineUtils.h"
#include "GameMenu/MenuCameraActor.h"

void AMenuPlayerController::SetNewView()
{
    if (SettingsCameraActor)
    {
        SetViewTargetWithBlend(SettingsCameraActor, 2.f, EViewTargetBlendFunction::VTBlend_EaseIn, 1.f, true);
    }
}

void AMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;

    for (const auto CameraActor : TActorRange<AMenuCameraActor>(GetWorld()))
    {
        if (CameraActor->ActorHasTag("Start"))
        {
            StartCameraActor = CameraActor;
        }

        if (CameraActor->ActorHasTag("Settings"))
        {
            SettingsCameraActor = CameraActor;
        }
    }

    if (StartCameraActor)
    {
        SetViewTargetWithBlend(StartCameraActor, 0.f, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
    }
}
