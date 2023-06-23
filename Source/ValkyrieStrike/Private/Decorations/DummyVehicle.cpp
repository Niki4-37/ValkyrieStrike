// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Decorations/DummyVehicle.h"
#include "Camera/CameraComponent.h"
#include "Weapon/BaseVehicleWeapon.h"

ADummyVehicle::ADummyVehicle()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(ActorRootComp);

    ChassisMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChassisMesh");
    ChassisMesh->SetupAttachment(ActorRootComp);

    const TMap<EVehicleUnitType, FName> FunctionsByType{
        {EVehicleUnitType::Armor, "MountArmor"},          //
        {EVehicleUnitType::Body, "MountBody"},            //
        {EVehicleUnitType::Chassis, "MountChassis"},      //
        {EVehicleUnitType::SecondWeapon, "MountWeapon"},  //
        {EVehicleUnitType::Turret, "MountWeapon"}         //
    };

    for (const TPair<EVehicleUnitType, FName>& Function : FunctionsByType)
    {
        FOnOperationDelegateSignature Delegate;
        Delegate.BindUFunction(this, Function.Value);
        OperationMap.Add(Function.Key, Delegate);
    }
}

void ADummyVehicle::MountVehicleUnit_OnServer(const FVehicleUnitData& UnitData)
{
    OperationMap[UnitData.UnitType].ExecuteIfBound(UnitData);
}

void ADummyVehicle::MountArmor(const FVehicleUnitData& UnitData)
{
    // create static mesh for armor to use this method needed
}

void ADummyVehicle::MountBody(const FVehicleUnitData& UnitData)
{
    ConstructVehicle(MeshComp, UnitData);
    SocketNames = MeshComp->GetAllSocketNames();

    for (const auto& SocketName : SocketNames)
    {
        if (!AttachedActorsMap.Contains(SocketName)) continue;
        AttachedActorsMap[SocketName]->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

        FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
        AttachedActorsMap[SocketName]->AttachToComponent(MeshComp, AttachmentRules, SocketName);
    }
}

void ADummyVehicle::MountChassis(const FVehicleUnitData& UnitData)
{
    ConstructVehicle(ChassisMesh, UnitData);
}

void ADummyVehicle::MountWeapon(const FVehicleUnitData& UnitData)
{
    if (!GetWorld() || !UnitData.UnitComponents.Num() || !UnitData.UnitSpawnClass) return;

    auto VehicleItem = GetWorld()->SpawnActor<ABaseVehicleWeapon>(UnitData.UnitSpawnClass);

    if (!VehicleItem) return;

    FName SocketName{NAME_None};
    for (const auto& UnitComponent : UnitData.UnitComponents)
    {
        if (UnitComponent.ComponentType == EUnitComponentType::Platform)
        {
            SocketName = UnitComponent.SocketName;
        }

        VehicleItem->SetStaticMesh(UnitComponent.ComponentType, UnitComponent.UnitComponentMesh, UnitComponent.SocketName);
    }

    if (AttachedActorsMap.Contains(SocketName))
    {
        AttachedActorsMap[SocketName]->Destroy();
    }

    AttachedActorsMap.Add(SocketName, VehicleItem);

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    AttachedActorsMap[SocketName]->AttachToComponent(MeshComp, AttachmentRules, SocketName);
}

void ADummyVehicle::ConstructVehicle(UStaticMeshComponent* VehicleMeshComponent, const FVehicleUnitData& UnitData)
{
    if (!VehicleMeshComponent) return;
    const auto FoundUnitCompDataPtr = UnitData.UnitComponents.FindByPredicate([](FUnitComponent& UnitComponent) { return UnitComponent.ComponentType == EUnitComponentType::Other; });
    if (!FoundUnitCompDataPtr || !FoundUnitCompDataPtr->UnitComponentMesh) return;
    VehicleMeshComponent->SetRelativeScale3D(FVector(1.f));
    VehicleMeshComponent->SetStaticMesh(FoundUnitCompDataPtr->UnitComponentMesh);
}

// void ADummyVehicle::MountVehicleItem_OnSever_Implementation(const FVehicleItemData& VehicleItemData)
//{
//     MountVehicleItem_Multicast(VehicleItemData);
// }
//
// void ADummyVehicle::MountVehiclePart_OnServer_Implementation(const FVehicleConstructPart& VehiclePart)
//{
//     MountVehiclePart_Multicast(VehiclePart);
// }

// void ADummyVehicle::MountVehicleItem_Multicast_Implementation(const FVehicleItemData& VehicleItemData)
//{
//     if (!GetWorld() || !VehicleItemData.ItemClass) return;
//
//     auto VehicleItem = GetWorld()->SpawnActor(VehicleItemData.ItemClass);
//
//     if (!VehicleItem) return;
//
//     FString EnumNameString{UEnum::GetValueAsName(VehicleItemData.ItemType).ToString()};
//     int32 ScopeIndex = EnumNameString.Find(TEXT("::"), ESearchCase::CaseSensitive);
//     FName SocketName{NAME_None};
//     if (ScopeIndex != INDEX_NONE)
//     {
//         SocketName = FName(*(EnumNameString.Mid(ScopeIndex + 2) + "Socket"));
//     }
//
//     if (AttachedActorsMap.Contains(VehicleItemData.ItemType))
//     {
//         AttachedActorsMap[VehicleItemData.ItemType]->Destroy();
//     }
//
//     AttachedActorsMap.Add(VehicleItemData.ItemType, VehicleItem);
//
//     FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
//     AttachedActorsMap[VehicleItemData.ItemType]->AttachToComponent(MeshComp, AttachmentRules, SocketName);
// }

// void ADummyVehicle::MountVehiclePart_Multicast_Implementation(const FVehicleConstructPart& VehiclePart)
//{
//     switch (VehiclePart.PartType)
//     {
//         case EVehicleConstructPartType::Body:
//             MeshComp->SetRelativeScale3D(FVector(1.f));
//             MeshComp->SetStaticMesh(VehiclePart.PartMesh);
//             SocketNames = MeshComp->GetAllSocketNames();
//             break;
//         case EVehicleConstructPartType::Chassis:
//             ChassisMesh->SetRelativeScale3D(FVector(1.f));
//             ChassisMesh->SetStaticMesh(VehiclePart.PartMesh);
//             break;
//         default: break;
//     }
// }