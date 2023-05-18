// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameCoreTypes.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ValkyrieGameInstance.generated.h"

UCLASS()
class VALKYRIESTRIKE_API UValkyrieGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UValkyrieGameInstance();

    FOnFoundSessionDataSignature OnFoundSessionData;

    TArray<FLevelData> GetLevelsData() const { return LevelsData; }

    FLevelData GetStartupLevel() const { return StartupLevel; }
    void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    /* used in widget */
    void CreateGame();
    void FindGame();
    void JoinGame();

    bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 InMaxNumPlayers);
    void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
    bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

    void DestroyClientSession();

    FName GetLobbyMapName() const { return LobbyMapName; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly)
    FName MenuMapName;

    UPROPERTY(EditDefaultsOnly)
    FName LobbyMapName;

    UPROPERTY(EditDefaultsOnly)
    int32 MaxNumPlayers{4};

private:
    FLevelData StartupLevel;

    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

    FDelegateHandle OnStartSessionCompleteDelegateHandle;
    FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

    FDelegateHandle OnFindSessionsCompleteDelegateHandle;
    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

    FDelegateHandle OnJoinSessionCompleteDelegateHandle;
    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

    FDelegateHandle OnDestroySessionCompleteDelegateHandle;
    FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

    TSharedPtr<FOnlineSessionSearch> SearchSettings;

    TSharedPtr<FOnlineSessionSettings> HostSettings;

    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    void DestroySessionComplete(FName InSessionName, bool bWasSuccessful);
};
