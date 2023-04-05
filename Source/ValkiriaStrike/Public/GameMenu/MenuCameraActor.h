// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MenuCameraActor.generated.h"

// class ACameraActor;
class UCameraComponent;

UCLASS()
class VALKIRIASTRIKE_API AMenuCameraActor : public AActor
{
    GENERATED_BODY()

public:
    AMenuCameraActor();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    USceneComponent* ActorRootComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    // ACameraActor* MenuActorCamera;
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ActorTag;

    virtual void BeginPlay() override;
};
