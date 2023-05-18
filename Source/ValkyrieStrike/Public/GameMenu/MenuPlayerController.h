// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameCoreTypes.h"
#include "MenuPlayerController.generated.h"

class AMenuCameraActor;
class AMenuVehicleActor;

UCLASS()
class VALKYRIESTRIKE_API AMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    FOnMenuStateChangedSignature OnMenuStateChanged;
    void SetNewView(EMenuState MenuState);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EMenuState, AMenuCameraActor*> MenuCameraActorsMap;
};
