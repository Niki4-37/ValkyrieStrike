// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameCoreTypes.h"
#include "VehiclePlayerController.generated.h"

class URespawnComponent;
class UAudioComponent;

UCLASS()
class VALKYRIESTRIKE_API AVehiclePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AVehiclePlayerController();

    FOnGameStateChangedSignature OnGameStateChanged;

    UFUNCTION(Server, reliable)
    void MakeMaintenance_OnServer(EItemPropertyType Type);

    UFUNCTION(Client, reliable)
    void ChangeGameState_OnClient(EValkyrieGameState NewState);
    UFUNCTION(Client, reliable)
    void CheckMusicTheme_OnClient(bool bHasAim);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    URespawnComponent* RespawnComponent;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void SetupInputComponent() override;
    virtual void BeginPlayingState();
    virtual void FailedToSpawnPawn() override;  // to do
    virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

private:
    FTimerHandle ChangeMusicTimer;

    UPROPERTY()
    UAudioComponent* CalmMusicComponent{nullptr};

    UPROPERTY()
    UAudioComponent* BattleMusicComponent{nullptr};

    void OnPauseGame();
    void ChangeGameState(EValkyrieGameState State);
    void OnStartRespawn(float RespawnDelay);

    void ToggleMusicTheme();
};
