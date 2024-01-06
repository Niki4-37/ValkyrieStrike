// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RespawnComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartRespawnSignature, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALKYRIESTRIKE_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URespawnComponent();

    FOnStartRespawnSignature OnStartRespawn;

    void StartRespawning(float TimeToRespawn);
    void UndoRespawn();

private:
    FTimerHandle RespawningTimer;

    void RespawnOwner();

    UFUNCTION(Client, reliable)
    void OnStartRespawn_OnClient(float RespawnDelay);
};
