// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstLevelGameModeBase.generated.h"

class APlayerStart;
class ASpawningActor;

UCLASS()
class VALKYRIESTRIKE_API AFirstLevelGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFirstLevelGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void HandleSeamlessTravelPlayer(AController*& C) override;

    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    virtual void RestartPlayer(AController* NewPlayer) override;

    void Killed(AController* VictimController, const FTransform& VictimTransform);

    const TArray<ASpawningActor*>& GetSpawningActors() const { return SpawningActors; }

    UFUNCTION(BlueprintCallable)
    void BeginFinalPhase() { bIsFinal = true; };

    virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
    // Create TMap<Controller, StaticMesh> VehicleMeshiesMap
    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    // UStaticMesh* BrokenVehicleMesh;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelConfig")
    TMap<TSubclassOf<AActor>, float> EnemySpawnClassesWithChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelConfig", meta = (ClampMin = "0", ClampMax = "1000"))
    int32 EnemiesToWin{10};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelCongig")
    FName FinalPhaseTagName{"FinalPhase"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelCongig")
    FName PlayerSpawnerTagName{"PlayerSpawner"};

private:
    bool bIsFinal{false};
    int32 Count{0};
    float SumOfWeights{0};

    UPROPERTY()
    TMap<AActor*, bool> PlayerStartMap;

    UPROPERTY()
    TArray<ASpawningActor*> SpawningActors;

    UPROPERTY()
    TArray<ASpawningActor*> PlayerSpawners;

    UPROPERTY()
    TArray<APlayerController*> EnablePlayers;

    void FillPlayerStartMap();
    void FillSpawningActorsArray();

    void StartRespawning(AController* VictimController);
    void RestartPlayerWithPlayerState(AController* NewPlayer);
    void RestartPlayerWithAIController(AController* NewPlayer);

    AActor* GetRandomSpawningActorByTag(FName TagName);

    void GameOver();

    bool GetBestSpawnLocation(const FVector& VictimLocation, FVector& BestLocation);
};
