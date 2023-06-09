// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameCoreTypes.h"
#include "ValkyrieGameInstance.generated.h"

UCLASS()
class VALKYRIESTRIKE_API UValkyrieGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    TArray<FLevelData> GetLevelsData() const { return LevelsData; }

    FLevelData GetStartupLevel() const { return StartupLevel; }
    void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    /* used in widget */
    void CreateGame();
    void FindGame();
    void JoinGame();

    FName GetLobbyMapName() const { return LobbyMapName; }
    FName GetMenuMapName() const { return MenuMapName; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly)
    FName MenuMapName;

    UPROPERTY(EditDefaultsOnly)
    FName LobbyMapName;

    UPROPERTY(VisibleAnywhere)
    int32 MaxNumPlayers{4};

    virtual void OnStart() override;

private:
    FLevelData StartupLevel;
};
