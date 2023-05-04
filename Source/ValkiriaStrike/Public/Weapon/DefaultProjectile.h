// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class VALKIRIASTRIKE_API ADefaultProjectile : public AActor
{
    GENERATED_BODY()

public:
    ADefaultProjectile();

protected:
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* DefaultProjectileMesh;

    UPROPERTY(VisibleDefaultsOnly)
    UProjectileMovementComponent* ProjectileMovementComponent{nullptr};

    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = false;

    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f;

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    UFUNCTION()
    virtual void ProjectileCollisionComponentHit(UPrimitiveComponent* HitComponent,  //
                                                 AActor* OtherActor,                 //
                                                 UPrimitiveComponent* OtherComp,     //
                                                 FVector NormalImpulse,              //
                                                 const FHitResult& Hit);

    void SetShootDirection(const FVector& Direction);

    UFUNCTION()
    void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
