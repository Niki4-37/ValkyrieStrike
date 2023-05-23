// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Subsystems/SessionSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ValkyrieGameInstance.h"

USessionSubsystem::USessionSubsystem()
    : OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
      OnUpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted)),
      OnStartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted)),
      OnEndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnEndSessionCompleted)),
      OnDestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted)),
      OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted)),
      OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted))
{
}

void USessionSubsystem::CreateSession(int32 InMaxNumPlayers, bool bIsLAN)
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface();
    if (!SessionInterface.IsValid()) return;

    HostSettings = MakeShareable(new FOnlineSessionSettings());

    HostSettings->bIsLANMatch = bIsLAN;
    HostSettings->bUsesPresence = true;
    HostSettings->NumPublicConnections = InMaxNumPlayers;
    HostSettings->NumPrivateConnections = 0;
    HostSettings->bAllowInvites = true;
    HostSettings->bAllowJoinInProgress = true;
    HostSettings->bShouldAdvertise = true;
    HostSettings->bAllowJoinViaPresence = true;
    HostSettings->bAllowJoinViaPresenceFriendsOnly = false;

    HostSettings->Set(SETTING_MAPNAME, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineService);

    OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

    const auto LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    auto UniqueNetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();

    if (SessionInterface->CreateSession(*UniqueNetId, NAME_GameSession, *HostSettings))
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
    }
}

void USessionSubsystem::UpdateSession()
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (!SessionInterface.IsValid()) return;

    TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*HostSettings));
    UpdatedSessionSettings->Set(SETTING_MAPNAME, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineService);

    OnUpdateSessionCompleteDelegateHandle = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegate);

    if (!SessionInterface->UpdateSession(NAME_GameSession, *UpdatedSessionSettings))
    {
        SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
    }
    else
    {
        HostSettings = UpdatedSessionSettings;
    }
}

void USessionSubsystem::StartSession()
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid()) return;

    OnStartSessionCompleteDelegateHandle = sessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

    if (!sessionInterface->StartSession(NAME_GameSession))
    {
        sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
    }
}

void USessionSubsystem::EndSession()
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (!SessionInterface.IsValid()) return;

    OnEndSessionCompleteDelegateHandle = SessionInterface->AddOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegate);

    if (!SessionInterface->EndSession(NAME_GameSession))
    {
        SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegateHandle);
    }
}

void USessionSubsystem::DestroySession()
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (!SessionInterface.IsValid()) return;

    OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

    if (!SessionInterface->DestroySession(NAME_GameSession))
    {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
    }
}

void USessionSubsystem::FindSessions(int32 MaxSearchResults, bool bIsLAN)
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (!SessionInterface.IsValid()) return;

    OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

    SearchSettings = MakeShareable(new FOnlineSessionSearch());
    SearchSettings->MaxSearchResults = MaxSearchResults;
    SearchSettings->bIsLanQuery = bIsLAN;

    SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const auto LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    auto UniqueNetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();

    if (!SessionInterface->FindSessions(*UniqueNetId, SearchSettings.ToSharedRef()))
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
    }
}

void USessionSubsystem::JoinSession(/*const FOnlineSessionSearchResult& SearchResult*/)
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (!SessionInterface.IsValid())
    {

        return;
    }

    OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    auto UniqueNetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
    FOnlineSessionSearchResult ResultToJoin;
    for (const auto& SearchResult : SearchSettings->SearchResults)
    {
        if (SearchResult.Session.OwningUserId == UniqueNetId) continue;
        ResultToJoin = SearchResult;
        break;
    }

    if (!SessionInterface->JoinSession(*UniqueNetId, NAME_GameSession, ResultToJoin))
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

        // OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
    }
}

void USessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
    }

    if (bWasSuccessful)
    {
        StartSession();
    }
}

void USessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
    if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
    {
        SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
    }
}

void USessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (SessionInterface)
    {
        SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
    }

    FName LobbyMapName = Cast<UValkyrieGameInstance>(GetGameInstance())->GetLobbyMapName();
    if (Successful)
    {
        /*TryTravelToCurrentSession();*/
        UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName, true, "listen");
    }
}

void USessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (SessionInterface)
    {
        SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegateHandle);
    }

    FName MenuMapName = Cast<UValkyrieGameInstance>(GetGameInstance())->GetMenuMapName();
    if (Successful)
    {
        UGameplayStatics::OpenLevel(GetWorld(), MenuMapName, true, "listen");
    }
}

void USessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (sessionInterface)
    {
        sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
    }

    FName MenuMapName = Cast<UValkyrieGameInstance>(GetGameInstance())->GetMenuMapName();
    if (Successful)
    {
        UGameplayStatics::OpenLevel(GetWorld(), MenuMapName, true, "listen");
    }
}

void USessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (SessionInterface)
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
    }

    if (SearchSettings->SearchResults.Num() <= 0)
    {
        // OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
        return;
    }

    for (const auto& SearchResult : SearchSettings->SearchResults)
    {
        // use widget to show results
        int32 NumberOfConnected = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
        OnFoundSessionData.Broadcast(SearchResult.Session.OwningUserName, NumberOfConnected, SearchResult.Session.SessionSettings.NumPublicConnections);
    }
}

void USessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (SessionInterface)
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
    }

    TryTravelToCurrentSession();
}

bool USessionSubsystem::TryTravelToCurrentSession()
{
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
    if (!SessionInterface.IsValid())
    {
        return false;
    }

    FString ConnectString;
    if (!SessionInterface->GetResolvedConnectString(NAME_GameSession, ConnectString))
    {
        return false;
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
    return true;
}
