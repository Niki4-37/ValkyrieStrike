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

protected:
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere)
    UStaticMesh* BombMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* ExplodeEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    UPROPERTY(EditAnywhere)
    float Radius{300.f};
    UPROPERTY(EditAnywhere)
    float Damage{0.f};

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void BeginPlay() override;
    virtual void OnDeath() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)  // to private
    ADecorationActor* ExplosiveBomb{nullptr};
};
