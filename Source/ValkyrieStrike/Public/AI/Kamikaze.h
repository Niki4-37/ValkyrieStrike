// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "Kamikaze.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class VALKYRIESTRIKE_API AKamikaze : public AAICharacter
{
    GENERATED_BODY()

public:
    AKamikaze();

protected:
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* BombMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UNiagaraSystem* ExpoleEffect;

    UPROPERTY(EditAnywhere)
    float Radius{300.f};
    UPROPERTY(EditAnywhere)
    float Damage{0.f};

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    bool bIsExploded{false};
};
