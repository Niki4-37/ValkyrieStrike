// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameCoreTypes.h"
#include "LobbyGameModeBase.generated.h"

class APlayerStart;
class ALobbyPlayerController;
class ADummyVehicle;

UCLASS()
class VALKYRIESTRIKE_API ALobbyGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALobbyGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    void LaunchGame(APlayerController* PC);

    virtual void PostLogin(APlayerController* NewPlayer);
    virtual void Logout(AController* Exiting);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ADummyVehicle> DummyVehicleClass;

    virtual void StartToLeaveMap() override;

private:
    UPROPERTY()
    TMap<APlayerStart*, APlayerController*> SpawningMap;

    UPROPERTY()
    TArray<ALobbyPlayerController*> Controllers;

    void SpawnLobbyVehicle(APlayerStart* Position, APlayerController* Controller);
};
