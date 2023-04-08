// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameCoreTypes.h"
#include "MenuPlayerController.generated.h"

class AMenuCameraActor;

UCLASS()
class VALKIRIASTRIKE_API AMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void SetNewView(EMenuState MenuState);

    FOnMenuStateChangedSignature OnMenuStateChanged;

    void MountVehicleItem(UClass* Class, EVehicleItemType Type);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EMenuState, AMenuCameraActor*> MenuCameraActorsMap;
};
