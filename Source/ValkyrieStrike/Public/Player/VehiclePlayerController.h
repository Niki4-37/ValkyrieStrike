// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameCoreTypes.h"
#include "VehiclePlayerController.generated.h"

class URespawnComponent;

UCLASS()
class VALKYRIESTRIKE_API AVehiclePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AVehiclePlayerController();

    FOnGameStateChangedSignature OnGameStateChanged;

    UFUNCTION(Server, reliable)
    void MakeMaintenance_OnServer(EItemPropertyType Type);

    void ChangeGameState(EValkyrieGameState State);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    URespawnComponent* RespawnComponent;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void SetupInputComponent() override;
    virtual void BeginPlayingState();
    virtual void FailedToSpawnPawn() override;  // to do next
    virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

    void OnPauseGame();
};
