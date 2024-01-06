// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameCoreTypes.h"
#include "ValkyrieGameInstance.generated.h"

class USoundCue;

UCLASS()
class VALKYRIESTRIKE_API UValkyrieGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UValkyrieGameInstance();

    TArray<FLevelData> GetLevelsData() const { return LevelsData; }

    FLevelData GetStartupLevel() const { return StartupLevel; }
    void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    /* used in widget */
    void CreateGame();
    void FindGame();
    void JoinGame();

    FName GetLobbyMapName() const { return LobbyMapName; }
    FName GetMenuMapName() const { return MenuMapName; }

    USoundCue* GetGameMusic(EMusicTheme Theme);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly)
    FName MenuMapName;

    UPROPERTY(EditDefaultsOnly)
    FName LobbyMapName;

    UPROPERTY(VisibleAnywhere)
    int32 MaxNumPlayers{4};

    UPROPERTY(EditDefaultsOnly, Category = "Music")
    TMap<EMusicTheme, USoundCue*> GameMusicMap;

    virtual void OnStart() override;

private:
    FLevelData StartupLevel;
};
