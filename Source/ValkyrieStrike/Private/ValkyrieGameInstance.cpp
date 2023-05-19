// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "ValkyrieGameInstance.h"
#include "Subsystems/SessionSubsystem.h"

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