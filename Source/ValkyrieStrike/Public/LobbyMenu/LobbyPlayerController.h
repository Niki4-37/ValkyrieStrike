// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameCoreTypes.h"
#include "LobbyPlayerController.generated.h"

class ADummyVehicle;
class AMenuCameraActor;

UCLASS()
class VALKYRIESTRIKE_API ALobbyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void SetNewView(EMenuState MenuState);
    void GoToMainMenu();

    FOnMenuStateChangedSignature OnMenuStateChanged;

    UFUNCTION(Server, reliable)
    void VehicleItemHasSelected_OnServer(const FVehicleItemData& VehicleItemData);

    void SetLobbyVehicle(ADummyVehicle* Vehicle) { DummyVehicle = Vehicle; }

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void Destroyed() override;

private:
    UPROPERTY()
    TMap<EMenuState, AMenuCameraActor*> MenuCameraActorsMap;

    UPROPERTY(Replicated)
    ADummyVehicle* DummyVehicle{nullptr};

    FTimerHandle ChangeViewDelayTimer;
};
