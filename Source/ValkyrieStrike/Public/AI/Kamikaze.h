// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "Kamikaze.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraSystem;
class UCameraShakeBase;
class ADecorationActor;

UCLASS()
class VALKYRIESTRIKE_API AKamikaze : public AAICharacter
{
    GENERATED_BODY()

public:
    AKamikaze();

    bool HasBomb() const { return ExplosiveBomb ? true : false; }

    virtual void AttackEnemy(AActor* Target) override;

protected:
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ADecorationActor> BombClass;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void BeginPlay() override;
    virtual void OnDeath() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)
    ADecorationActor* ExplosiveBomb{nullptr};

    void TakeBomb();
    void ThrowBomb(const FVector& ToLocation);
};
