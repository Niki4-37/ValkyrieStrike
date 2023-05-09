// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "MenuVehicleActor.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;

UCLASS()
class VALKYRIESTRIKE_API AMenuVehicleActor : public AActor
{
    GENERATED_BODY()

public:
    AMenuVehicleActor();

    void MountItemOnVehicle(UClass* Class, EVehicleItemType Type);

    UFUNCTION(Server, reliable)
    void MountVehicleItem_OnSever(const FVehicleItemData& VehicleItemData);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USceneComponent* ActorRootComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USkeletalMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName TurretSocketName{"TurretSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SecondWeaponSocketName{"SecondWeaponSocket"};

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY()
    TMap<EVehicleItemType, AActor*> AttachedActorsMap;

    UFUNCTION(NetMulticast, reliable)
    void MountVehicleItem_Multicast(const FVehicleItemData& VehicleItemData);
};
