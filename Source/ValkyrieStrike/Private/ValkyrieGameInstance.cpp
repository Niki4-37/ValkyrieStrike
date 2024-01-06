// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "ValkyrieGameInstance.h"
#include "Subsystems/SessionSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

UValkyrieGameInstance::UValkyrieGameInstance()
{
    for (EMusicTheme EMusicTheme = EMusicTheme::Begin; EMusicTheme != EMusicTheme::End; ++EMusicTheme)
    {
        GameMusicMap.Add(EMusicTheme, nullptr);
    }
}

void UValkyrieGameInstance::CreateGame()
{
    GetSubsystem<USessionSubsystem>()->CreateSession(MaxNumPlayers, true);
}

void UValkyrieGameInstance::FindGame()
{
    GetSubsystem<USessionSubsystem>()->FindSessions(20, true);
}

void UValkyrieGameInstance::JoinGame()
{
    GetSubsystem<USessionSubsystem>()->JoinSession();
}

USoundCue* UValkyrieGameInstance::GetGameMusic(EMusicTheme Theme)
{
    if (GameMusicMap.Contains(Theme))
    {
        return GameMusicMap.FindRef(Theme);
    }
    return nullptr;
}

void UValkyrieGameInstance::OnStart()
{
    Super::OnStart();

    FString FPSLock = "t.maxfps 60";
    UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), FPSLock);
}
