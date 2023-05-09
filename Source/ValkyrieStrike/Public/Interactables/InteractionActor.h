// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class VALKYRIESTRIKE_API AInteractionActor : public AActor
{
    GENERATED_BODY()

public:
    AInteractionActor();

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* ActorRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(Replicated)
    bool bHasOwner{false};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
