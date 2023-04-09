// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCoreTypes.h"
#include "MenuVehicleActor.generated.h"

class USkeletalMeshComponent;

UCLASS()
class VALKIRIASTRIKE_API AMenuVehicleActor : public AActor
{
    GENERATED_BODY()

public:
    AMenuVehicleActor();

    void MountItemOnVehicle(UClass* Class, EVehicleItemType Type);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USceneComponent* ActorRootComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USkeletalMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName TurretSocketName{"TurretSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SecondWeaponSocketName{"SecondWeaponSocket"};

    virtual void BeginPlay() override;

private:
    TMap<EVehicleItemType, AActor*> AttachedActorsMap;
};

inline void AMenuVehicleActor::MountItemOnVehicle(UClass* Class, EVehicleItemType Type)
{
    auto VehicleItem = GetWorld()->SpawnActor(Class);

    if (!VehicleItem) return;

    FName SocketName = NAME_None;
    switch (Type)
    {
        case EVehicleItemType::Turret: SocketName = TurretSocketName; break;
        case EVehicleItemType::SecondWeapon: SocketName = SecondWeaponSocketName; break;
    }
    if (SocketName.IsNone()) return;

    if (AttachedActorsMap.Contains(Type))
    {
        AttachedActorsMap[Type]->Destroy();
    }

    AttachedActorsMap.Add(Type, VehicleItem);

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    AttachedActorsMap[Type]->AttachToComponent(Mesh, AttachmentRules, SocketName);
}
