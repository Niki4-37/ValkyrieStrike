// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameLevelsConfig/ValkiriaPlayerState.h"
#include "Net/UnrealNetwork.h"

void AValkiriaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AValkiriaPlayerState, Lives);
}

void AValkiriaPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    if (auto ValkiriaPlayerState = Cast<AValkiriaPlayerState>(PlayerState))
    {
    }
}

void AValkiriaPlayerState::OverrideWith(APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);

    if (auto ValkiriaPlayerState = Cast<AValkiriaPlayerState>(PlayerState))
    {
    }
}
