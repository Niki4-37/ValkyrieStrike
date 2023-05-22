// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameCoreTypes.h"
#include "VehiclePlayerController.generated.h"

UCLASS()
class VALKYRIESTRIKE_API AVehiclePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    FOnGameStateChangedSignature OnGameStateChanged;

    UFUNCTION(Server, reliable)
    void MakeMaintenance_OnServer(EItemPropertyType Type);

    void ChangeGameState(EValkyrieGameState State);

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void SetupInputComponent() override;
    virtual void BeginPlayingState();
    virtual void FailedToSpawnPawn() override;  // to do

    void OnPauseGame();
};
