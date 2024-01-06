// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Components/RespawnComponent.h"
#include "GameFramework/GameModeBase.h"

URespawnComponent::URespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URespawnComponent::StartRespawning(float TimeToRespawn)
{
    if (!GetWorld()) return;

    GetWorld()->GetTimerManager().SetTimer(RespawningTimer, this, &URespawnComponent::RespawnOwner, TimeToRespawn, false);

    OnStartRespawn_OnClient(TimeToRespawn);
}

void URespawnComponent::UndoRespawn()
{
    if (!GetWorld() || !GetWorld()->GetTimerManager().IsTimerActive(RespawningTimer)) return;
    GetWorld()->GetTimerManager().ClearTimer(RespawningTimer);
}

void URespawnComponent::RespawnOwner()
{
    if (!GetWorld()) return;

    GetWorld()->GetTimerManager().ClearTimer(RespawningTimer);
    if (!GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->RestartPlayer(Cast<AController>(GetOwner()));
}

void URespawnComponent::OnStartRespawn_OnClient_Implementation(float RespawnDelay)
{
    OnStartRespawn.Broadcast(RespawnDelay);
}
