// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecorationActor.generated.h"

class UHealthComponent;

// class UNiagaraComponent;
class UNiagaraSystem;

class UBoxComponent;

UCLASS()
class VALKYRIESTRIKE_API ADecorationActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADecorationActor();

    void SetupDecoration(UStaticMesh* Mesh);
    void IsDealsDamage(bool bEnabled, float InDamage, float InRadius);
    void SetDestructionTimer(float InTime);
    void SetDestructionEffect(UNiagaraSystem* NewEffect) { DestructionEffect = NewEffect; };

    UFUNCTION(NetMulticast, reliable)
    void Throw_Multicast();

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* ActorRootComp;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* MeshComp;

    UPROPERTY(EditDefaultsOnly)
    UHealthComponent* HitPointComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bDealsDamage{false};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bDealsDamage"), Category = "Config")
    float Damage{100.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bDealsDamage"), Category = "Config")
    float Radius{100.f};

    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    // UNiagaraComponent* DestructionFXComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* DestructionEffect{nullptr};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Gravity{2.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Impulse{25.f};

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void OnDeath();

private:
    FTimerHandle DestructionTimer;

    FTimerHandle TrowTimer;

    void Throw();

    UFUNCTION(NetMulticast, reliable)
    void PlayFXEffect_Multicast(UNiagaraSystem* FXEffect);
};
