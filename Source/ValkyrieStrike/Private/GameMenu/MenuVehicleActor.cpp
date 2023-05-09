// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "GameMenu/MenuVehicleActor.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

AMenuVehicleActor::AMenuVehicleActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ActorRootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
    SetRootComponent(ActorRootComp);

    CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComp->SetupAttachment(ActorRootComp);

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
    Mesh->SetupAttachment(ActorRootComp);

    // Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(1))->SetVectorParameterValue()
}

void AMenuVehicleActor::MountItemOnVehicle(UClass* Class, EVehicleItemType Type)
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

void AMenuVehicleActor::MountVehicleItem_OnSever_Implementation(const FVehicleItemData& VehicleItemData)
{
    MountVehicleItem_Multicast(VehicleItemData);
}

void AMenuVehicleActor::BeginPlay()
{
    Super::BeginPlay();
}

void AMenuVehicleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMenuVehicleActor::MountVehicleItem_Multicast_Implementation(const FVehicleItemData& VehicleItemData)
{
    MountItemOnVehicle(VehicleItemData.ItemClass, VehicleItemData.ItemType);
}
