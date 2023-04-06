// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "MenuCameraActor.generated.h"

class UCameraComponent;

UCLASS()
class VALKIRIASTRIKE_API AMenuCameraActor : public AActor
{
    GENERATED_BODY()

public:
    AMenuCameraActor();

    EMenuState GetActorTypeForMenuState() const { return ActorTypeForMenuState; };

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    USceneComponent* ActorRootComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMenuState ActorTypeForMenuState;

    virtual void BeginPlay() override;
};
