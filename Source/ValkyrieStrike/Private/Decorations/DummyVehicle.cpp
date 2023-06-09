// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Decorations/DummyVehicle.h"
#include "Camera/CameraComponent.h"

ADummyVehicle::ADummyVehicle()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(ActorRootComp);

    ChassisMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChassisMesh");
    ChassisMesh->SetupAttachment(ActorRootComp);
}

void ADummyVehicle::MountVehicleItem_OnSever_Implementation(const FVehicleItemData& VehicleItemData)
{
    MountVehicleItem_Multicast(VehicleItemData);
}

void ADummyVehicle::MountVehiclePart_OnServer_Implementation(const FVehicleConstructPart& VehiclePart)
{
    MountVehiclePart_Multicast(VehiclePart);
}

void ADummyVehicle::MountVehicleItem_Multicast_Implementation(const FVehicleItemData& VehicleItemData)
{
    if (!GetWorld() || !VehicleItemData.ItemClass) return;

    auto VehicleItem = GetWorld()->SpawnActor(VehicleItemData.ItemClass);

    if (!VehicleItem) return;

    FString EnumNameString{UEnum::GetValueAsName(VehicleItemData.ItemType).ToString()};
    int32 ScopeIndex = EnumNameString.Find(TEXT("::"), ESearchCase::CaseSensitive);
    FName SocketName{NAME_None};
    if (ScopeIndex != INDEX_NONE)
    {
        SocketName = FName(*(EnumNameString.Mid(ScopeIndex + 2) + "Socket"));
    }

    if (AttachedActorsMap.Contains(VehicleItemData.ItemType))
    {
        AttachedActorsMap[VehicleItemData.ItemType]->Destroy();
    }

    AttachedActorsMap.Add(VehicleItemData.ItemType, VehicleItem);

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    AttachedActorsMap[VehicleItemData.ItemType]->AttachToComponent(MeshComp, AttachmentRules, SocketName);
}

void ADummyVehicle::MountVehiclePart_Multicast_Implementation(const FVehicleConstructPart& VehiclePart)
{
    switch (VehiclePart.PartType)
    {
        case EVehicleConstructPartType::Body:
            MeshComp->SetRelativeScale3D(FVector(1.f));
            MeshComp->SetStaticMesh(VehiclePart.PartMesh);
            break;
        case EVehicleConstructPartType::Chassis:
            ChassisMesh->SetRelativeScale3D(FVector(1.f));
            ChassisMesh->SetStaticMesh(VehiclePart.PartMesh);
            break;
        default: break;
    }
}