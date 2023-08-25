// Final work on the SkillBox course "Unreal Engine Junior Developer". All assets are publicly available, links in the ReadMe.

#include "Decorations/DummyVehicle.h"
#include "Camera/CameraComponent.h"
#include "Weapon/BaseVehicleWeapon.h"

#include "Engine.h"

ADummyVehicle::ADummyVehicle()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(ActorRootComp);

    ChassisMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChassisMesh");
    ChassisMesh->SetupAttachment(ActorRootComp);
    ChassisMesh->SetMobility(EComponentMobility::Movable);
    ChassisMesh->SetIsReplicated(true);

    const TMap<EVehicleUnitType, FName> FunctionsByType{
        {EVehicleUnitType::Armor, "MountArmor"},                //
        {EVehicleUnitType::Body, "MountBody"},                  //
        {EVehicleUnitType::Chassis, "MountChassis_Multicast"},  //
        {EVehicleUnitType::SecondWeapon, "MountWeapon"},        //
        {EVehicleUnitType::Turret, "MountWeapon"}               //
    };

    for (const TPair<EVehicleUnitType, FName>& Function : FunctionsByType)
    {
        FOnOperationDelegateSignature Delegate;
        Delegate.BindUFunction(this, Function.Value);
        OperationMap.Add(Function.Key, Delegate);
    }
}

void ADummyVehicle::MountVehicleUnit(const FVehicleUnitData& UnitData)
{
    /* handled on server */
    if (!OperationMap.Contains(UnitData.UnitType)) return;
    OperationMap[UnitData.UnitType].ExecuteIfBound(UnitData);
}

void ADummyVehicle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    for (const auto& SocketName : SocketNames)
    {
        if (!AttachedActorsMap[SocketName]) continue;
        AttachedActorsMap[SocketName]->Destroy();
    }
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
        SetActorTransform_Multicast(AttachedActorsMap[SocketName], MeshComp->GetSocketTransform(SocketName));
    }
}

void ADummyVehicle::MountChassis_Multicast_Implementation(const FVehicleUnitData& UnitData)
{
    ConstructVehicle(ChassisMesh, UnitData);
}

void ADummyVehicle::MountWeapon(const FVehicleUnitData& UnitData)
{
    if (!GetWorld() || !UnitData.UnitComponents.Num() || !UnitData.UnitSpawnClass) return;

    auto VehicleWeapon = GetWorld()->SpawnActor<ABaseVehicleWeapon>(UnitData.UnitSpawnClass);

    if (!VehicleWeapon) return;

    FName SocketName{NAME_None};
    for (const auto& UnitComponent : UnitData.UnitComponents)
    {
        if (UnitComponent.ComponentType == EUnitComponentType::Platform)
        {
            SocketName = UnitComponent.SocketName;
        }

        VehicleWeapon->SetStaticMesh(UnitComponent.ComponentType, UnitComponent.UnitComponentMesh, UnitComponent.SocketName);
    }

    if (AttachedActorsMap.Contains(SocketName))
    {
        AttachedActorsMap[SocketName]->Destroy();
    }

    AttachedActorsMap.Add(SocketName, VehicleWeapon);

    AttachedActorsMap[SocketName]->SetActorTransform(MeshComp->GetSocketTransform(SocketName));
}

void ADummyVehicle::ConstructVehicle(UStaticMeshComponent* VehicleMeshComponent, const FVehicleUnitData& UnitData)
{
    if (!VehicleMeshComponent) return;
    const auto FoundUnitCompDataPtr = UnitData.UnitComponents.FindByPredicate([](FUnitComponent& UnitComponent) { return UnitComponent.ComponentType == EUnitComponentType::Other; });
    if (!FoundUnitCompDataPtr || !FoundUnitCompDataPtr->UnitComponentMesh) return;
    VehicleMeshComponent->SetRelativeScale3D(FVector(1.f));
    VehicleMeshComponent->SetStaticMesh(FoundUnitCompDataPtr->UnitComponentMesh);
}

void ADummyVehicle::SetActorTransform_Multicast_Implementation(AActor* TransformingActor, const FTransform& NewTransform)
{
    if (!TransformingActor) return;
    TransformingActor->SetActorTransform(NewTransform);
}
