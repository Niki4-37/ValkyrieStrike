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
    void SetNewView(EMenuState MenuState);

    FOnMenuStateChangedSignature OnMenuStateChanged;

    UFUNCTION(Server, reliable)
    void VehicleItemHasSelected_OnServer(const FVehicleItemData& VehicleItemData);

    void SetMenuVehicleActor(AMenuVehicleActor* MenuVehicle) { MenuVehicleActor = MenuVehicle; }

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY()
    TMap<EMenuState, AMenuCameraActor*> MenuCameraActorsMap;

    UPROPERTY(Replicated)
    AMenuVehicleActor* MenuVehicleActor{nullptr};
};
