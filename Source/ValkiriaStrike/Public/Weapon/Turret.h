// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ADefaultProjectile;
class UBehaviorTree;

UCLASS()
class VALKIRIASTRIKE_API ATurret : public APawn
{
    GENERATED_BODY()

public:
    ATurret();

    UFUNCTION(Server, unreliable)
    void Fire_OnServer(bool bIsPressed);

    void RotateToTarget(AActor* AimActor, float TimerRate);

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* PawnRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FVector MuzzleOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRate{0.5f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ADefaultProjectile> DefaultProjectileClass;

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    FTimerHandle FireTimer;

    FTimerHandle SmoothRotationTimer;

    float Alpha{0.f};

    void MakeShot();
};
