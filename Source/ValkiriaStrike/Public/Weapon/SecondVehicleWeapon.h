// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecondVehicleWeapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class VALKIRIASTRIKE_API ASecondVehicleWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASecondVehicleWeapon();

    UFUNCTION(Server, unreliable)
    void MakeShot_OnServer();

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WeaponRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BeemRadius = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReloadingTime{2.f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName{"MuzzleSocket"};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    FTimerHandle ReloadingTimer;

    UPROPERTY(Replicated)
    bool bIsReady{true};
};
