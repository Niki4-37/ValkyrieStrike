// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractionActor.h"
#include "GameCoreTypes.h"
#include "WorkshopActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class VALKYRIESTRIKE_API AWorkshopActor : public AInteractionActor
{
    GENERATED_BODY()

public:
    AWorkshopActor();

protected:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FInteractionData> WorkshopTasks;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
