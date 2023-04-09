// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameCoreTypes.h"
#include "MenuGameModeBase.generated.h"

class AMenuVehicleActor;
class APlayerStart;

UCLASS()
class VALKIRIASTRIKE_API AMenuGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMenuGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void StartPlay() override;

    void MountVehicleItem(UClass* Class, EVehicleItemType Type, APlayerController* PC);

    void LaunchGame(APlayerController* PC);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AMenuVehicleActor> MenuVehicleActorClass;

private:
    UPROPERTY()
    TArray<APlayerStart*> SpawnPositions;

    UPROPERTY()
    AMenuVehicleActor* MenuVehicleActor;

    void SpawnMenuVehicleActor();
};
