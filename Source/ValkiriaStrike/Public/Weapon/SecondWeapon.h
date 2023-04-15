// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "SecondWeapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class VALKIRIASTRIKE_API ASecondWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASecondWeapon();

    UFUNCTION(Server, unreliable)
    void MakeShot_OnServer();

    void ChangeAmmoCapacity(int32 Value);
    int32 GetAmmoCapacity() const { return AmmoCapacity; };

    void SetWeaponData(const FVehicleItemData& Data) { WeaponData = Data; };
    const FVehicleItemData& GetWeaponData() const { return WeaponData; };

protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* WeaponRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BeemRadius = 50.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName{"MuzzleSocket"};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    FTimerHandle ReloadingTimer;

    UPROPERTY(Replicated)
    FVehicleItemData WeaponData;

    UPROPERTY(Replicated)
    bool bIsReady{true};

    UPROPERTY(Replicated)
    int32 AmmoCapacity;

    bool IsEmpty() const { return AmmoCapacity == 0; };
};
