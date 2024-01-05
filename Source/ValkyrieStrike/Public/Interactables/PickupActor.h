// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractionActor.h"
#include "GameCoreTypes.h"
#include "PickupActor.generated.h"

class USphereComponent;

UCLASS()
class VALKYRIESTRIKE_API APickupActor : public AInteractionActor
{
    GENERATED_BODY()

public:
    APickupActor();

    void SetupPickup(UStaticMesh* Mesh, const FInteractionData& InData);

    UFUNCTION(NetMulticast, reliable)
    void ThrowUp_Multicast(const FVector& NewLocation);

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInteractionData Data;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoveRate{1.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Gravity{2.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Impulse{50.f};

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
    UPROPERTY()
    TArray<AActor*> SavedActors;

    float Alpha{0.f};

    FTimerHandle MoveTimer;

    FTimerHandle TrowTimer;

    float Height{0.f};

    bool IsAppliedToActor(AActor* OtherActor);
    void ThrowUp(const FVector& StartPosition);
};
