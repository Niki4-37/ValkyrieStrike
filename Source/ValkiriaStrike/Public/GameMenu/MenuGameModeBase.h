// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameCoreTypes.h"
#include "MenuGameModeBase.generated.h"

class AMenuVehicleActor;
class APlayerStart;
class AMenuPlayerController;

UCLASS()
class VALKIRIASTRIKE_API AMenuGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMenuGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void StartPlay() override;

    void MountVehicleItem(const FVehicleItemData& VehicleItemData, APlayerController* PC);

    void LaunchGame(APlayerController* PC);

    virtual void PostLogin(APlayerController* NewPlayer);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AMenuVehicleActor> MenuVehicleActorClass;

private:
    UPROPERTY()
    TMap<APlayerStart*, bool> SpawnPositionsMap;

    UPROPERTY()
    TArray<AMenuPlayerController*> Controllers;

    void SpawnMenuVehicleActor(AMenuPlayerController* Controller);
};
