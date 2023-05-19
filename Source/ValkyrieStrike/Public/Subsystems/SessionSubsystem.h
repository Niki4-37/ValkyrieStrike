// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameCoreTypes.h"
#include "SessionSubsystem.generated.h"

UCLASS()
class VALKYRIESTRIKE_API USessionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    USessionSubsystem();

    FOnFoundSessionDataSignature OnFoundSessionData;

    void CreateSession(int32 InMaxNumPlayers, bool bIsLAN);
    void UpdateSession();  // private???
    void StartSession();   // Private???
    void EndSession();     // private???
    void DestroySession();
    void FindSessions(int32 MaxSearchResults, bool bIsLAN);
    void JoinSession(/*const FOnlineSessionSearchResult& SearchResult*/);

private:
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

    FDelegateHandle OnUpdateSessionCompleteDelegateHandle;
    FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;

    FDelegateHandle OnStartSessionCompleteDelegateHandle;
    FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

    FOnEndSessionCompleteDelegate OnEndSessionCompleteDelegate;
    FDelegateHandle OnEndSessionCompleteDelegateHandle;

    FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
    FDelegateHandle OnDestroySessionCompleteDelegateHandle;

    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;

    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;

    TSharedPtr<FOnlineSessionSettings> HostSettings;

    TSharedPtr<FOnlineSessionSearch> SearchSettings;

    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnUpdateSessionCompleted(FName SessionName, bool Successful);
    void OnStartSessionCompleted(FName SessionName, bool Successful);
    void OnEndSessionCompleted(FName SessionName, bool Successful);
    void OnDestroySessionCompleted(FName SessionName, bool Successful);
    void OnFindSessionsCompleted(bool Successful);
    void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    bool TryTravelToCurrentSession();
};
