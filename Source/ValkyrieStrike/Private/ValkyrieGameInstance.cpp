// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "ValkyrieGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

UValkyrieGameInstance::UValkyrieGameInstance()
{
    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UValkyrieGameInstance::OnCreateSessionComplete);
    OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UValkyrieGameInstance::OnStartOnlineGameComplete);
    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UValkyrieGameInstance::OnFindSessionsComplete);
    OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UValkyrieGameInstance::OnJoinSessionComplete);
    // OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnDestroySessionComplete);
}

void UValkyrieGameInstance::CreateGame()
{
    auto UniqueNetId = GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
    FName SessionName = FName(FString::Printf(TEXT("Session%d"), FMath::RandHelper(100)));
    HostSession(UniqueNetId, SessionName, true, false, MaxNumPlayers);
}

void UValkyrieGameInstance::FindGame()
{
    auto UniqueNetId = GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
    FindSessions(UniqueNetId, true, false);
}

void UValkyrieGameInstance::JoinGame()
{
    auto UniqueNetId = GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

    // If the Array is not empty, we can go through it
    if (!SearchSettings->SearchResults.Num())
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("No session to join")));
    }

    // Just a SearchResult where we can save the one we want to use, for the case we find more than one!
    // FOnlineSessionSearchResult ResultToJoin;

    for (const auto& SearchResult : SearchSettings->SearchResults)
    {
        if (SearchResult.Session.OwningUserId == UniqueNetId) continue;
        // ResultToJoin = SearchResult;
        // JoinSession(UniqueNetId, "RedHot", ResultToJoin);
        JoinSession(UniqueNetId, "RedHot", SearchResult);
        break;
    }
}

bool UValkyrieGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 InMaxNumPlayers)
{
    IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Called Host Session")));
    if (!OnlineSub)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
        return false;
    }

    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
    if (!Sessions.IsValid() || !UserId.IsValid()) return false;

    /*
    Fill in all the Session Settings that we want to use.

    There are more with SessionSettings.Set(...);
    For example the Map or the GameMode/Type.
    */
    HostSettings = MakeShareable(new FOnlineSessionSettings());

    HostSettings->bIsLANMatch = bIsLAN;
    HostSettings->bUsesPresence = bIsPresence;
    HostSettings->NumPublicConnections = InMaxNumPlayers;
    HostSettings->NumPrivateConnections = 0;
    HostSettings->bAllowInvites = true;
    HostSettings->bAllowJoinInProgress = true;
    HostSettings->bShouldAdvertise = true;
    HostSettings->bAllowJoinViaPresence = true;
    HostSettings->bAllowJoinViaPresenceFriendsOnly = false;

    HostSettings->Set(SETTING_MAPNAME, FString("Red Hot"), EOnlineDataAdvertisementType::ViaOnlineService);

    // Set the delegate to the Handle of the SessionInterface
    OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

    // Our delegate should get called when this is complete (doesn't need to be successful!)
    return Sessions->CreateSession(*UserId, SessionName, *HostSettings);
}

void UValkyrieGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, "Finding sessions");
    // Get the OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

    if (!OnlineSub)
    {
        OnFindSessionsComplete(false);
        return;
    }

    // Get the SessionInterface from our OnlineSubsystem
    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

    if (!Sessions.IsValid() || !UserId.IsValid()) return;

    /*
    Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
    */
    SearchSettings = MakeShareable(new FOnlineSessionSearch());

    SearchSettings->bIsLanQuery = bIsLAN;
    SearchSettings->MaxSearchResults = 20;
    SearchSettings->PingBucketSize = 1000;

    // We only want to set this Query Setting if "bIsPresence" is true
    if (bIsPresence)
    {
        SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
    }

    TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SearchSettings.ToSharedRef();

    // Set the Delegate to the Delegate Handle of the FindSession function
    OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

    // Finally call the SessionInterface function. The Delegate gets called once this is finished
    Sessions->FindSessions(*UserId, SearchSettingsRef);
}

bool UValkyrieGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
    // Get OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

    if (!OnlineSub) return false;

    // Get SessionInterface from the OnlineSubsystem
    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

    if (!Sessions.IsValid() || !UserId.IsValid()) return false;

    // Set the Handle again
    OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

    // Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
    // "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
    return Sessions->JoinSession(*UserId, SessionName, SearchResult);
}

void UValkyrieGameInstance::DestroyClientSession()
{
    auto OnlineSubSystem = IOnlineSubsystem::Get();
    if (!OnlineSubSystem) return;

    auto Session = OnlineSubSystem->GetSessionInterface();
    if (!Session.IsValid()) return;

    OnDestroySessionCompleteDelegateHandle = Session->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
    Session->DestroySession(NAME_GameSession);
}

void UValkyrieGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

    // Get the OnlineSubsystem so we can get the Session Interface
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub) return;

    // Get the Session Interface to call the StartSession function
    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

    if (!Sessions.IsValid()) return;

    // Clear the SessionComplete delegate handle, since we finished this call
    Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
    if (!bWasSuccessful) return;

    // Set the StartSession delegate handle
    OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

    // Our StartSessionComplete delegate should get called after this
    Sessions->StartSession(SessionName);
}

void UValkyrieGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

    // Get the Online Subsystem so we can get the Session Interface
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        // Get the Session Interface to clear the Delegate
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid())
        {
            // Clear the delegate, since we are done with this call
            Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
        }
    }

    // If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
    if (bWasSuccessful)
    {
        UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName, true, "listen");
    }
}

void UValkyrieGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Ping Bucket size: %d"), SearchSettings->PingBucketSize));

    // Get OnlineSubsystem we want to work with
    IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub) return;

    // Get SessionInterface of the OnlineSubsystem
    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
    if (!Sessions.IsValid()) return;

    // Clear the Delegate handle, since we finished this call
    Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

    // Just debugging the Number of Search results. Can be displayed in UMG or something later on
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SearchSettings->SearchResults.Num()));

    // If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
    if (!SearchSettings->SearchResults.Num()) return;

    // OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
    // This is something you can't do in Blueprint for example!
    for (const auto& SearchResult : SearchSettings->SearchResults)
    {
        // use widget to show results
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Sessionname: %s "), *(SearchResult.Session.OwningUserName)));
        int32 NumberOfConnected = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
        OnFoundSessionData.Broadcast(SearchResult.Session.OwningUserName, NumberOfConnected, SearchResult.Session.NumOpenPublicConnections);
    }
}

void UValkyrieGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

    // Get the OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub) return;

    // Get SessionInterface from the OnlineSubsystem
    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

    if (!Sessions.IsValid()) return;

    // Clear the Delegate again
    Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

    // Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
    // This is something the Blueprint Node "Join Session" does automatically!
    APlayerController* const PlayerController = GetFirstLocalPlayerController();

    // We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
    // String for us by giving him the SessionName and an empty String. We want to do this, because
    // Every OnlineSubsystem uses different TravelURLs
    FString TravelURL;

    if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
    {
        // Finally call the ClienTravel. If you want, you could print the TravelURL to see
        // how it really looks like
        PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
    }
}

void UValkyrieGameInstance::DestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
    if (!bWasSuccessful || MenuMapName.IsNone()) return;
    UGameplayStatics::OpenLevel(this, MenuMapName);
}
